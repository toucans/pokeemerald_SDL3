/* m4a.c — the GBA m4a (MP2K/"Sappy") engine as a plain C synthesizer.
 *
 * Line-for-line port of pokeemerald-music/web/m4a-worklet.js, which is the
 * verified reference (A/B'd against offline renders and the official GBA
 * recordings — see pokeemerald-music/README.md). The sequencer and envelopes
 * tick once per GBA frame (59.7275 Hz); audio is synthesized per output
 * sample. Any behavior change belongs in the worklet first, then here.
 *
 * Data comes from assets/music.pak (tools/pack_music.py); song/track/sample
 * payloads are read in place from the loaded file buffer.
 */
#include "m4a.h"

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FRAME       (1.0f / 59.7275f)   /* GBA VBlank; sequencer tick */
#define PSG_FULL    (15.0f * 8 / 1024)  /* one full PSG channel vs full-scale PCM */
#define MASTER      0.34f               /* headroom: loudest song peaks ~0.93 */
#define DS_RATE     13379               /* Emerald DirectSound mix rate (reverb timing) */

#define MAX_TRACKS  16                  /* m4a songs use <=10; MIDI originals <=16 */
#define MAX_VOICES  48                  /* live voices per track (JS is unbounded) */
#define REV_MAX     16384               /* reverb ring; needs 7*224/13379*rate + 4 */

/* ---- pak layout (tools/pack_music.py writes these, packed little-endian) ---- */

typedef struct {
    const char *label;
    float rate;
    int loop;
    uint32_t loopStart;
    uint32_t length;
    const int8_t *data;
} Sample;

typedef struct {
    uint8_t type;                       /* 0 pcm, 1 square, 2 wave, 3 noise */
    uint8_t base, fixed, rhythm;
    int8_t pan;
    uint8_t duty, period, pad;
    uint16_t sampleIdx, pad2;
    uint8_t adsr[4];
    uint8_t pattern[32];
} VoiceDef;
_Static_assert(sizeof(VoiceDef) == 48, "pak voice layout");

typedef struct {
    float t;
    uint8_t type;                       /* 0 n,1 v,2 p,3 b,4 br,5 m,6 ls,7 ev,8 el */
    uint8_t key, vel, vid;
    float x;                            /* note: gate seconds; control: value */
} Event;
_Static_assert(sizeof(Event) == 12, "pak event layout");

typedef struct {
    char name[32], title[48], cat[32];
    float reverb, loopStart, loopEnd;   /* loopStart < 0 = non-looping */
    uint8_t keyLo, keyHi;               /* sounding-pitch range, for the viz */
    uint32_t nVoices, nTracks;
    const VoiceDef *voices;
    const Event *ev[MAX_TRACKS];
    uint32_t nEv[MAX_TRACKS];
} Song;

/* ---- music-orig.pak: the composers' original MIDIs + SC-88Pro samples ----
 * (tools/extract_orig.py; GM-subset synthesis, voice type 4) */

typedef struct {
    float rate;
    int loop;
    uint32_t loopStart, loopEnd, length;
    const int16_t *data;
} OSample;

typedef struct {
    uint8_t keyLo, keyHi, velLo, velHi;
    uint16_t sampleIdx;
    uint8_t rootKey, excl;
    int8_t coarse, fine;
    uint16_t att;                       /* initialAttenuation, cB */
    int16_t pan10;                      /* -500..500 (0.1%) */
    int16_t tc[5];                      /* delay,attack,hold,decay,release timecents */
    uint16_t sus;                       /* sustain attenuation, cB */
} OZone;
_Static_assert(sizeof(OZone) == 26, "orig pak zone layout");

typedef struct {
    char label[32];
    uint16_t gm, nZones;
    const OZone *zones;
} OProg;

typedef struct {
    char name[32];
    float loopStart, loopEnd;
    uint32_t nTracks;
    const Event *ev[MAX_TRACKS];
    uint32_t nEv[MAX_TRACKS];
    uint32_t nVids;
    const uint16_t *vids;               /* song voice slot -> prog index */
} OSong;

/* ------------------------------ engine state ------------------------------ */

typedef struct {
    const VoiceDef *def;
    const Sample *smp;
    uint8_t t;                          /* voice type; 4 = GM (orig pak) */
    int playKey, rp, vel, vid;
    float visPitch;                     /* sounding pitch, for the viz */
    uint64_t born;                      /* frame index at note-on */
    double phase, incr;
    float envHeld, glHeld, grHeld;
    /* envelope, precomputed at note-on (JS spawnNote) */
    float gate;
    int a, d, s, r, echoV, echoL;
    float t_a, sus, tau_d, tau_r, valOff, peak;   /* pcm */
    int goal;                                     /* psg */
    int psgCached;
    float psgGl, psgGr;
    /* GM (orig) voice */
    const OZone *oz;
    const OSample *osmp;
    float gBase;                        /* attenuation x velocity gain */
    float zPan;                         /* zone pan, -1..1 */
    float tDelay, tAttack, tHold, tDecay, tRelease, susLvl;
} Voice;

typedef struct {
    const Event *ev;
    uint32_t n, i;
    int pass, done, hasLoop;
    int vol, pan, bend, bendRange, mod, ls, echoV, echoL;
    float bendSemis;                    /* GM: resolved bend in semitones */
    Voice voices[MAX_VOICES];
    int nv;
} Track;

static struct {
    uint8_t *pak;                       /* whole file, owns all pointed-to data */
    Sample *samples;
    uint32_t nSamples;
    Song *songs;
    uint32_t nSongs;
    int rate;

    uint8_t *opak;                      /* music-orig.pak (optional) */
    OSample *oSamples;
    OProg *oProgs;
    OSong *oSongs;
    uint32_t nOSamples, nOProgs, nOSongs;

