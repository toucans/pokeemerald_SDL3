/* WebAudio player for pokeemerald overworld BGM.
 *
 * Plays the JSON extracted by extract.py, imitating the GBA m4a (MP2K) engine:
 * DirectSound PCM voices with the engine's per-frame ADSR, GB PSG squares /
 * programmable wave / noise, track volume+pan automation, pitch bends,
 * mod-wheel vibrato, loop markers and pseudo-echo.
 *
 * Output is band-limited to the GBA's 13.379 kHz DAC (lowpass ~6.6 kHz) with
 * the engine's feedback-echo "reverb" - i.e. it aims to sound like the GBA.
 */
"use strict";

const FRAME = 1 / 59.7275;          // GBA VBlank; m4a envelopes tick per frame
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

/* ------------------------------------------------------- PSG wave shaping */

const periodicWaves = {};            // cache key -> PeriodicWave

function squareWave(duty) {
  const key = "sq" + duty;
  if (!periodicWaves[key]) {
    const dutyFrac = [0.125, 0.25, 0.5, 0.75][duty];
    const N = 64;
    const real = new Float32Array(N), imag = new Float32Array(N);
    for (let n = 1; n < N; n++) {       // Fourier series of a pulse wave
      real[n] = (2 / (n * Math.PI)) * Math.sin(n * Math.PI * dutyFrac);
      imag[n] = (2 / (n * Math.PI)) * (1 - Math.cos(n * Math.PI * dutyFrac));
    }
    periodicWaves[key] = ctx.createPeriodicWave(real, imag);
  }
  return periodicWaves[key];
}

function gbWave(pattern) {
  const key = "w" + pattern.join("");
  if (!periodicWaves[key]) {
    // DFT of the 32-step 4-bit wavetable -> band-limited PeriodicWave
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
    periodicWaves[key] = ctx.createPeriodicWave(real, imag);
  }
  return periodicWaves[key];
}

let noiseBufs = {};
function noiseBuffer(period7) {
  const key = period7 ? "n7" : "n15";
  if (!noiseBufs[key]) {
    const bits = period7 ? 127 : 32767;
    const data = new Float32Array(bits);
    let lfsr = 0x7fff;
    for (let i = 0; i < bits; i++) {
      const bit = (lfsr ^ (lfsr >> 1)) & 1;
      lfsr = (lfsr >> 1) | (bit << (period7 ? 6 : 14));
      if (period7) lfsr &= 0x7f;
      data[i] = (lfsr & 1) ? 0.6 : -0.6;
    }
    const buf = ctx.createBuffer(1, bits, 48000);
    buf.copyToChannel(data, 0);
    noiseBufs[key] = buf;
  }
  return noiseBufs[key];
}

/* ------------------------------------------------------------- output bus */

function makeBus(reverbAmt) {
  const input = ctx.createGain();
  const master = ctx.createGain();
  // GBA output path: ~13.4 kHz DAC -> keep only what fits below its Nyquist
  const lp = ctx.createBiquadFilter();
  lp.type = "lowpass"; lp.frequency.value = 6600; lp.Q.value = 0.55;
  // m4a "reverb": the mixer feeds back its own DMA buffer from a few frames
  // ago at intensity/128 - i.e. a short mushy feedback echo.
  const delay = ctx.createDelay(0.3);
  delay.delayTime.value = 5 * FRAME;
  const fb = ctx.createGain(); fb.gain.value = reverbAmt / 128 * 0.75;
  const wet = ctx.createGain(); wet.gain.value = reverbAmt / 128 * 0.9;
  input.connect(lp);
  lp.connect(master);
  lp.connect(delay);
  delay.connect(fb); fb.connect(delay);
  delay.connect(wet); wet.connect(master);
  master.gain.value = 0.5;
  master.connect(ctx.destination);
  return { input, master, nodes: [input, master, lp, delay, fb, wet] };
}

/* ------------------------------------------------------------- envelopes */

/* Both envelope shapers know the gate length upfront, so they schedule the
 * complete shape (attack/decay/sustain/release) analytically - no automation
 * cancellation, which misbehaves for notes shorter than their attack. */

