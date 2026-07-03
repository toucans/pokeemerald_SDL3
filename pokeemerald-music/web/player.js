/* WebAudio player for pokeemerald overworld BGM.
 *
 * Plays the JSON extracted by extract.py with a model of the GBA m4a (MP2K)
 * engine taken from the decomp source (src/m4a.c, src/m4a_1.s), aiming for
 * the "faithful remaster" sound: the exact sequence, mix and synthesis rules
 * of the GBA, minus its output degradation (13.4 kHz nearest-neighbour
 * resampling of the sample channels).
 *
 * Engine facts implemented here:
 * - note side volume = velocity * panFactor * trackVol >> 14 (ChnVolSetAsm);
 *   pan is LINEAR; a drum voice's own pan MULTIPLIES the track pan.
 * - PSG channels (squares/wave/noise) are quantised: volume goal =
 *   clamp15((sideL+sideR)/16); one PSG channel at full volume has 8*15/1024
 *   = ~0.117 of a full-scale PCM channel's amplitude (SOUNDCNT_H all-full);
 *   wave channel volume only has 25/50/75/100% steps (gCgb3Vol); PSG pan is
 *   a hard 3-way switch (CgbPan: 2:1 volume ratio flips a side off).
 *   PSG is mixed in hardware AFTER the DAC - full bandwidth, and NO reverb.
 * - PSG frequency comes from GB registers: f = 131072/(2048-reg) for
 *   squares, half that for the 32-step wave (patterns hold 2 cycles);
 *   noise = LFSR clocked at 524288/r/2^(s+1) via gNoiseTable[key-21].
 * - vibrato: triangle LFO, freq = lfoSpeed*59.7275/256 Hz, peak depth =
 *   mod/16 semitones (pitM += 16*modM, 256 units/semitone).
 * - "reverb" (only on the PCM mix): the mixer re-feeds its own DMA buffer,
 *   ~7 frames (117 ms) old, mono-summed, at reverb/128 - a feedback echo.
 */
"use strict";

const FRAME = 1 / 59.7275;          // GBA VBlank; m4a envelopes tick per frame
const PSG_FULL = 15 * 8 / 1024;     // one full PSG channel vs full-scale PCM
const DATA = "data/";

let ctx = null;
const buffers = {};                  // sample label -> AudioBuffer
let samplesJson = null;
const songCache = {};
let current = null;                  // running playback {stop()}

/* ---------------------------------------------------------------- loading */

async function fetchJson(p) { return (await fetch(DATA + p)).json(); }

function b64ToFloats(b64) {
  const raw = atob(b64);
  const out = new Float32Array(raw.length);
  for (let i = 0; i < raw.length; i++) {
    let v = raw.charCodeAt(i);
    if (v > 127) v -= 256;
    out[i] = v / 128;
  }
  return out;
}

function ensureCtx() {
  if (!ctx) ctx = new (window.AudioContext || window.webkitAudioContext)();
  if (ctx.state === "suspended") ctx.resume();
}

function sampleBuffer(label) {
  if (!buffers[label]) {
    const s = samplesJson[label];
    const data = b64ToFloats(s.b64);
    const buf = ctx.createBuffer(1, data.length, s.rate);
    buf.copyToChannel(data, 0);
    buffers[label] = buf;
  }
  return buffers[label];
}

/* --------------------------------------------------- GB PSG frequency math */

// gCgbFreqTable/gCgbScaleTable from m4a_tables.c: GB frequency register for
// key 36..166; key 60 = middle C. Returns the 11-bit register value.
const CGB_FREQ = [-2004, -1891, -1785, -1685, -1591, -1501, -1417, -1337,
                  -1262, -1192, -1125, -1062];
function cgbFreqReg(key) {
  let k = Math.max(36, Math.min(166, key)) - 36;
  const oct = k / 12 | 0, semi = k % 12;
  return 2048 + (CGB_FREQ[semi] >> oct);
}
const sqFreq = key => 131072 / (2048 - cgbFreqReg(key));

