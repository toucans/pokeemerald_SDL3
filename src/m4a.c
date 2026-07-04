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

#define MAX_TRACKS  12
#define MAX_VOICES  48                  /* live voices per track (JS is unbounded) */
#define REV_MAX     16384               /* reverb ring; needs 7*224/13379*rate + 4 */

/* ---- pak layout (tools/pack_music.py writes these, packed little-endian) ---- */

typedef struct {
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
    char name[32], title[48];
    float reverb, loopStart, loopEnd;   /* loopStart < 0 = non-looping */
    uint32_t nVoices, nTracks;
    const VoiceDef *voices;
    const Event *ev[MAX_TRACKS];
    uint32_t nEv[MAX_TRACKS];
} Song;

/* ------------------------------ engine state ------------------------------ */

typedef struct {
    const VoiceDef *def;
    const Sample *smp;
    uint8_t t;                          /* voice type, copied from def */
    int playKey, rp, vel;
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
} Voice;

typedef struct {
    const Event *ev;
    uint32_t n, i;
    int pass, done, hasLoop;
    int vol, pan, bend, bendRange, mod, ls, echoV, echoL;
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

    const Song *song;
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

    int8_t lfsr15[32767], lfsr7[127];
    uint8_t noiseTable[60];
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

/* ------------------------------- sequencer -------------------------------- */

static void spawn_note(Track *tk, const Event *e) {
    if (e->vid >= E.song->nVoices) return;
    const VoiceDef *def = &E.song->voices[e->vid];
    if (tk->nv >= MAX_VOICES) return;   /* pool full: drop the note */
    Voice *v = &tk->voices[tk->nv++];
    memset(v, 0, sizeof(*v));
    v->def = def;
    v->t = def->type;
    v->playKey = def->rhythm ? def->base : e->key;
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
    case 0: spawn_note(tk, e); break;
    case 1: tk->vol = (int)e->x; break;
    case 2: tk->pan = (int)e->x; break;
    case 3: tk->bend = (int)e->x; break;
    case 4: tk->bendRange = (int)e->x; break;
    case 5: tk->mod = (int)e->x; break;
    case 6: tk->ls = (int)e->x; break;
    case 7: tk->echoV = (int)e->x; break;
    case 8: tk->echoL = (int)e->x; break;
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
}

/* One GBA frame: fire due events, then update every live voice. */
static void frame_tick(void) {
    float now = (float)(E.frame * (double)FRAME);

    for (uint32_t ti = 0; ti < E.nTracks; ti++) {
        Track *tk = &E.tracks[ti];
        while (!tk->done) {
            if (tk->i >= tk->n) {
                if (E.looping && tk->hasLoop) { tk->i = 0; tk->pass++; continue; }
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
            if (!update_voice(tk, &tk->voices[k]))
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
    if (!E.playing || !E.song) {
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
                float s = render_sample(v) * v->envHeld;
                if (v->t == 0) { pcmL += s * v->glHeld; pcmR += s * v->grHeld; }
                else           { psgL += s * v->glHeld; psgR += s * v->grHeld; }
            }
        }

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

        float l = (pcmL + e + psgL) * MASTER, r = (pcmR + e + psgR) * MASTER;
        out[n * 2]     = l > 1 ? 1 : l < -1 ? -1 : l;
        out[n * 2 + 1] = r > 1 ? 1 : r < -1 ? -1 : r;
        E.sampleClock++;
    }
}

static void start_song(const Song *s) {
    E.song = s;
    E.looping = s->loopStart >= 0 && s->loopEnd > s->loopStart;
    E.loopStart = s->loopStart;
    E.loopEnd = s->loopEnd;
    E.passLen = E.looping ? s->loopEnd - s->loopStart : 0;
    E.nTracks = s->nTracks;
    for (uint32_t ti = 0; ti < s->nTracks; ti++) {
        Track *tk = &E.tracks[ti];
        memset(tk, 0, sizeof(*tk));
        tk->ev = s->ev[ti];
        tk->n = s->nEv[ti];
        tk->vol = 100;
        tk->bendRange = 2;
        tk->ls = 22;
        tk->hasLoop = !E.looping;
        for (uint32_t i = 0; i < tk->n && !tk->hasLoop; i++)
            if (tk->ev[i].t >= E.loopStart && tk->ev[i].t < E.loopEnd)
                tk->hasLoop = 1;
    }
    /* reverb ring buffer (PCM sub-mix only) */
    E.reverb = s->reverb;
    E.revA = (int)lround(7.0 * 224 / DS_RATE * E.rate);
    E.revB = (int)lround(6.0 * 224 / DS_RATE * E.rate);
    E.revN = (E.revA > E.revB ? E.revA : E.revB) + 4;
    if (E.revN > REV_MAX) E.revN = REV_MAX;
    memset(E.revBuf, 0, sizeof(E.revBuf));
    E.revIdx = 0;
    E.revG = s->reverb / 256.0f;

    E.frame = 0;
    E.framePeriod = E.rate / 59.7275;
    E.nextFrameSample = 0;
    E.sampleClock = 0;
    E.silentFrames = 0;
    E.playing = 1;
    frame_tick();                       /* fire t=0 events before first sample */
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

void m4a_stop(void) {
    E.playing = 0;
    E.song = NULL;
}

bool m4a_is_playing(void) { return E.playing; }

const char *m4a_current(void) { return E.song ? E.song->name : NULL; }

/* ------------------------------- pak loading ------------------------------ */

static const uint8_t *rd(const uint8_t **p, void *dst, size_t n) {
    memcpy(dst, *p, n);
    *p += n;
    return *p;
}

bool m4a_init(const char *pak_path, int sample_rate) {
    E.rate = sample_rate;
    make_lfsr(E.lfsr15, 32767, 0);
    make_lfsr(E.lfsr7, 127, 1);
    int ni = 0;
    for (int s = 13; s >= 0; s--)
        for (int r = 7; r >= 4; r--) E.noiseTable[ni++] = (uint8_t)((s << 4) | r);
    E.noiseTable[ni++] = 3; E.noiseTable[ni++] = 2;
    E.noiseTable[ni++] = 1; E.noiseTable[ni] = 0;

    FILE *f = fopen(pak_path, "rb");
    if (!f) { fprintf(stderr, "m4a: cannot open %s\n", pak_path); return false; }
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    E.pak = malloc((size_t)size);
    if (!E.pak || fread(E.pak, 1, (size_t)size, f) != (size_t)size) {
        fclose(f);
        return false;
    }
    fclose(f);

    const uint8_t *p = E.pak;
    uint32_t magic, version;
    rd(&p, &magic, 4);
    rd(&p, &version, 4);
    rd(&p, &E.nSamples, 4);
    rd(&p, &E.nSongs, 4);
    if (magic != 0x5041344d || version != 1) {   /* "M4AP" */
        fprintf(stderr, "m4a: bad pak %s\n", pak_path);
        return false;
    }

    E.samples = calloc(E.nSamples, sizeof(Sample));
    for (uint32_t i = 0; i < E.nSamples; i++) {
        Sample *s = &E.samples[i];
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
        rd(&p, &s->reverb, 4);
        rd(&p, &s->loopStart, 4);
        rd(&p, &s->loopEnd, 4);
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
    if (p - E.pak > size) {
        fprintf(stderr, "m4a: truncated pak %s\n", pak_path);
        return false;
    }
    return true;
}