function dsEnvelope(g, t0, tOff, peak, adsr, echoVol, echoLen) {
  // m4a DirectSound envelope, per-frame: A += a until 255; then *= d/256 down
  // to sustain s/256; release *= r/256 per frame. Returns source stop time.
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

function psgEnvelope(g, t0, tOff, peak, adsr) {
  // GB envelope: 4-bit volume, one step per <phase> frames, linear steps.
  const [a, d, s, r] = adsr;
  const tA = t0 + a * 15 * FRAME;
  const sus = peak * s / 15;
  const tD = tA + d * (15 - s) * FRAME;
  let valOff;
  g.setValueAtTime(0, t0);
  if (a > 0 && tOff <= tA) {
    valOff = peak * (tOff - t0) / (tA - t0);
    g.linearRampToValueAtTime(valOff, tOff);
  } else {
    if (a === 0) g.setValueAtTime(peak, t0);
    else g.linearRampToValueAtTime(peak, tA);
    if (d === 0) {
      g.setValueAtTime(sus, tA);
      valOff = sus;
    } else if (tOff <= tD) {
      valOff = peak - (peak - sus) * (tOff - tA) / (tD - tA);
      g.linearRampToValueAtTime(valOff, tOff);
    } else {
      g.linearRampToValueAtTime(sus, tD);
      valOff = sus;
    }
  }
  if (r === 0) { g.setValueAtTime(0, tOff); return tOff + 0.01; }
  const tRel = r * 15 * FRAME * (valOff / Math.max(peak, 1e-6));
  g.setValueAtTime(valOff, tOff);
  g.linearRampToValueAtTime(0, tOff + Math.max(tRel, 0.005));
  return tOff + Math.max(tRel, 0.005) + 0.01;
}

/* ------------------------------------------------------------- scheduling */

function playSong(song) {
  const bus = makeBus(song.reverb || 0);
  const t0 = ctx.currentTime + 0.15;
  const loopStart = song.loopStart, loopEnd = song.loopEnd;
  const looping = loopStart != null && loopEnd != null && loopEnd > loopStart;

  const tracks = song.tracks.map(tr => {
    const gain = ctx.createGain(); gain.gain.value = 100 / 127;
    gain.connect(bus.input);
    // per-track vibrato LFO feeding every active note's detune AudioParam
    const lfo = ctx.createOscillator(); lfo.type = "sine";
    lfo.frequency.value = 22 * 59.7275 / 256;
    const lfoGain = ctx.createGain(); lfoGain.gain.value = 0;
    lfo.connect(lfoGain); lfo.start();
    const hasLoopEvents = !looping ||
      tr.some(e => e[0] >= loopStart && e[0] < loopEnd);
    return {
      ev: tr, i: 0, pass: 0, done: false, hasLoopEvents,
      vol: 100, pan: 0, bend: 0, bendRange: 2, mod: 0,
      echoV: 0, echoL: 0,
      gain, lfo, lfoGain,
      activeNotes: [],   // {srcs, panner, key, until}
    };
  });

  const passLen = looping ? loopEnd - loopStart : 0;
  const absTime = (tk, s) => tk.pass === 0
    ? t0 + s
    : t0 + loopEnd + (tk.pass - 1) * passLen + (s - loopStart);

  const bendCents = tk => tk.bend * tk.bendRange * 100 / 64;

  function scheduleNote(tk, at, e) {
    const [, , key, vel, dur, vid] = e;
    const v = song.voices[vid];
    const playKey = v.rhythm ? v.base : key;
    const peak = vel / 127;
    const env = ctx.createGain();
    const panner = ctx.createStereoPanner();
    const panVal = (v.pan !== undefined) ? v.pan / 128 : tk.pan / 64;
    panner.pan.setValueAtTime(Math.max(-1, Math.min(1, panVal)), at);
    env.connect(panner); panner.connect(tk.gain);

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
      stopAt = dsEnvelope(env.gain, at, tOff, peak, v.adsr, tk.echoV, tk.echoL);
    } else if (v.t === "sq" || v.t === "wave") {
      const o = ctx.createOscillator();
      o.setPeriodicWave(v.t === "sq" ? squareWave(v.duty) : gbWave(v.pattern));
      o.frequency.setValueAtTime(440 * Math.pow(2, (playKey - 69) / 12), at);
      o.detune.setValueAtTime(bendCents(tk), at);
      srcs = [o];
      stopAt = psgEnvelope(env.gain, at, tOff, peak * 0.5, v.adsr);
    } else if (v.t === "noise") {
      const src = ctx.createBufferSource();
      src.buffer = noiseBuffer(v.period === 1);
      src.loop = true;
      src.playbackRate.setValueAtTime(Math.pow(2, (playKey - 60) / 12), at);
      srcs = [src];
      stopAt = psgEnvelope(env.gain, at, tOff, peak * 0.4, v.adsr);
    }

    srcs.forEach(s => s.connect(env));
    // vibrato -> detune of every source that has one
    srcs.forEach(s => { if (s.detune) tk.lfoGain.connect(s.detune); });

    stopAt += 0.05;
    srcs.forEach(s => { s.start(at); s.stop(stopAt); });
    const note = { srcs, env, panner, key: playKey,
                   fixed: v.fixed, until: stopAt, t: v.t, ownPan: v.pan !== undefined };
    tk.activeNotes.push(note);
    srcs[0].onended = () => {
      const i = tk.activeNotes.indexOf(note);
      if (i >= 0) tk.activeNotes.splice(i, 1);
      env.disconnect(); panner.disconnect();
    };
  }

  function applyEvent(tk, at, e) {
    switch (e[1]) {
      case "n": scheduleNote(tk, at, e); break;
      case "v":
        tk.vol = e[2];
        tk.gain.gain.setValueAtTime(e[2] / 127, at);
        break;
      case "p":
        tk.pan = e[2];
        for (const n of tk.activeNotes)
          if (!n.ownPan && n.until > at)
            n.panner.pan.setValueAtTime(Math.max(-1, Math.min(1, e[2] / 64)), at);
        break;
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
        tk.lfoGain.gain.setValueAtTime(e[2] * 100 * 4 / 256, at);
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
