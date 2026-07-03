/* m4a-engine.js — the GBA m4a (MP2K/"Sappy") engine, runtime-agnostic.
 *
 * A plain ES-module class that synthesizes every output sample itself in a
 * per-sample mixing loop, following the decomp's rules (src/m4a.c, src/m4a_1.s).
 * The sequencer and envelopes tick once per GBA frame (59.7275 Hz); audio is
 * generated per output sample — the faithful hardware model, no browser DSP.
 *
 * Reused by BOTH runtimes so the sound is identical either way:
 *   - m4a-worklet.js  → an AudioWorkletProcessor (secure contexts: https/localhost)
 *   - player.js       → a ScriptProcessorNode fallback (plain http, e.g. the
 *                       VPN dashboard, where AudioWorklet is not exposed)
 *
 * Reproduces the exact math validated in render_previews.py (spectrum-checked
 * against the official recordings). See WORKLET_HANDOFF.md §4/§5.
 */

const FRAME = 1 / 59.7275;            // GBA VBlank; sequencer + envelopes tick here
const PSG_FULL = 15 * 8 / 1024;       // one full PSG channel vs full-scale PCM
const MASTER = 0.45;                  // headroom so the loudest song peaks < 1
const DS_RATE = 13379;                // Emerald DirectSound mix rate (reverb timing)

// GB square frequency, from gCgbFreqTable/gCgbScaleTable (m4a_tables.c).
const CGB_FREQ = [-2004, -1891, -1785, -1685, -1591, -1501, -1417, -1337,
                  -1262, -1192, -1125, -1062];
function sqFreq(key) {
  const k = Math.max(36, Math.min(166, key)) - 36;
  const reg = 2048 + (CGB_FREQ[k % 12] >> ((k / 12) | 0));
  return 131072 / (2048 - reg);
}

// gNoiseTable: NR43 per key 21..80 -> LFSR clock (Hz).
const NOISE_TABLE = [];
for (let s = 13; s >= 0; s--) for (const r of [7, 6, 5, 4]) NOISE_TABLE.push((s << 4) | r);
NOISE_TABLE.push(3, 2, 1, 0);
function noiseClock(key) {
  const nr43 = NOISE_TABLE[Math.max(0, Math.min(59, key - 21))];
  const s = nr43 >> 4, r = nr43 & 7;
  return 524288 / (r || 0.5) / Math.pow(2, s + 1);
}

// gCgb3Vol: the wave channel only has these coarse volume fractions.
function waveVolCode(level) {
  if (level <= 1) return 0;
  if (level <= 5) return 0.25;
  if (level <= 9) return 0.5;
  if (level <= 13) return 0.75;
  return 1;
}

// Precompute the two GB LFSR sequences (15-bit and 7-bit), ±1.
function makeLfsr(period7) {
  const bits = period7 ? 127 : 32767;
  const out = new Float32Array(bits);
  let lfsr = period7 ? 0x7f : 0x7fff;
  for (let i = 0; i < bits; i++) {
    const bit = (lfsr ^ (lfsr >> 1)) & 1;
    lfsr = (lfsr >> 1) | (bit << (period7 ? 6 : 14));
    out[i] = (lfsr & 1) ? 1 : -1;
  }
  return out;
}
const LFSR15 = makeLfsr(false), LFSR7 = makeLfsr(true);

/* ---- volume / pan (ChnVolSetAsm; linear pan; CgbModVol/CgbPan for PSG) ---- */

function sideVols(vel, vol, pan, rp) {
  const y = Math.max(-128, Math.min(127, 2 * pan));
  const volMR = ((y + 128) * 2 * vol) >> 8;
  const volML = ((127 - y) * 2 * vol) >> 8;
  return [Math.min(255, (vel * (127 - rp) * volML) >> 14),   // L
          Math.min(255, (vel * (rp + 128) * volMR) >> 14)];  // R
}

function psgGains(vel, vol, pan, rp) {
  const [l, r] = sideVols(vel, vol, pan, rp);
  const goal = Math.min(15, (l + r) >> 4);
  const amp = goal / 15 * PSG_FULL;
  let gl = amp, gr = amp;                      // CgbPan: hard 3-way switch
  if (r >= l && (r >> 1) >= l) gl = 0;
  else if (l > r && (l >> 1) >= r) gr = 0;
  return { goal, gl, gr };
}

// PCM per-side gain: float decomposition (matches render_previews / player.js).
function pcmGains(vol, pan, rp) {
  const y = Math.max(-128, Math.min(127, 2 * pan));
  return [((127 - y) / 128) * ((127 - rp) / 128) * (vol / 127),
          ((y + 128) / 128) * ((rp + 128) / 128) * (vol / 127)];
}