    const Song *song;
    const OSong *osong;                 /* set instead of song for originals */
    int playing, looping;
    float loopStart, loopEnd, passLen;
    Track tracks[MAX_TRACKS];
    uint32_t nTracks;
    uint64_t frame;
    double framePeriod, nextFrameSample, sampleClock;
    int silentFrames;

    float reverb, revG;
    int revA, revB, revN, revIdx;
    float revBuf[REV_MAX];

    /* Freeverb-style room for the originals (SC-88 defaults to a light
     * hall; the m4a two-tap echo above is a different, GBA-only thing) */
    float gmCombBuf[4][1600], gmCombLP[4];
    float gmApBuf[2][640];
    int gmIdx;

    int8_t lfsr15[32767], lfsr7[127];
    uint8_t noiseTable[60];

    int vizOn;
    uint32_t vizSeq;
    float vizBuf[3 + MAX_TRACKS * MAX_VOICES * 4];
} E;

/* ------------------------- tables (worklet header) ------------------------- */

/* GB square frequency, from gCgbFreqTable/gCgbScaleTable (m4a_tables.c). */
static const int CGB_FREQ[12] = {-2004, -1891, -1785, -1685, -1591, -1501,
                                 -1417, -1337, -1262, -1192, -1125, -1062};

static double sq_freq(int key) {
    int k = key < 36 ? 36 : key > 166 ? 166 : key;
    k -= 36;
    int reg = 2048 + (CGB_FREQ[k % 12] >> (k / 12));
    return 131072.0 / (2048 - reg);
}

/* gNoiseTable: NR43 per key 21..80 -> LFSR clock (Hz). */
static double noise_clock(int key) {
    int i = key - 21;
    if (i < 0) i = 0;
    if (i > 59) i = 59;
    int nr43 = E.noiseTable[i], s = nr43 >> 4, r = nr43 & 7;
    return 524288.0 / (r ? r : 0.5) / pow(2, s + 1);
}

/* gCgb3Vol: the wave channel only has these coarse volume fractions. */
static float wave_vol_code(int level) {
    if (level <= 1) return 0;
    if (level <= 5) return 0.25f;
    if (level <= 9) return 0.5f;
    if (level <= 13) return 0.75f;
    return 1;
}

static void make_lfsr(int8_t *out, int bits, int period7) {
    int lfsr = period7 ? 0x7f : 0x7fff;
    for (int i = 0; i < bits; i++) {
        int bit = (lfsr ^ (lfsr >> 1)) & 1;
        lfsr = (lfsr >> 1) | (bit << (period7 ? 6 : 14));
        out[i] = (lfsr & 1) ? 1 : -1;
    }
}

static const float DUTY[4] = {0.125f, 0.25f, 0.5f, 0.75f};

/* ---- volume / pan (ChnVolSetAsm; linear pan; CgbModVol/CgbPan for PSG) ---- */

static void side_vols(int vel, int vol, int pan, int rp, int *L, int *R) {
    int y = 2 * pan;
    if (y < -128) y = -128;
    if (y > 127) y = 127;
    int volMR = ((y + 128) * 2 * vol) >> 8;
    int volML = ((127 - y) * 2 * vol) >> 8;
    int l = (vel * (127 - rp) * volML) >> 14;
    int r = (vel * (rp + 128) * volMR) >> 14;
    *L = l > 255 ? 255 : l;
    *R = r > 255 ? 255 : r;
}

static void psg_gains(int vel, int vol, int pan, int rp,
                      int *goal, float *gl, float *gr) {
    int l, r;
    side_vols(vel, vol, pan, rp, &l, &r);
    int g = (l + r) >> 4;
    if (g > 15) g = 15;
    float amp = g / 15.0f * PSG_FULL;
    *goal = g;
    *gl = amp;
    *gr = amp;                          /* CgbPan: hard 3-way switch */
    if (r >= l && (r >> 1) >= l) *gl = 0;
    else if (l > r && (l >> 1) >= r) *gr = 0;
}

/* PCM per-side gain: float decomposition (matches the worklet / renders). */
static void pcm_gains_into(Voice *v, int vol, int pan, int rp) {
    int y = 2 * pan;
    if (y < -128) y = -128;
    if (y > 127) y = 127;
    v->glHeld = ((127 - y) / 128.0f) * ((127 - rp) / 128.0f) * (vol / 127.0f);
    v->grHeld = ((y + 128) / 128.0f) * ((rp + 128) / 128.0f) * (vol / 127.0f);
}

/* ------------------------------- envelopes -------------------------------- */
/* Held for the frame, evaluated as functions of dt = seconds since note-on;
 * dt < gate = attack/decay/sustain, dt >= gate = release (+ pseudo-echo).
 * Returns -1 when the voice is dead. */

static float ds_env(const Voice *v, float dt) {
    float f;
    if (dt < v->gate) {
        float att = v->t_a > 0 ? fminf(dt / v->t_a, 1) : 1;
        float dec;
        if (v->d == 0) dec = dt >= v->t_a ? v->sus : 1;
        else dec = dt >= v->t_a ? v->sus + (1 - v->sus) * expf(-(dt - v->t_a) / v->tau_d) : 1;
        f = fminf(att, dec);
    } else if (v->r == 0) {
        return -1;                      /* instant cut -> dead */
    } else {
        float rel = v->valOff * expf(-(dt - v->gate) / v->tau_r);
        if (v->echoV > 0 && v->echoL > 0) {
            float echo = v->echoV / 256.0f;
            float tReach = echo < v->valOff ? v->tau_r * logf(v->valOff / echo) : 0;
            if (dt - v->gate >= tReach + v->echoL * FRAME) return -1;
            f = fmaxf(rel, fminf(echo, v->valOff));
        } else {
            if (rel < 1.0f / 512) return -1;
            f = rel;
        }
    }
    return f * v->peak;
}