// gNoiseTable: NR43 value per key 21..80 -> LFSR clock in Hz.
const NOISE_TABLE = [];
{
  const rows = [0xD, 0xC, 0xB, 0xA, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0];
  for (const s of rows) for (const r of [7, 6, 5, 4]) NOISE_TABLE.push((s << 4) | r);
  NOISE_TABLE.push(0x03, 0x02, 0x01, 0x00);
}
function noiseClock(key) {
  const nr43 = NOISE_TABLE[Math.max(0, Math.min(59, key - 21))];
  const s = nr43 >> 4, r = nr43 & 7;
  return 524288 / (r || 0.5) / Math.pow(2, s + 1);
}

/* ------------------------------------------------------- PSG wave shaping */

const periodicWaves = {};            // cache key -> PeriodicWave

function squareWave(duty) {
  const key = "sq" + duty;
  if (!periodicWaves[key]) {
    const dutyFrac = [0.125, 0.25, 0.5, 0.75][duty];
    const N = 128;
    const real = new Float32Array(N), imag = new Float32Array(N);
    for (let n = 1; n < N; n++) {       // Fourier series of a +-1 pulse wave
      real[n] = (2 / (n * Math.PI)) * Math.sin(n * Math.PI * dutyFrac);
      imag[n] = (2 / (n * Math.PI)) * (1 - Math.cos(n * Math.PI * dutyFrac));
    }
    periodicWaves[key] = ctx.createPeriodicWave(real, imag,
                                                { disableNormalization: true });
  }
  return periodicWaves[key];
}

function gbWave(pattern) {
  const key = "w" + pattern.join(",");
  if (!periodicWaves[key]) {
    // DFT of the 32-step 4-bit wavetable -> band-limited PeriodicWave,
    // amplitude in the same +-1 units as the squares.
    const M = 32, H = 16;
    const real = new Float32Array(H + 1), imag = new Float32Array(H + 1);
    for (let n = 1; n <= H; n++) {
      let re = 0, im = 0;
      for (let k = 0; k < M; k++) {
        const v = (pattern[k] - 7.5) / 7.5;
        re += v * Math.cos(2 * Math.PI * n * k / M);
        im -= v * Math.sin(2 * Math.PI * n * k / M);
      }
      real[n] = re / M * 2;
      imag[n] = im / M * 2;
    }
    periodicWaves[key] = ctx.createPeriodicWave(real, imag,
                                                { disableNormalization: true });
  }
  return periodicWaves[key];
}

let noiseBufs = {};
function noiseBuffer(period7) {
  const key = period7 ? "n7" : "n15";
  if (!noiseBufs[key]) {
    const bits = period7 ? 127 : 32767;
    const data = new Float32Array(bits);
    let lfsr = period7 ? 0x7f : 0x7fff;
    for (let i = 0; i < bits; i++) {
      const bit = (lfsr ^ (lfsr >> 1)) & 1;
      lfsr = (lfsr >> 1) | (bit << (period7 ? 6 : 14));
      data[i] = (lfsr & 1) ? 1 : -1;
    }
    const buf = ctx.createBuffer(1, bits, 48000);
    buf.copyToChannel(data, 0);
    noiseBufs[key] = buf;
  }
  return noiseBufs[key];
}

// gCgb3Vol: the wave channel only has these volume fractions per 0..15 level.
function waveVolCode(level) {
  if (level <= 1) return 0;
  if (level <= 5) return 0.25;
  if (level <= 9) return 0.5;
  if (level <= 13) return 0.75;
  return 1;
}

/* ------------------------------------------------------------ output buses */