/* ------------------------------- envelopes -------------------------------- */
/* Evaluated at frame boundaries (held for the frame) as functions of dt =
 * seconds since note-on; exact continuous forms from render_previews.py. */

function dsEnv(v, dt) {
  const { d, r, gate, tau_d, tau_r, sus, t_a, valOff, echoV, echoL, peak } = v.env;
  let f;
  if (dt < gate) {
    const att = t_a > 0 ? Math.min(dt / t_a, 1) : 1;
    let dec;
    if (d === 0) dec = dt >= t_a ? sus : 1;
    else dec = dt >= t_a ? sus + (1 - sus) * Math.exp(-(dt - t_a) / tau_d) : 1;
    f = Math.min(att, dec);
  } else if (r === 0) {
    return -1;                                  // instant cut -> dead
  } else {
    const rel = valOff * Math.exp(-(dt - gate) / tau_r);
    if (echoV > 0 && echoL > 0) {
      const echo = echoV / 256;
      const tReach = echo < valOff ? tau_r * Math.log(valOff / echo) : 0;
      if (dt - gate >= tReach + echoL * FRAME) return -1;
      f = Math.max(rel, Math.min(echo, valOff));
    } else {
      if (rel < 1 / 512) return -1;
      f = rel;
    }
  }
  return f * peak;
}

function psgEnv(v, dt) {
  const { a, d, s, r, gate, goal, echoV, echoL } = v.env;
  const susLevel = goal > 0 ? ((goal * s + 15) >> 4) : 0;
  const sus = goal > 0 ? susLevel / goal : 0;
  const t_a = a * goal * FRAME;
  const gated = (x) => {
    const att = a ? Math.min(x / t_a, 1) : 1;
    const t_d = d * Math.max(0, goal - susLevel) * FRAME;
    let dec;
    if (d === 0) dec = x >= t_a ? sus : 1;
    else dec = x < t_a ? 1 : Math.max(sus, Math.min(1, 1 - (x - t_a) / (t_d || 1e-9) * (1 - sus)));
    return Math.min(att, dec);
  };
  if (dt < gate) return gated(dt);
  if (r === 0) return -1;
  const lvl = gated(gate);
  const echoLvl = (echoV > 0 && echoL > 0 && goal > 0) ? ((goal * echoV + 0xFF) >> 8) : 0;
  const t_r = r * Math.max(1, lvl * goal - echoLvl) * FRAME;
  let rel = Math.max(0, Math.min(1, lvl * (1 - (dt - gate) / t_r)));
  if (echoLvl > 0 && echoLvl / goal < lvl) {
    rel = Math.max(rel, echoLvl / goal);
    if (dt - gate >= t_r + echoL * FRAME) return -1;
  } else if (rel <= 0) {
    return -1;
  }
  return rel;
}

/* ------------------------------- the engine ------------------------------- */

export class M4AEngine {
  constructor(sampleRate) {
    this.sr = sampleRate;
    this.bank = null;
    this.song = null;
    this.playing = false;
    this.onended = null;                        // called when a song finishes
  }

  loadBank(bank) { this.bank = bank; }          // {label:{rate,loop,loopStart,data:Int8Array}}

  play(song) { this.startSong(song); }

  stop() { this.playing = false; this.song = null; }

  startSong(song) {
    const looping = song.loopStart != null && song.loopEnd != null &&
                    song.loopEnd > song.loopStart;
    this.song = song;
    this.looping = looping;
    this.loopStart = song.loopStart;
    this.loopEnd = song.loopEnd;
    this.passLen = looping ? song.loopEnd - song.loopStart : 0;
    this.tracks = song.tracks.map((ev) => ({
      ev, i: 0, pass: 0, done: false,
      hasLoop: !looping || ev.some((e) => e[0] >= song.loopStart && e[0] < song.loopEnd),
      vol: 100, pan: 0, bend: 0, bendRange: 2, mod: 0, ls: 22, echoV: 0, echoL: 0,
      voices: [],
    }));
    this.reverb = song.reverb || 0;
    this.revA = Math.round(7 * 224 / DS_RATE * this.sr);
    this.revB = Math.round(6 * 224 / DS_RATE * this.sr);
    this.revN = Math.max(this.revA, this.revB) + 4;
    this.revBuf = new Float32Array(this.revN);
    this.revIdx = 0;
    this.revG = this.reverb / 256;

    this.frame = 0;
    this.framePeriod = this.sr / 59.7275;
    this.nextFrameSample = 0;
    this.sampleClock = 0;
    this.silentFrames = 0;
    this.playing = true;
    this.frameTick();                           // fire t=0 events before first sample
  }