static float psg_gated(const Voice *v, float x) {
    int goal = v->goal;
    int susLevel = goal > 0 ? ((goal * v->s + 15) >> 4) : 0;
    float sus = goal > 0 ? (float)susLevel / goal : 0;
    float t_a = v->a * goal * FRAME;
    /* t_a is 0 when goal quantises to 0 (near-silent note): avoid 0/0 = NaN */
    float att = (v->a && t_a > 0) ? fminf(x / t_a, 1) : 1;
    float t_d = v->d * (goal > susLevel ? goal - susLevel : 0) * FRAME;
    float dec;
    if (v->d == 0) dec = x >= t_a ? sus : 1;
    else dec = x < t_a ? 1 : fmaxf(sus, fminf(1, 1 - (x - t_a) / (t_d > 0 ? t_d : 1e-9f) * (1 - sus)));
    return fminf(att, dec);
}

static float psg_env(const Voice *v, float dt) {
    int goal = v->goal;
    if (dt < v->gate) return psg_gated(v, dt);
    if (v->r == 0) return -1;
    float lvl = psg_gated(v, v->gate);
    int echoLvl = (v->echoV > 0 && v->echoL > 0 && goal > 0)
                      ? ((goal * v->echoV + 0xFF) >> 8) : 0;
    float t_r = v->r * fmaxf(1, lvl * goal - echoLvl) * FRAME;
    float rel = fmaxf(0, fminf(1, lvl * (1 - (dt - v->gate) / t_r)));
    if (echoLvl > 0 && goal > 0 && (float)echoLvl / goal < lvl) {
        rel = fmaxf(rel, (float)echoLvl / goal);
        if (dt - v->gate >= t_r + v->echoL * FRAME) return -1;
    } else if (rel <= 0) {
        return -1;
    }
    return rel;
}

/* --------------------- GM voices (the originals pak) ---------------------- */
/* SF2-subset synthesis for the composers' original MIDIs. The gain model is
 * the one the soundfont was authored against: initialAttenuation at the EMU
 * 0.4 convention (players that apply the full spec value are why fluidsynth
 * renders of this bank have instruments 10+ dB apart), and the GM square-law
 * curves for velocity and CC7. */

#define GM_MASTER 0.35f                 /* loudest original (rg_vs_legend) peaks ~0.87 */

static float cb_to_amp(float cb) { return powf(10.0f, -cb / 200.0f); }
static float tc_to_sec(int tc) { return tc <= -12000 ? 0 : powf(2.0f, tc / 1200.0f); }

static void spawn_gm_note(Track *tk, const Event *e) {
    const OSong *s = E.osong;
    if (e->vid >= s->nVids) return;
    uint16_t pi = s->vids[e->vid];
    if (pi >= E.nOProgs) return;
    const OProg *prog = &E.oProgs[pi];
    for (uint32_t zi = 0; zi < prog->nZones; zi++) {
        const OZone *z = &prog->zones[zi];
        if (e->key < z->keyLo || e->key > z->keyHi) continue;
        if (e->vel < z->velLo || e->vel > z->velHi) continue;
        if (z->excl) {                  /* exclusive class: choke (hi-hats) */
            for (int k = 0; k < tk->nv; k++) {
                Voice *o = &tk->voices[k];
                if (o->t == 4 && o->oz->excl == z->excl)
                    o->gate = 0;        /* force into release now */
            }
        }
        if (tk->nv >= MAX_VOICES) return;
        Voice *v = &tk->voices[tk->nv++];
        memset(v, 0, sizeof(*v));
        v->t = 4;
        v->oz = z;
        v->osmp = &E.oSamples[z->sampleIdx];
        v->vid = e->vid;
        v->playKey = e->key;
        v->vel = e->vel;
        v->born = E.frame;
        v->gate = e->x;
        v->visPitch = (float)e->key;
        v->gBase = cb_to_amp(0.4f * z->att) *
                   ((float)(e->vel * e->vel) / (127.0f * 127.0f));
        v->zPan = z->pan10 / 500.0f;
        v->tDelay = tc_to_sec(z->tc[0]);
        v->tAttack = tc_to_sec(z->tc[1]);
        v->tHold = tc_to_sec(z->tc[2]);
        v->tDecay = tc_to_sec(z->tc[3]);
        v->tRelease = fmaxf(tc_to_sec(z->tc[4]), 0.01f);
        v->susLvl = z->sus >= 1440 ? 0 : cb_to_amp(z->sus);
    }
}

/* volume envelope value (amplitude) at dt seconds after note-on, gate-free */
static float gm_env_gated(const Voice *v, float dt) {
    if (dt < v->tDelay) return 0;
    dt -= v->tDelay;
    if (dt < v->tAttack) return dt / v->tAttack;
    dt -= v->tAttack;
    if (dt < v->tHold) return 1;
    dt -= v->tHold;
    /* decay: 96 dB linear-in-dB slope over tDecay, floored at sustain */
    float lvl = v->tDecay > 0 ? powf(10.0f, -(dt / v->tDecay) * 96 / 20.0f) : v->susLvl;
    return fmaxf(lvl, v->susLvl);
}