function makeBus(reverbAmt) {
  const master = ctx.createGain();
  master.gain.value = 0.45;                 // calibrated on offline renders
  master.connect(ctx.destination);

  const psg = ctx.createGain();            // PSG: hardware-mixed, dry
  psg.connect(master);

  const pcm = ctx.createGain();            // DirectSound mix, gets "reverb"
  pcm.connect(master);
  const nodes = [master, psg, pcm];
  if (reverbAmt > 0) {
    // m4a reverb: buffer re-fed ~7/6 frames old, mono, at reverb/128 with
    // feedback (SoundMainRAM_Reverb sums L+R of two adjacent old frames).
    const mono = ctx.createGain();
    mono.channelCount = 1;
    mono.channelCountMode = "explicit";
    mono.channelInterpretation = "speakers";
    const tapA = ctx.createDelay(0.3), tapB = ctx.createDelay(0.3);
    tapA.delayTime.value = 7 * 224 / 13379;
    tapB.delayTime.value = 6 * 224 / 13379;
    const g = ctx.createGain();
    g.gain.value = reverbAmt / 256;        // two taps -> reverb/128 total
    pcm.connect(mono);
    mono.connect(tapA); mono.connect(tapB);
    tapA.connect(g); tapB.connect(g);
    g.connect(master);
    g.connect(mono);                       // feedback
    nodes.push(mono, tapA, tapB, g);
  }
  return { master, psg, pcm, nodes };
}

/* ------------------------------------------------------------- envelopes */

/* Envelope shapers know the gate length upfront and schedule the complete
 * shape analytically - no automation cancellation, which misbehaves for
 * notes shorter than their attack. Both return the source stop time. */

function dsEnvelope(g, t0, tOff, peak, adsr, echoVol, echoLen) {
  // m4a DirectSound envelope, per-frame: A += a until 255; then *= d/256 down
  // to sustain s/256; release *= r/256 per frame.
  const [a, d, s, r] = adsr;
  const tA = a >= 255 ? t0 : t0 + (255 / Math.max(1, a)) * FRAME;
  const sus = peak * s / 256;
  let valOff;                                    // envelope value at gate end
  g.setValueAtTime(0, t0);
  if (tOff <= tA) {                              // gate ends mid-attack
    valOff = peak * (tOff - t0) / Math.max(tA - t0, 1e-6);
    g.linearRampToValueAtTime(valOff, tOff);
  } else {
    if (a >= 255) g.setValueAtTime(peak, t0);
    else g.linearRampToValueAtTime(peak, tA);
    if (d === 0) {
      g.setValueAtTime(sus, tA);
      valOff = sus;
    } else {
      const tau = FRAME / Math.log(256 / d);
      g.setTargetAtTime(sus, tA, tau);
      valOff = sus + (peak - sus) * Math.exp(-(tOff - tA) / tau);
    }
  }
  if (r === 0) { g.setValueAtTime(0, tOff); return tOff + 0.01; }
  const tau = FRAME / Math.log(256 / r);
  g.setTargetAtTime(0, tOff, tau);
  let stop = tOff + tau * 7;
  const echo = peak * echoVol / 256;
  if (echoVol > 0 && echoLen > 0 && echo < valOff) {   // pseudo-echo tail
    const tEcho = tOff + echoLen * FRAME;
    g.setValueAtTime(echo, tEcho);
    g.linearRampToValueAtTime(0, tEcho + 0.01);
    stop = Math.max(stop, tEcho + 0.02);
  }
  return stop;
}

function psgEnvelope(g, t0, tOff, adsr, goal) {
  // GB envelope in quantised levels: one +-1 level step per <phase> frames.
  // g ramps 0..1 (fraction of the note's volume goal).
  const [a, d, s, r] = adsr;
  const susLevel = goal > 0 ? ((goal * s + 15) >> 4) : 0;
  const sus = goal > 0 ? susLevel / goal : 0;
  const tA = t0 + a * goal * FRAME;              // a frames per level step
  const tD = tA + d * Math.max(0, goal - susLevel) * FRAME;
  let valOff;
  g.setValueAtTime(0, t0);
  if (a > 0 && tOff <= tA) {
    valOff = (tOff - t0) / (tA - t0);
    g.linearRampToValueAtTime(valOff, tOff);
  } else {
    if (a === 0) g.setValueAtTime(1, t0);
    else g.linearRampToValueAtTime(1, tA);
    if (d === 0) {
      g.setValueAtTime(sus, tA);
      valOff = sus;
    } else if (tOff <= tD) {
      valOff = 1 - (1 - sus) * (tOff - tA) / (tD - tA);
      g.linearRampToValueAtTime(valOff, tOff);
    } else {
      g.linearRampToValueAtTime(sus, tD);
      valOff = sus;
    }
  }
  if (r === 0) { g.setValueAtTime(0, tOff); return tOff + 0.01; }
  const tRel = r * Math.max(1, valOff * goal) * FRAME;
  g.setValueAtTime(valOff, tOff);
  g.linearRampToValueAtTime(0, tOff + tRel);
  return tOff + tRel + 0.01;
}