  absTime(tk, s) {
    return tk.pass === 0 ? s
      : this.loopEnd + (tk.pass - 1) * this.passLen + (s - this.loopStart);
  }

  // One GBA frame: fire due events, then update every live voice.
  frameTick() {
    const now = this.frame * FRAME;

    for (const tk of this.tracks) {
      while (!tk.done) {
        if (tk.i >= tk.ev.length) {
          if (this.looping && tk.hasLoop) { tk.i = 0; tk.pass++; continue; }
          tk.done = true; break;
        }
        const e = tk.ev[tk.i];
        const s = e[0];
        if (this.looping) {
          if (tk.pass > 0 && s < this.loopStart) { tk.i++; continue; }
          if (s >= this.loopEnd) { tk.i = tk.ev.length; continue; }
        }
        if (this.absTime(tk, s) > now) break;
        this.applyEvent(tk, e);
        tk.i++;
      }
      for (let k = tk.voices.length - 1; k >= 0; k--) {
        if (!this.updateVoice(tk, tk.voices[k])) tk.voices.splice(k, 1);
      }
    }

    if (!this.looping) {
      const allDone = this.tracks.every((t) => t.done && t.voices.length === 0);
      if (allDone) { this.silentFrames++; if (this.silentFrames > 30) this.endSong(); }
      else this.silentFrames = 0;
    }
    this.frame++;
  }

  applyEvent(tk, e) {
    switch (e[1]) {
      case 'n': this.spawnNote(tk, e); break;
      case 'v': tk.vol = e[2]; break;
      case 'p': tk.pan = e[2]; break;
      case 'b': tk.bend = e[2]; break;
      case 'br': tk.bendRange = e[2]; break;
      case 'm': tk.mod = e[2]; break;
      case 'ls': tk.ls = e[2]; break;
      case 'ev': tk.echoV = e[2]; break;
      case 'el': tk.echoL = e[2]; break;
    }
  }

  spawnNote(tk, e) {
    const [, , key, vel, dur, vid] = e;
    const voice = this.song.voices[vid];
    if (!voice) return;
    const playKey = voice.rhythm ? voice.base : key;
    const rp = voice.pan !== undefined ? voice.pan : 0;
    const v = {
      voice, playKey, rp, vel, born: this.frame, phase: 0,
      env: null, envHeld: 0, glHeld: 0, grHeld: 0, incr: 0, t: voice.t,
    };

    if (voice.t === 'pcm') {
      v.smp = this.bank[voice.sample];
      const [a, d, ss, r] = voice.adsr;
      const t_a = a >= 255 ? 0 : (255 / Math.max(1, a)) * FRAME;
      const sus = ss / 256;
      const tau_d = d === 0 ? 0 : FRAME / Math.log(256 / d);
      const tau_r = r === 0 ? 0 : FRAME / Math.log(256 / r);
      let vo;
      if (dur < t_a) vo = t_a > 0 ? dur / t_a : 1;
      else if (d === 0) vo = sus;
      else vo = sus + (1 - sus) * Math.exp(-(dur - t_a) / tau_d);
      v.env = { a, d, s: ss, r, gate: dur, t_a, sus, tau_d, tau_r,
                echoV: tk.echoV, echoL: tk.echoL, peak: vel / 127, valOff: Math.min(1, vo) };
    } else {
      const g = psgGains(vel, tk.vol, tk.pan, rp);
      const [a, d, ss, r] = voice.adsr;
      v.env = { a, d, s: ss, r, gate: dur, goal: g.goal, echoV: tk.echoV, echoL: tk.echoL };
      v.goal = g.goal;
    }
    tk.voices.push(v);
  }