static int gm_update_voice(Track *tk, Voice *v) {
    float dt = (float)((E.frame - v->born) * (double)FRAME);
    float env;
    if (dt < v->gate) {
        env = gm_env_gated(v, dt);
    } else {
        float lvl = gm_env_gated(v, v->gate);
        env = lvl * powf(10.0f, -((dt - v->gate) / v->tRelease) * 96 / 20.0f);
        if (env < 1e-4f) return 0;      /* -80 dB: dead */
    }
    /* one-shot that ran off the end of its sample */
    if (!v->osmp->loop && v->phase >= v->osmp->length - 1) return 0;
    v->envHeld = env;

    float vib = 0;
    if (tk->mod) {                      /* triangle LFO, 5.2 Hz, mod/127 * 50 cents */
        float ph = fmodf((float)(E.frame - v->born) * FRAME * 5.2f, 1.0f);
        float tri = ph < 0.25f ? ph * 4 : ph < 0.75f ? 2 - ph * 4 : ph * 4 - 4;
        vib = (tk->mod / 127.0f) * 0.5f * tri;
    }
    float semis = (v->playKey - v->oz->rootKey) + v->oz->coarse +
                  v->oz->fine / 100.0f + tk->bendSemis + vib;
    v->visPitch = v->playKey + tk->bendSemis + vib;
    v->incr = v->osmp->rate * pow(2, semis / 12.0) / E.rate;

    float g = v->gBase * ((float)(tk->vol * tk->vol) / (127.0f * 127.0f));
    float p = v->zPan + tk->pan / 64.0f;
    if (p < -1) p = -1;
    if (p > 1) p = 1;
    /* equal-power pan */
    float a = (p + 1) * 0.78539816f;    /* pi/4 */
    v->glHeld = g * cosf(a);
    v->grHeld = g * sinf(a);
    return 1;
}

static float gm_render_sample(Voice *v) {
    const OSample *s = v->osmp;
    double p = v->phase;
    uint32_t n = s->length;
    if (s->loop && s->loopEnd > s->loopStart) {
        if (p >= s->loopEnd) {
            double span = s->loopEnd - s->loopStart;
            p = s->loopStart + fmod(p - s->loopStart, span);
        }
    } else if (p >= n - 1) {
        return 0;
    }
    uint32_t i = (uint32_t)p;
    float frac = (float)(p - i);
    uint32_t j = i + 1 < n ? i + 1 : i;
    v->phase = v->phase + v->incr;
    return (s->data[i] * (1 - frac) + s->data[j] * frac) * (1.0f / 32768);
}

/* fixed light room (Freeverb-style: 4 combs + 2 allpasses, mono in/out) */
static const int GM_COMB[4] = {1214, 1293, 1390, 1476};   /* at 48 kHz */
static const int GM_AP[2] = {605, 480};

static float gm_reverb(float in) {
    float out = 0;
    for (int c = 0; c < 4; c++) {
        float *buf = E.gmCombBuf[c];
        int i = E.gmIdx % GM_COMB[c];
        float y = buf[i];
        E.gmCombLP[c] = y * 0.7f + E.gmCombLP[c] * 0.3f;
        float w = in + E.gmCombLP[c] * 0.79f;
        if (w < 1e-15f && w > -1e-15f) w = 0;   /* flush denormals */
        buf[i] = w;
        out += y;
    }
    for (int a = 0; a < 2; a++) {
        float *buf = E.gmApBuf[a];
        int i = E.gmIdx % GM_AP[a];
        float y = buf[i];
        float w = out + y * 0.5f;
        if (w < 1e-15f && w > -1e-15f) w = 0;
        buf[i] = w;
        out = y - w * 0.5f;
    }
    E.gmIdx++;
    return out * 0.25f;
}

/* ------------------------------- sequencer -------------------------------- */

static void spawn_note(Track *tk, const Event *e) {
    if (e->vid >= E.song->nVoices) return;
    const VoiceDef *def = &E.song->voices[e->vid];
    if (tk->nv >= MAX_VOICES) return;   /* pool full: drop the note */
    Voice *v = &tk->voices[tk->nv++];
    memset(v, 0, sizeof(*v));
    v->def = def;
    v->t = def->type;
    v->vid = e->vid;
    v->playKey = def->rhythm ? def->base : e->key;
    v->visPitch = (float)v->playKey;
    v->rp = def->pan;
    v->vel = e->vel;
    v->born = E.frame;
    v->gate = e->x;
    v->a = def->adsr[0];
    v->d = def->adsr[1];
    v->s = def->adsr[2];
    v->r = def->adsr[3];
    v->echoV = tk->echoV;
    v->echoL = tk->echoL;

    if (v->t == 0) {                    /* pcm */
        if (def->sampleIdx >= E.nSamples) { tk->nv--; return; }
        v->smp = &E.samples[def->sampleIdx];
        v->t_a = v->a >= 255 ? 0 : (255.0f / (v->a > 1 ? v->a : 1)) * FRAME;
        v->sus = v->s / 256.0f;
        v->tau_d = v->d == 0 ? 0 : FRAME / logf(256.0f / v->d);
        v->tau_r = v->r == 0 ? 0 : FRAME / logf(256.0f / v->r);
        v->peak = v->vel / 127.0f;
        /* env value at end of gate (start of release) */
        float vo;
        if (v->gate < v->t_a) vo = v->t_a > 0 ? v->gate / v->t_a : 1;
        else if (v->d == 0) vo = v->sus;
        else vo = v->sus + (1 - v->sus) * expf(-(v->gate - v->t_a) / v->tau_d);
        v->valOff = fminf(1, vo);
    } else {
        float gl, gr;
        psg_gains(v->vel, tk->vol, tk->pan, v->rp, &v->goal, &gl, &gr);
    }
}

static void apply_event(Track *tk, const Event *e) {
    switch (e->type) {
    case 0: if (E.osong) spawn_gm_note(tk, e); else spawn_note(tk, e); break;
    case 1: tk->vol = (int)e->x; break;
    case 2: tk->pan = (int)e->x; break;
    case 3: tk->bend = (int)e->x; break;
    case 4: tk->bendRange = (int)e->x; break;
    case 5: tk->mod = (int)e->x; break;
    case 6: tk->ls = (int)e->x; break;
    case 7: tk->echoV = (int)e->x; break;
    case 8: tk->echoL = (int)e->x; break;
    case 9: tk->bendSemis = e->x; break;          /* GM: bend in semitones */
    }
}