/* ------------------------------------------------------- volume/pan maths */

// ChnVolSetAsm: side volumes 0..255. rp = drum's own pan (0 melodic).
function sideVols(vel, vol, pan, rp) {
  const y = Math.max(-128, Math.min(127, 2 * pan));
  const volMR = ((y + 128) * 2 * vol) >> 8;      // track vol*volX>>5 = 2*vol
  const volML = ((127 - y) * 2 * vol) >> 8;
  return [
    Math.min(255, (vel * (127 - rp) * volML) >> 14),
    Math.min(255, (vel * (rp + 128) * volMR) >> 14),
  ];
}

function psgGains(vel, vol, pan, rp) {
  const [l, r] = sideVols(vel, vol, pan, rp);
  const goal = Math.min(15, (l + r) >> 4);
  const amp = goal / 15 * PSG_FULL;
  let gl = amp, gr = amp;                        // CgbPan: hard 3-way switch
  if (r >= l && (r >> 1) >= l) gl = 0;
  else if (l > r && (l >> 1) >= r) gr = 0;
  return { goal, gl, gr };
}

/* ------------------------------------------------------------- scheduling */

function playSong(song) {
  const bus = makeBus(song.reverb || 0);
  const t0 = ctx.currentTime + 0.15;
  const loopStart = song.loopStart, loopEnd = song.loopEnd;
  const looping = loopStart != null && loopEnd != null && loopEnd > loopStart;

  const tracks = song.tracks.map(tr => {
    // per-track vibrato LFO feeding every active note's detune AudioParam
    const lfo = ctx.createOscillator(); lfo.type = "triangle";
    lfo.frequency.value = 22 * 59.7275 / 256;
    const lfoGain = ctx.createGain(); lfoGain.gain.value = 0;
    lfo.connect(lfoGain); lfo.start();
    const hasLoopEvents = !looping ||
      tr.some(e => e[0] >= loopStart && e[0] < loopEnd);
    return {
      ev: tr, i: 0, pass: 0, done: false, hasLoopEvents,
      vol: 100, pan: 0, bend: 0, bendRange: 2, mod: 0,
      echoV: 0, echoL: 0,
      lfo, lfoGain,
      activeNotes: [],
    };
  });

  const passLen = looping ? loopEnd - loopStart : 0;
  const absTime = (tk, s) => tk.pass === 0
    ? t0 + s
    : t0 + loopEnd + (tk.pass - 1) * passLen + (s - loopStart);

  const bendCents = tk => tk.bend * tk.bendRange * 100 / 64;

  function noteGains(tk, v, vel) {
    const rp = v.pan !== undefined ? v.pan : 0;
    if (v.t === "pcm") {
      // vol automation is exact here: env carries vel, gains carry pan+drum
      // factors, and trackVol enters as vol/127 (matches >>14 within 1%).
      const y = Math.max(-128, Math.min(127, 2 * tk.pan));
      return {
        gl: ((127 - y) / 128) * ((127 - rp) / 128) * (tk.vol / 127),
        gr: ((y + 128) / 128) * ((rp + 128) / 128) * (tk.vol / 127),
        goal: 0,
      };
    }
    return psgGains(vel, tk.vol, tk.pan, rp);
  }

  function scheduleNote(tk, at, e) {
    const [, , key, vel, dur, vid] = e;
    const v = song.voices[vid];
    const playKey = v.rhythm ? v.base : key;
    const env = ctx.createGain();
    const gL = ctx.createGain(), gR = ctx.createGain();
    const merger = ctx.createChannelMerger(2);
    env.connect(gL); env.connect(gR);
    gL.connect(merger, 0, 0); gR.connect(merger, 0, 1);
    merger.connect(v.t === "pcm" ? bus.pcm : bus.psg);

    const { gl, gr, goal } = noteGains(tk, v, vel);
    gL.gain.setValueAtTime(gl, at);
    gR.gain.setValueAtTime(gr, at);

    const tOff = at + dur;
    let stopAt, srcs = [];

    if (v.t === "pcm") {
      const src = ctx.createBufferSource();
      src.buffer = sampleBuffer(v.sample);
      const s = samplesJson[v.sample];
      if (s.loop) {
        src.loop = true;
        src.loopStart = s.loopStart / s.rate;
        src.loopEnd = src.buffer.duration;
      }
      if (!v.fixed) {
        src.detune.setValueAtTime((playKey - 60) * 100 + bendCents(tk), at);
      }
      srcs = [src];
      stopAt = dsEnvelope(env.gain, at, tOff, vel / 127, v.adsr,
                          tk.echoV, tk.echoL);
    } else if (v.t === "sq" || v.t === "wave") {
      const o = ctx.createOscillator();
      let scale = 1;
      if (v.t === "sq") {
        o.setPeriodicWave(squareWave(v.duty));
        o.frequency.setValueAtTime(sqFreq(playKey), at);
      } else {
        o.setPeriodicWave(gbWave(v.pattern));
        // 32-step wave runs at half the square rate; Emerald's patterns hold
        // two cycles, so written pitch matches - table cycle = f/2.
        o.frequency.setValueAtTime(sqFreq(playKey) / 2, at);
        scale = waveVolCode(goal) * 15 / Math.max(goal, 1);
      }
      o.detune.setValueAtTime(bendCents(tk), at);
      if (scale !== 1) {
        gL.gain.setValueAtTime(gl * scale, at);
        gR.gain.setValueAtTime(gr * scale, at);
      }
      srcs = [o];
      stopAt = psgEnvelope(env.gain, at, tOff, v.adsr, goal);
    } else if (v.t === "noise") {
      const src = ctx.createBufferSource();
      src.buffer = noiseBuffer(v.period === 1);
      src.loop = true;
      src.playbackRate.setValueAtTime(noiseClock(playKey) / 48000, at);
      srcs = [src];
      stopAt = psgEnvelope(env.gain, at, tOff, v.adsr, goal);
    }

    srcs.forEach(s => s.connect(env));
    if (v.t !== "noise")
      srcs.forEach(s => { if (s.detune) tk.lfoGain.connect(s.detune); });

    stopAt += 0.05;
    srcs.forEach(s => { s.start(at); s.stop(stopAt); });
    const note = { srcs, env, gL, gR, merger, key: playKey, vel, v,
                   fixed: v.fixed, until: stopAt, t: v.t };
    tk.activeNotes.push(note);
    srcs[0].onended = () => {
      const i = tk.activeNotes.indexOf(note);
      if (i >= 0) tk.activeNotes.splice(i, 1);
      env.disconnect(); gL.disconnect(); gR.disconnect(); merger.disconnect();
    };
  }

  function updateActiveGains(tk, at) {
    for (const n of tk.activeNotes) {
      if (n.until <= at) continue;
      let { gl, gr, goal } = noteGains(tk, n.v, n.vel);
      if (n.t === "wave") {
        const sc = waveVolCode(goal) * 15 / Math.max(goal, 1);
        gl *= sc; gr *= sc;
      }
      n.gL.gain.setValueAtTime(gl, at);
      n.gR.gain.setValueAtTime(gr, at);
    }
  }

  function applyEvent(tk, at, e) {
    switch (e[1]) {
      case "n": scheduleNote(tk, at, e); break;
      case "v": tk.vol = e[2]; updateActiveGains(tk, at); break;
      case "p": tk.pan = e[2]; updateActiveGains(tk, at); break;
      case "b": {
        tk.bend = e[2];
        const c = bendCents(tk);
        for (const n of tk.activeNotes) {
          if (n.until <= at) continue;
          for (const s of n.srcs)
            if (s.detune && !n.fixed)
              s.detune.setValueAtTime((n.t === "pcm" ? (n.key - 60) * 100 : 0) + c, at);
        }
        break;
      }
      case "br": tk.bendRange = e[2]; break;
      case "m":
        tk.mod = e[2];
        // pitM += 16*modM, 256 units/semitone, LFO triangle peak = mod
        tk.lfoGain.gain.setValueAtTime(e[2] * 16 * 100 / 256, at);
        break;
      case "ls":
        tk.lfo.frequency.setValueAtTime(e[2] * 59.7275 / 256, at);
        break;
      case "ev": tk.echoV = e[2]; break;
      case "el": tk.echoL = e[2]; break;
    }
  }

  const AHEAD = 1.2;
  let timer = null, dead = false;

  function pump() {
    const horizon = ctx.currentTime + AHEAD;
    let allDone = true;
    for (const tk of tracks) {
      if (tk.done) continue;
      for (;;) {
        if (tk.i >= tk.ev.length) {
          if (looping && tk.hasLoopEvents) { tk.i = 0; tk.pass++; continue; }
          tk.done = true;
          break;
        }
        const e = tk.ev[tk.i];
        const s = e[0];
        if (looping) {
          if (tk.pass > 0 && s < loopStart) { tk.i++; continue; }
          if (s >= loopEnd) { tk.i = tk.ev.length; continue; }
        }
        const at = absTime(tk, s);
        if (at > horizon) { allDone = false; break; }
        applyEvent(tk, at, e);
        tk.i++;
        allDone = false;
      }
      if (looping && !tk.done) allDone = false;
    }
    if (allDone && ctx.currentTime > t0) stop(3);
  }

  function stop(fadeout) {
    if (dead) return;
    dead = true;
    clearInterval(timer);
    const now = ctx.currentTime;
    bus.master.gain.setValueAtTime(bus.master.gain.value, now);
    bus.master.gain.linearRampToValueAtTime(0, now + (fadeout || 0.1));
    setTimeout(() => {
      tracks.forEach(tk => { try { tk.lfo.stop(); } catch (e) {} });
      bus.nodes.forEach(n => { try { n.disconnect(); } catch (e) {} });
    }, (fadeout || 0.1) * 1000 + 100);
    if (current === self) current = null;
  }

  const self = { stop };
  timer = setInterval(pump, 250);
  pump();
  return self;
}

/* --------------------------------------------------------------------- UI */

async function main() {
  const list = document.getElementById("songs");
  const [manifest, samples] = await Promise.all([
    fetchJson("manifest.json"), fetchJson("samples.json"),
  ]);
  samplesJson = samples;

  const rows = [];
  for (const entry of manifest) {
    const row = document.createElement("div");
    row.className = "song";
    row.innerHTML = `<span class="title">${entry.title}</span>
      <span class="file">${entry.name}</span>
      <span class="btns"><button>play</button></span>`;
    list.appendChild(row);
    rows.push(row);
    const btn = row.querySelector("button");
    btn.onclick = async () => {
      ensureCtx();
      const was = btn.classList.contains("playing");
      if (current) current.stop(0.25);
      rows.forEach(r => r.querySelector("button").classList.remove("playing"));
      if (was) return;
      if (!songCache[entry.file]) songCache[entry.file] = await fetchJson(entry.file);
      btn.classList.add("playing");
      current = playSong(songCache[entry.file]);
    };
  }
  document.getElementById("stop").onclick = () => {
    if (current) current.stop(0.3);
    document.querySelectorAll("button.playing")
      .forEach(b => b.classList.remove("playing"));
  };
}

main();