  // Per-frame update: envelope + live gains + pitch. Returns false if voice died.
  updateVoice(tk, v) {
    const dt = (this.frame - v.born) * FRAME;
    const voice = v.voice;
    let vibSemi = 0;
    if (tk.mod && v.t !== 'noise') {
      const hz = tk.ls * 59.7275 / 256;
      const ph = (dt * hz) % 1;
      const tri = ph < 0.25 ? ph * 4 : ph < 0.75 ? 2 - ph * 4 : ph * 4 - 4;
      vibSemi = (tk.mod * 16 / 256) * tri;
    }
    const bendSemi = tk.bend * tk.bendRange / 64;

    if (v.t === 'pcm') {
      const e = dsEnv(v, dt);
      if (e < 0) return false;
      v.envHeld = e;
      const [gl, gr] = pcmGains(tk.vol, tk.pan, v.rp);   // live vol/pan
      v.glHeld = gl; v.grHeld = gr;
      const ratio = voice.fixed ? 1 : Math.pow(2, (v.playKey - 60 + bendSemi + vibSemi) / 12);
      v.incr = v.smp.rate * ratio / this.sr;
    } else {
      const e = psgEnv(v, dt);
      if (e < 0) return false;
      v.envHeld = e;
      let gl = v.env.__gl, gr = v.env.__gr;               // PSG goal/pan snapshot at note-on
      if (gl === undefined) {
        const g = psgGains(v.vel, tk.vol, tk.pan, v.rp);
        gl = g.gl; gr = g.gr;
        if (v.t === 'wave') {
          const sc = waveVolCode(v.goal) * 15 / Math.max(v.goal, 1);
          gl *= sc; gr *= sc;
        }
        v.env.__gl = gl; v.env.__gr = gr;
      }
      v.glHeld = gl; v.grHeld = gr;
      if (v.t === 'sq') {
        v.incr = sqFreq(v.playKey) * Math.pow(2, (bendSemi + vibSemi) / 12) / this.sr;
      } else if (v.t === 'wave') {
        v.incr = sqFreq(v.playKey) / 2 * Math.pow(2, (bendSemi + vibSemi) / 12) / this.sr;
      } else {
        v.incr = noiseClock(v.playKey) / this.sr;
      }
    }
    return true;
  }

  endSong() {
    this.playing = false;
    this.song = null;
    if (this.onended) this.onended();
  }

  renderSample(v) {
    const voice = v.voice;
    let s = 0;
    if (v.t === 'pcm') {
      const smp = v.smp, data = smp.data, n = data.length;
      let p = v.phase;
      if (smp.loop) {
        if (p >= n) { const span = n - smp.loopStart; p = smp.loopStart + ((p - smp.loopStart) % span); }
      } else if (p >= n - 1) {
        return 0;
      }
      const i = p | 0, frac = p - i, j = i + 1 < n ? i + 1 : i;
      s = (data[i] * (1 - frac) + data[j] * frac) / 128;
      v.phase = p + v.incr;
    } else if (v.t === 'sq') {
      const duty = [0.125, 0.25, 0.5, 0.75][voice.duty];
      const inc = v.incr / 4;
      let acc = 0;
      for (let o = 0; o < 4; o++) { acc += (v.phase % 1) < duty ? 1 : -1; v.phase += inc; }
      s = acc / 4;
    } else if (v.t === 'wave') {
      const pat = voice.pattern;
      const pos = (v.phase % 1) * 32;
      const i = pos | 0, frac = pos - i;
      s = ((pat[i] - 7.5) * (1 - frac) + (pat[(i + 1) % 32] - 7.5) * frac) / 7.5;
      v.phase += v.incr;
    } else {
      const seq = voice.period === 1 ? LFSR7 : LFSR15;
      s = seq[(v.phase | 0) % seq.length];
      v.phase += v.incr;
    }
    return s;
  }

  // Fill N frames of stereo output. Silent (and returns) if not playing.
  render(L, R, N) {
    if (!this.playing || !this.song) { L.fill(0); if (R !== L) R.fill(0); return; }
    for (let n = 0; n < N; n++) {
      if (this.sampleClock >= this.nextFrameSample) {
        this.frameTick();
        this.nextFrameSample += this.framePeriod;
        if (!this.playing) { for (let m = n; m < N; m++) { L[m] = 0; if (R !== L) R[m] = 0; } return; }
      }
      let pcmL = 0, pcmR = 0, psgL = 0, psgR = 0;
      for (const tk of this.tracks) {
        for (const v of tk.voices) {
          const smp = this.renderSample(v) * v.envHeld;
          if (v.t === 'pcm') { pcmL += smp * v.glHeld; pcmR += smp * v.grHeld; }
          else { psgL += smp * v.glHeld; psgR += smp * v.grHeld; }
        }
      }
      let e = 0;
      if (this.reverb > 0) {
        const buf = this.revBuf, sz = this.revN;
        const a = buf[(this.revIdx - this.revA + sz) % sz];
        const b = buf[(this.revIdx - this.revB + sz) % sz];
        e = this.revG * (a + b);
        buf[this.revIdx] = 0.5 * (pcmL + pcmR) + e;
        this.revIdx = (this.revIdx + 1) % sz;
      }
      L[n] = (pcmL + e + psgL) * MASTER;
      R[n] = (pcmR + e + psgR) * MASTER;
      this.sampleClock++;
    }
  }
}