/* Per-frame update: envelope + live gains + pitch. Returns 0 if voice died. */
static int update_voice(Track *tk, Voice *v) {
    float dt = (float)((E.frame - v->born) * (double)FRAME);
    /* vibrato: triangle LFO, freq ls*59.7275/256 Hz, peak depth mod/16 semi */
    float vibSemi = 0;
    if (tk->mod && v->t != 3) {
        float hz = tk->ls * 59.7275f / 256;
        float ph = fmodf((float)(E.frame - v->born) * FRAME * hz, 1.0f);
        float tri = ph < 0.25f ? ph * 4 : ph < 0.75f ? 2 - ph * 4 : ph * 4 - 4;
        vibSemi = (tk->mod * 16 / 256.0f) * tri;
    }
    float bendSemi = tk->bend * tk->bendRange / 64.0f;
    v->visPitch = v->playKey + bendSemi + vibSemi;

    if (v->t == 0) {                    /* pcm */
        float e = ds_env(v, dt);
        if (e < 0) return 0;
        v->envHeld = e;
        pcm_gains_into(v, tk->vol, tk->pan, v->rp);   /* live vol/pan */
        double ratio = v->def->fixed ? 1
                     : pow(2, (v->playKey - 60 + bendSemi + vibSemi) / 12.0);
        v->incr = v->smp->rate * ratio / E.rate;
    } else {
        float e = psg_env(v, dt);
        if (e < 0) return 0;
        v->envHeld = e;
        /* PSG goal/pan snapshot at note-on (matches renders); pitch is live */
        if (!v->psgCached) {
            int goal;
            float gl, gr;
            psg_gains(v->vel, tk->vol, tk->pan, v->rp, &goal, &gl, &gr);
            if (v->t == 2) {            /* wave */
                float sc = wave_vol_code(v->goal) * 15 / (v->goal > 1 ? v->goal : 1);
                gl *= sc;
                gr *= sc;
            }
            v->psgGl = gl;
            v->psgGr = gr;
            v->psgCached = 1;
        }
        v->glHeld = v->psgGl;
        v->grHeld = v->psgGr;
        if (v->t == 1)                  /* square */
            v->incr = sq_freq(v->playKey) * pow(2, (bendSemi + vibSemi) / 12.0) / E.rate;
        else if (v->t == 2)             /* wave: 32 steps at half the square rate */
            v->incr = sq_freq(v->playKey) / 2 * pow(2, (bendSemi + vibSemi) / 12.0) / E.rate;
        else                            /* noise */
            v->incr = noise_clock(v->playKey) / E.rate;
    }
    return 1;
}

static float abs_time(const Track *tk, float s) {
    return tk->pass == 0 ? s
         : E.loopEnd + (tk->pass - 1) * E.passLen + (s - E.loopStart);
}

static void end_song(void) {
    E.playing = 0;
    E.song = NULL;
    E.osong = NULL;
}

/* One GBA frame: fire due events, then update every live voice. */
static void frame_tick(void) {
    float now = (float)(E.frame * (double)FRAME);
    int wrapped = 0;

    for (uint32_t ti = 0; ti < E.nTracks; ti++) {
        Track *tk = &E.tracks[ti];
        while (!tk->done) {
            if (tk->i >= tk->n) {
                if (E.looping && tk->hasLoop) { tk->i = 0; tk->pass++; wrapped = 1; continue; }
                tk->done = 1;
                break;
            }
            const Event *e = &tk->ev[tk->i];
            if (E.looping) {
                if (tk->pass > 0 && e->t < E.loopStart) { tk->i++; continue; }
                if (e->t >= E.loopEnd) { tk->i = tk->n; continue; }
            }
            if (abs_time(tk, e->t) > now) break;
            apply_event(tk, e);
            tk->i++;
        }
        for (int k = tk->nv - 1; k >= 0; k--) {
            Voice *v = &tk->voices[k];
            int alive = v->t == 4 ? gm_update_voice(tk, v) : update_voice(tk, v);
            if (!alive)
                tk->voices[k] = tk->voices[--tk->nv];   /* swap-remove */
        }
    }

    /* stop a finished non-looping song once everything has decayed */
    if (!E.looping) {
        int allDone = 1;
        for (uint32_t ti = 0; ti < E.nTracks; ti++)
            if (!E.tracks[ti].done || E.tracks[ti].nv) allDone = 0;
        if (allDone) {
            if (++E.silentFrames > 30) { end_song(); return; }
        } else {
            E.silentFrames = 0;
        }
    }

    /* viz snapshot: [t, wrapped, n, then vid/pitch/level/pan per live voice] */
    if (E.vizOn) {
        float *o = E.vizBuf;
        *o++ = now;
        *o++ = (float)wrapped;
        float *nslot = o++;
        int n = 0;
        for (uint32_t ti = 0; ti < E.nTracks; ti++) {
            Track *tk = &E.tracks[ti];
            for (int vi = 0; vi < tk->nv; vi++) {
                Voice *v = &tk->voices[vi];
                float g = v->glHeld + v->grHeld;
                float level = (v->t == 0 || v->t == 4)
                                  ? v->envHeld * g * 0.5f
                                  : v->envHeld * g / (2 * PSG_FULL);
                *o++ = (float)v->vid;
                *o++ = v->visPitch;
                *o++ = level > 1 ? 1 : level;
                *o++ = g > 0 ? (v->grHeld - v->glHeld) / g : 0;
                n++;
            }
        }
        *nslot = (float)n;
        E.vizSeq++;
    }
    E.frame++;
}

