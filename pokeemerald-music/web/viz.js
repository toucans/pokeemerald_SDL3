/* viz.js — live visualization of what the m4a engine is playing.
 *
 * The worklet posts one snapshot per GBA frame (59.7 Hz): for every live
 * voice its vid, *sounding* pitch (so vibrato and pitch bends draw as real
 * wobbles), envelope level and stereo pan. This module keeps a few seconds
 * of history and draws a scrolling waterfall on a 16:9 canvas:
 *
 *   - one hue per instrument (PSG channels get fixed neon hues: squares
 *     cyan, wavetable violet, noise a gray speckle band)
 *   - stroke thickness + brightness = envelope level, so attacks bloom and
 *     pseudo-echo tails read as long dim streaks that cut off
 *   - each stroke is split horizontally: upper half brightness = right
 *     channel, lower half = left, so panning reads at a glance
 *   - PCM strokes leave a faint ghost 117 ms behind the note — the engine's
 *     real "reverb" (a mono two-tap feedback echo), drawn at its real delay
 *   - vertical dashed line each time the sequencer wraps the []-loop
 *
 * Drawing is latency-compensated: snapshots arrive when *generated*, audio
 * plays outputLatency later, so the now-line trails by that much.
 */
"use strict";

const M4AViz = (() => {
  const WINDOW = 9;          // seconds of history on screen
  const NOWX = 0.86;         // now-line position (fraction of width)
  const REVERB_S = 7 * 224 / 13379;   // the engine's echo delay (~117 ms)

  let canvas = null, ctx2d = null, wrap = null;
  let running = false, raf = 0;
  let frames = [];           // [{t, v:[vid,pitch,level,pan,...]}]
  let loopMarks = [];        // song-times where the loop wrapped
  let song = null, title = "";
  let palette = {};          // vid -> {h,s,l, name, kind}
  let pitchLo = 36, pitchHi = 96;
  let audioLatency = 0.1;
  let dispNow = 0, lastRaf = 0;
  let active = new Map();    // vid -> last-seen song time (legend)

  function shortName(v) {
    if (v.t === "sq") return "square " + ["12.5%", "25%", "50%", "75%"][v.duty];
    if (v.t === "wave") return "wavetable";
    if (v.t === "noise") return "noise";
    return v.sample.replace(/^DirectSoundWaveData_/, "")
      .replace(/^(sc88pro_|sd90_|trinity_|unknown_|unused_)/, "")
      .replace(/_/g, " ");
  }

  function buildPalette(s) {
    palette = {};
    // stable, well-spread hues for PCM instruments; fixed neons for PSG
    const pcm = Object.keys(s.voices).filter((k) => s.voices[k].t === "pcm");
    pcm.forEach((vid, i) => {
      const h = (i * 360 / Math.max(pcm.length, 1) + 20) % 360;
      // steer away from the PSG cyan/violet bands so classes stay legible
      const hue = (h > 170 && h < 300) ? (h + 130) % 360 : h;
      palette[vid] = { h: hue, s: 85, l: 62, kind: "pcm", name: shortName(s.voices[vid]) };
    });
    for (const [vid, v] of Object.entries(s.voices)) {
      if (v.t === "sq") palette[vid] = { h: 187, s: 95, l: 60, kind: "sq", name: shortName(v) };
      else if (v.t === "wave") palette[vid] = { h: 268, s: 90, l: 66, kind: "wave", name: shortName(v) };
      else if (v.t === "noise") palette[vid] = { h: 0, s: 0, l: 78, kind: "noise", name: shortName(v) };
    }
    // pitch range from the actual notes, padded
    let lo = 127, hi = 0;
    for (const tr of s.tracks) for (const e of tr) {
      if (e[1] !== "n") continue;
      const v = s.voices[e[5]];
      const k = v && v.rhythm ? v.base : e[2];
      if (k < lo) lo = k;
      if (k > hi) hi = k;
    }
    pitchLo = Math.min(lo - 3, 48); pitchHi = Math.max(hi + 3, 84);
  }

  function start(opts) {
    song = opts.song; title = opts.title || song.name;
    frames = []; loopMarks = []; active.clear();
    buildPalette(song);
    dispNow = 0; lastRaf = 0;
    if (opts.actx) {
      audioLatency = (opts.actx.outputLatency || 0) + (opts.actx.baseLatency || 0) || 0.1;
    }
    if (wrap) wrap.hidden = false;
    if (!running) { running = true; raf = requestAnimationFrame(draw); }
  }

  function stop() {
    running = false;
    cancelAnimationFrame(raf);
    if (wrap) wrap.hidden = true;
    frames = []; song = null;
  }

  function frame(msg) {
    if (!running) return;
    frames.push(msg);
    if (msg.wrapped) loopMarks.push(msg.t);
    const cut = msg.t - WINDOW - 1;
    while (frames.length && frames[0].t < cut) frames.shift();
    while (loopMarks.length && loopMarks[0] < cut) loopMarks.shift();
    for (let i = 0; i < msg.v.length; i += 4) active.set(msg.v[i], msg.t);
  }

  const yOf = (pitch, H) =>
    H * (1 - (pitch - pitchLo) / (pitchHi - pitchLo)) * 0.92 + H * 0.02;

  function draw(ts) {
    if (!running) return;
    raf = requestAnimationFrame(draw);
    if (!canvas || !frames.length) return;

    const W = canvas.width, H = canvas.height;
    const g = ctx2d;
    const pxPerSec = (W * NOWX) / WINDOW;

    // smooth clock: follow the newest snapshot minus the audio latency
    const target = frames[frames.length - 1].t - audioLatency;
    if (!lastRaf) { dispNow = target; lastRaf = ts; }
    dispNow += (ts - lastRaf) / 1000;
    lastRaf = ts;
    dispNow += (target - dispNow) * 0.06;            // gentle resync

    // background
    const bg = g.createLinearGradient(0, 0, 0, H);
    bg.addColorStop(0, "#0c1017"); bg.addColorStop(1, "#10141a");
    g.globalCompositeOperation = "source-over";
    g.fillStyle = bg; g.fillRect(0, 0, W, H);

    // octave grid
    g.strokeStyle = "rgba(120,150,190,.08)"; g.lineWidth = 1;
    g.fillStyle = "rgba(120,150,190,.35)"; g.font = `${Math.round(H / 48)}px ui-monospace, monospace`;
    for (let k = Math.ceil(pitchLo / 12) * 12; k <= pitchHi; k += 12) {
      const y = yOf(k, H);
      g.beginPath(); g.moveTo(0, y); g.lineTo(W, y); g.stroke();
      g.fillText("C" + (k / 12 - 1), 8, y - 4);
    }

    // loop wrap markers
    for (const t of loopMarks) {
      const x = W * NOWX - (dispNow - t) * pxPerSec;
      if (x < 0) continue;
      g.strokeStyle = "rgba(224,180,92,.5)"; g.setLineDash([6, 6]);
      g.beginPath(); g.moveTo(x, 0); g.lineTo(x, H); g.stroke();
      g.setLineDash([]);
      g.fillStyle = "rgba(224,180,92,.8)";
      g.fillText("loop", x + 6, H * 0.05);
    }

    // voice strokes (additive glow)
    g.globalCompositeOperation = "lighter";
    const revAlpha = song && song.reverb ? song.reverb / 128 * 0.35 : 0;
    for (let i = 1; i < frames.length; i++) {
      const f = frames[i];
      const dt = dispNow - f.t;
      if (dt > WINDOW || dt < -0.2) continue;
      const x = W * NOWX - dt * pxPerSec;
      const dx = Math.max(1.5, (f.t - frames[i - 1].t) * pxPerSec + 0.75);
      for (let j = 0; j < f.v.length; j += 4) {
        const vid = f.v[j], pitch = f.v[j + 1], level = f.v[j + 2], pan = f.v[j + 3];
        const p = palette[vid];
        if (!p || level <= 0.001) continue;
        const y = yOf(pitch, H);
        const lv = Math.sqrt(level);                  // perceptual-ish
        const hh = (H / 90) * (0.6 + lv * 2.4);       // half-height
        const jitter = p.kind === "noise" ? ((f.t * 631 + pitch) % 1 - 0.5) * hh : 0;
        const aTop = lv * (0.5 + pan * 0.45);
        const aBot = lv * (0.5 - pan * 0.45);
        // soft halo
        g.fillStyle = `hsla(${p.h},${p.s}%,${p.l}%,${lv * 0.10})`;
        g.fillRect(x - dx, y - hh * 3 + jitter, dx, hh * 6);
        // core, split top (R) / bottom (L)
        g.fillStyle = `hsla(${p.h},${p.s}%,${p.l}%,${aTop})`;
        g.fillRect(x - dx, y - hh + jitter, dx, hh);
        g.fillStyle = `hsla(${p.h},${p.s}%,${p.l}%,${aBot})`;
        g.fillRect(x - dx, y + jitter, dx, hh);
        // the engine's PCM echo, at its true 117 ms delay
        if (revAlpha && p.kind === "pcm") {
          g.fillStyle = `hsla(${p.h},${p.s * 0.7}%,${p.l}%,${lv * revAlpha})`;
          g.fillRect(x - dx - REVERB_S * pxPerSec, y - hh * 0.6 + jitter, dx, hh * 1.2);
        }
      }
    }

    // now-line
    g.globalCompositeOperation = "source-over";
    g.strokeStyle = "rgba(215,224,234,.25)"; g.lineWidth = 1;
    g.beginPath(); g.moveTo(W * NOWX, 0); g.lineTo(W * NOWX, H); g.stroke();

    // header: title, mode, clock
    const fs = Math.round(H / 26);
    g.font = `600 ${fs}px system-ui, sans-serif`;
    g.fillStyle = "rgba(215,224,234,.92)";
    g.fillText(title, W * 0.015, H * 0.075);
    g.font = `${Math.round(fs * 0.62)}px ui-monospace, monospace`;
    g.fillStyle = "rgba(107,122,140,.9)";
    g.fillText("m4a engine @ 59.7275 Hz", W * 0.015, H * 0.075 + fs * 0.85);
    const mm = Math.floor(dispNow / 60), ss = ("0" + Math.floor(dispNow % 60)).slice(-2);
    g.textAlign = "right";
    g.fillText(`${mm}:${ss}`, W * 0.985, H * 0.06);
    g.textAlign = "left";

    // legend: instruments heard in the last couple of seconds
    const now = frames[frames.length - 1].t;
    let lx = W * 0.015;
    const ly = H * 0.965;
    g.font = `${Math.round(fs * 0.58)}px ui-monospace, monospace`;
    for (const [vid, seen] of active) {
      if (now - seen > 2.5) continue;
      const p = palette[vid];
      if (!p) continue;
      const tw = g.measureText(p.name).width;
      if (lx + tw + fs > W * 0.99) break;
      g.fillStyle = `hsl(${p.h},${p.s}%,${p.l}%)`;
      g.fillRect(lx, ly - fs * 0.45, fs * 0.4, fs * 0.4);
      g.fillStyle = "rgba(215,224,234,.75)";
      g.fillText(p.name, lx + fs * 0.55, ly);
      lx += tw + fs * 1.3;
    }
  }

  function attach(el) {
    wrap = el;
    canvas = el.querySelector("canvas");
    ctx2d = canvas.getContext("2d");
    const fit = () => {
      const w = el.clientWidth || 680;
      const dpr = Math.min(window.devicePixelRatio || 1, 2);
      canvas.width = Math.round(w * dpr);
      canvas.height = Math.round(w * 9 / 16 * dpr);
      canvas.style.height = Math.round(w * 9 / 16) + "px";
    };
    fit();
    new ResizeObserver(fit).observe(el);
    const fs = el.querySelector("#viz-fs");
    if (fs) fs.onclick = () => {
      if (document.fullscreenElement) document.exitFullscreen();
      else el.requestFullscreen && el.requestFullscreen();
    };
  }

  return { attach, start, stop, frame };
})();