/* Generate one output sample for a voice and advance its phase. */
static float render_sample(Voice *v) {
    if (v->t == 0) {                    /* pcm */
        const Sample *smp = v->smp;
        const int8_t *data = smp->data;
        uint32_t n = smp->length;
        double p = v->phase;
        if (smp->loop) {
            if (p >= n) {
                double span = n - smp->loopStart;
                p = smp->loopStart + fmod(p - smp->loopStart, span);
            }
        } else if (p >= n - 1) {
            return 0;                   /* one-shot finished */
        }
        uint32_t i = (uint32_t)p;
        float frac = (float)(p - i);
        uint32_t j = i + 1 < n ? i + 1 : i;
        v->phase = p + v->incr;
        return (data[i] * (1 - frac) + data[j] * frac) * (1.0f / 128);
    }
    if (v->t == 1) {                    /* square: 4x oversampled naive pulse */
        float duty = DUTY[v->def->duty & 3];
        double inc = v->incr / 4;
        float acc = 0;
        for (int o = 0; o < 4; o++) {
            acc += fmod(v->phase, 1.0) < duty ? 1 : -1;
            v->phase += inc;
        }
        if (v->phase >= 1024) v->phase = fmod(v->phase, 1.0);
        return acc / 4;
    }
    if (v->t == 2) {                    /* wave */
        const uint8_t *pat = v->def->pattern;
        float pos = (float)fmod(v->phase, 1.0) * 32;
        int i = (int)pos;
        float frac = pos - i;
        v->phase += v->incr;
        if (v->phase >= 1024) v->phase = fmod(v->phase, 1.0);
        return ((pat[i] - 7.5f) * (1 - frac) + (pat[(i + 1) % 32] - 7.5f) * frac) / 7.5f;
    }
    /* noise */
    {
        const int8_t *seq = v->def->period == 1 ? E.lfsr7 : E.lfsr15;
        int len = v->def->period == 1 ? 127 : 32767;
        float s = seq[(uint64_t)v->phase % len];
        v->phase += v->incr;
        if (v->phase >= (double)len * 64) v->phase = fmod(v->phase, (double)len);
        return s;
    }
}

/* --------------------------------- public --------------------------------- */

void m4a_render(float *out, int frames) {
    if (!E.playing || (!E.song && !E.osong)) {
        memset(out, 0, (size_t)frames * 2 * sizeof(float));
        return;
    }
    for (int n = 0; n < frames; n++) {
        /* frame boundary: run the sequencer + envelope/control updates */
        if (E.sampleClock >= E.nextFrameSample) {
            frame_tick();
            E.nextFrameSample += E.framePeriod;
            if (!E.playing) {
                memset(out + n * 2, 0, (size_t)(frames - n) * 2 * sizeof(float));
                return;
            }
        }

        float pcmL = 0, pcmR = 0, psgL = 0, psgR = 0;
        for (uint32_t ti = 0; ti < E.nTracks; ti++) {
            Track *tk = &E.tracks[ti];
            for (int vi = 0; vi < tk->nv; vi++) {
                Voice *v = &tk->voices[vi];
                float s = (v->t == 4 ? gm_render_sample(v) : render_sample(v))
                          * v->envHeld;
                if (v->t == 0 || v->t == 4) { pcmL += s * v->glHeld; pcmR += s * v->grHeld; }
                else                        { psgL += s * v->glHeld; psgR += s * v->grHeld; }
            }
        }

        float l, r;
        if (E.osong) {
            /* originals: dry mix + fixed light room */
            float wet = gm_reverb(0.5f * (pcmL + pcmR));
            l = (pcmL + wet) * GM_MASTER;
            r = (pcmR + wet) * GM_MASTER;
        } else {
            /* m4a reverb: mono two-tap feedback echo on the PCM sub-mix only */
            float e = 0;
            if (E.reverb > 0) {
                float a = E.revBuf[(E.revIdx - E.revA + E.revN) % E.revN];
                float b = E.revBuf[(E.revIdx - E.revB + E.revN) % E.revN];
                e = E.revG * (a + b);
                float w = 0.5f * (pcmL + pcmR) + e;
                /* flush denormals: subnormal feedback tails are 10-100x slower
                 * on x86 — a CPU spike in the middle of silence */
                if (w < 1e-15f && w > -1e-15f) w = 0;
                E.revBuf[E.revIdx] = w;
                E.revIdx = (E.revIdx + 1) % E.revN;
            }
            l = (pcmL + e + psgL) * MASTER;
            r = (pcmR + e + psgR) * MASTER;
        }
        out[n * 2]     = l > 1 ? 1 : l < -1 ? -1 : l;
        out[n * 2 + 1] = r > 1 ? 1 : r < -1 ? -1 : r;
        E.sampleClock++;
    }
}

static void start_tracks(uint32_t nTracks, const Event *const *ev,
                         const uint32_t *nEv, float loopStart, float loopEnd) {
    E.looping = loopStart >= 0 && loopEnd > loopStart;
    E.loopStart = loopStart;
    E.loopEnd = loopEnd;
    E.passLen = E.looping ? loopEnd - loopStart : 0;
    E.nTracks = nTracks;
    for (uint32_t ti = 0; ti < nTracks; ti++) {
        Track *tk = &E.tracks[ti];
        memset(tk, 0, sizeof(*tk));
        tk->ev = ev[ti];
        tk->n = nEv[ti];
        tk->vol = 100;
        tk->bendRange = 2;
        tk->ls = 22;
        tk->hasLoop = !E.looping;
        for (uint32_t i = 0; i < tk->n && !tk->hasLoop; i++)
            if (tk->ev[i].t >= E.loopStart && tk->ev[i].t < E.loopEnd)
                tk->hasLoop = 1;
    }
    E.frame = 0;
    E.framePeriod = E.rate / 59.7275;
    E.nextFrameSample = 0;
    E.sampleClock = 0;
    E.silentFrames = 0;
    E.playing = 1;
    frame_tick();                       /* fire t=0 events before first sample */
}

static void start_song(const Song *s) {
    E.osong = NULL;
    E.song = s;
    /* reverb ring buffer (PCM sub-mix only) */
    E.reverb = s->reverb;
    E.revA = (int)lround(7.0 * 224 / DS_RATE * E.rate);
    E.revB = (int)lround(6.0 * 224 / DS_RATE * E.rate);
    E.revN = (E.revA > E.revB ? E.revA : E.revB) + 4;
    if (E.revN > REV_MAX) E.revN = REV_MAX;
    memset(E.revBuf, 0, sizeof(E.revBuf));
    E.revIdx = 0;
    E.revG = s->reverb / 256.0f;
    start_tracks(s->nTracks, s->ev, s->nEv, s->loopStart, s->loopEnd);
}

static void start_orig_song(const OSong *s) {
    E.song = NULL;
    E.osong = s;
    E.reverb = 0;
    memset(E.gmCombBuf, 0, sizeof(E.gmCombBuf));
    memset(E.gmApBuf, 0, sizeof(E.gmApBuf));
    memset(E.gmCombLP, 0, sizeof(E.gmCombLP));
    E.gmIdx = 0;
    start_tracks(s->nTracks, s->ev, s->nEv, s->loopStart, s->loopEnd);
}

bool m4a_play_name(const char *name) {
    for (uint32_t i = 0; i < E.nSongs; i++) {
        const char *a = E.songs[i].name, *b = name;
        for (; *a && *b; a++, b++)
            if ((*a | 32) != (*b | 32)) break;
        if (!*a && !*b) { start_song(&E.songs[i]); return true; }
    }
    m4a_stop();
    return false;
}

bool m4a_play_index(uint32_t i) {
    if (i >= E.nSongs) return false;
    start_song(&E.songs[i]);
    return true;
}

void m4a_stop(void) {
    E.playing = 0;
    E.song = NULL;
    E.osong = NULL;
}

bool m4a_is_playing(void) { return E.playing; }

const char *m4a_current(void) {
    return E.song ? E.song->name : E.osong ? E.osong->name : NULL;
}

/* --------------------------- originals (orig pak) -------------------------- */

uint32_t m4a_orig_count(void) { return E.nOSongs; }
const char *m4a_orig_name(uint32_t i) { return i < E.nOSongs ? E.oSongs[i].name : ""; }

int m4a_orig_find(const char *name) {
    for (uint32_t i = 0; i < E.nOSongs; i++) {
        const char *a = E.oSongs[i].name, *b = name;
        for (; *a && *b; a++, b++)
            if ((*a | 32) != (*b | 32)) break;
        if (!*a && !*b) return (int)i;
    }
    return -1;
}

bool m4a_play_orig_index(uint32_t i) {
    if (i >= E.nOSongs) return false;
    start_orig_song(&E.oSongs[i]);
    return true;
}

bool m4a_orig_is_current(void) { return E.osong != NULL; }

uint32_t m4a_orig_nvids(uint32_t i) { return i < E.nOSongs ? E.oSongs[i].nVids : 0; }
const char *m4a_orig_vid_label(uint32_t i, uint32_t v) {
    if (i >= E.nOSongs || v >= E.oSongs[i].nVids) return "";
    uint16_t pi = E.oSongs[i].vids[v];
    return pi < E.nOProgs ? E.oProgs[pi].label : "";
}

/* ----------------------- song-table introspection ------------------------- */

uint32_t m4a_song_count(void) { return E.nSongs; }
const char *m4a_song_name(uint32_t i) { return i < E.nSongs ? E.songs[i].name : ""; }
const char *m4a_song_title(uint32_t i) { return i < E.nSongs ? E.songs[i].title : ""; }
const char *m4a_song_cat(uint32_t i) { return i < E.nSongs ? E.songs[i].cat : ""; }
float m4a_song_reverb(uint32_t i) { return i < E.nSongs ? E.songs[i].reverb : 0; }
int m4a_song_key_lo(uint32_t i) { return i < E.nSongs ? E.songs[i].keyLo : 60; }
int m4a_song_key_hi(uint32_t i) { return i < E.nSongs ? E.songs[i].keyHi : 60; }
uint32_t m4a_song_nvoices(uint32_t i) { return i < E.nSongs ? E.songs[i].nVoices : 0; }

static const VoiceDef *voice_at(uint32_t i, uint32_t v) {
    return (i < E.nSongs && v < E.songs[i].nVoices) ? &E.songs[i].voices[v] : NULL;
}
int m4a_voice_type(uint32_t i, uint32_t v) { const VoiceDef *d = voice_at(i, v); return d ? d->type : 0; }
int m4a_voice_duty(uint32_t i, uint32_t v) { const VoiceDef *d = voice_at(i, v); return d ? d->duty : 0; }
int m4a_voice_rhythm(uint32_t i, uint32_t v) { const VoiceDef *d = voice_at(i, v); return d ? d->rhythm : 0; }
int m4a_voice_base(uint32_t i, uint32_t v) { const VoiceDef *d = voice_at(i, v); return d ? d->base : 0; }
const char *m4a_voice_label(uint32_t i, uint32_t v) {
    const VoiceDef *d = voice_at(i, v);
    return (d && d->sampleIdx < E.nSamples) ? E.samples[d->sampleIdx].label : "";
}

void m4a_set_viz(bool on) { E.vizOn = on; }
uint32_t m4a_viz_seq(void) { return E.vizSeq; }
const float *m4a_viz_data(void) { return E.vizBuf; }

/* ------------------------------- pak loading ------------------------------ */

static const uint8_t *rd(const uint8_t **p, void *dst, size_t n) {
    memcpy(dst, *p, n);
    *p += n;
    return *p;
}

bool m4a_init_mem(void *buf, long size, int sample_rate) {
    E.rate = sample_rate;
    E.pak = buf;
    make_lfsr(E.lfsr15, 32767, 0);
    make_lfsr(E.lfsr7, 127, 1);
    int ni = 0;
    for (int s = 13; s >= 0; s--)
        for (int r = 7; r >= 4; r--) E.noiseTable[ni++] = (uint8_t)((s << 4) | r);
    E.noiseTable[ni++] = 3; E.noiseTable[ni++] = 2;
    E.noiseTable[ni++] = 1; E.noiseTable[ni] = 0;

    const uint8_t *p = E.pak;
    uint32_t magic, version;
    rd(&p, &magic, 4);
    rd(&p, &version, 4);
    rd(&p, &E.nSamples, 4);
    rd(&p, &E.nSongs, 4);
    if (magic != 0x5041344d || version != 2)     /* "M4AP" */
        return false;

    E.samples = calloc(E.nSamples, sizeof(Sample));
    for (uint32_t i = 0; i < E.nSamples; i++) {
        Sample *s = &E.samples[i];
        s->label = (const char *)p;
        p += 64;
        rd(&p, &s->rate, 4);
        uint32_t loop;
        rd(&p, &loop, 4);
        s->loop = (int)loop;
        rd(&p, &s->loopStart, 4);
        rd(&p, &s->length, 4);
        s->data = (const int8_t *)p;
        p += (s->length + 3) & ~3u;     /* data padded to 4 bytes in the pak */
    }

    E.songs = calloc(E.nSongs, sizeof(Song));
    for (uint32_t i = 0; i < E.nSongs; i++) {
        Song *s = &E.songs[i];
        rd(&p, s->name, 32);
        rd(&p, s->title, 48);
        rd(&p, s->cat, 32);
        rd(&p, &s->reverb, 4);
        rd(&p, &s->loopStart, 4);
        rd(&p, &s->loopEnd, 4);
        uint32_t range;
        rd(&p, &range, 4);
        s->keyLo = range & 0xFF;
        s->keyHi = (range >> 8) & 0xFF;
        rd(&p, &s->nVoices, 4);
        rd(&p, &s->nTracks, 4);
        s->voices = (const VoiceDef *)p;
        p += s->nVoices * sizeof(VoiceDef);
        if (s->nTracks > MAX_TRACKS) return false;
        for (uint32_t t = 0; t < s->nTracks; t++) {
            rd(&p, &s->nEv[t], 4);
            s->ev[t] = (const Event *)p;
            p += s->nEv[t] * sizeof(Event);
        }
    }
    return p - E.pak <= size;
}

bool m4a_orig_mem(void *buf, long size) {
    E.opak = buf;
    const uint8_t *p = E.opak;
    uint32_t magic, version;
    rd(&p, &magic, 4);
    rd(&p, &version, 4);
    rd(&p, &E.nOSamples, 4);
    rd(&p, &E.nOProgs, 4);
    rd(&p, &E.nOSongs, 4);
    if (magic != 0x4f41344d || version != 1)     /* "M4AO" */
        return false;

    E.oSamples = calloc(E.nOSamples, sizeof(OSample));
    for (uint32_t i = 0; i < E.nOSamples; i++) {
        OSample *s = &E.oSamples[i];
        uint32_t loop;
        rd(&p, &s->rate, 4);
        rd(&p, &loop, 4);
        s->loop = (int)loop;
        rd(&p, &s->loopStart, 4);
        rd(&p, &s->loopEnd, 4);
        rd(&p, &s->length, 4);
        s->data = (const int16_t *)p;
        p += (s->length * 2 + 3) & ~3u;
    }
    E.oProgs = calloc(E.nOProgs, sizeof(OProg));
    for (uint32_t i = 0; i < E.nOProgs; i++) {
        OProg *g = &E.oProgs[i];
        rd(&p, g->label, 32);
        rd(&p, &g->gm, 2);
        rd(&p, &g->nZones, 2);
        g->zones = (const OZone *)p;
        p += (g->nZones * sizeof(OZone) + 3) & ~3u;   /* padded to 4 in the pak */
    }
    E.oSongs = calloc(E.nOSongs, sizeof(OSong));
    for (uint32_t i = 0; i < E.nOSongs; i++) {
        OSong *s = &E.oSongs[i];
        rd(&p, s->name, 32);
        rd(&p, &s->loopStart, 4);
        rd(&p, &s->loopEnd, 4);
        rd(&p, &s->nTracks, 4);
        if (s->nTracks > MAX_TRACKS) return false;
        for (uint32_t t = 0; t < s->nTracks; t++) {
            rd(&p, &s->nEv[t], 4);
            s->ev[t] = (const Event *)p;
            p += s->nEv[t] * sizeof(Event);
        }
        rd(&p, &s->nVids, 4);
        s->vids = (const uint16_t *)p;
        p += (s->nVids * 2 + 3) & ~3u;
    }
    return p - E.opak <= size;
}

#ifndef M4A_NO_STDIO
static void *read_file(const char *path, long *size) {
    FILE *f = fopen(path, "rb");
    if (!f) return NULL;
    fseek(f, 0, SEEK_END);
    *size = ftell(f);
    fseek(f, 0, SEEK_SET);
    void *buf = malloc((size_t)*size);
    if (!buf || fread(buf, 1, (size_t)*size, f) != (size_t)*size) {
        fclose(f);
        free(buf);
        return NULL;
    }
    fclose(f);
    return buf;
}

bool m4a_orig_init(const char *pak_path) {
    long size;
    void *buf = read_file(pak_path, &size);
    if (!buf) return false;
    if (!m4a_orig_mem(buf, size)) {
        fprintf(stderr, "m4a: bad orig pak %s\n", pak_path);
        return false;
    }
    return true;
}

bool m4a_init(const char *pak_path, int sample_rate) {
    FILE *f = fopen(pak_path, "rb");
    if (!f) { fprintf(stderr, "m4a: cannot open %s\n", pak_path); return false; }
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    void *buf = malloc((size_t)size);
    if (!buf || fread(buf, 1, (size_t)size, f) != (size_t)size) {
        fclose(f);
        return false;
    }
    fclose(f);
    if (!m4a_init_mem(buf, size, sample_rate)) {
        fprintf(stderr, "m4a: bad pak %s\n", pak_path);
        return false;
    }
    return true;
}
#endif
