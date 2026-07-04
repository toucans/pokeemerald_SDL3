/* player.js — main-thread shim for the m4a AudioWorklet.
 *
 * Loads the extracted data (manifest + samples + song JSON), decodes the base64
 * sample bank once, and drives a single AudioWorkletNode running m4a-worklet.js.
 * All synthesis happens in the worklet; this file is just UI + data plumbing.
 *
 * No build step, no deps — vanilla, served static. See WORKLET_HANDOFF.md.
 */
"use strict";

const DATA = "data/";

let ctx = null;
let node = null;          // the AudioWorkletNode running the engine
let bankSent = false;
let bank = null;          // {label:{rate,loop,loopStart,scale,data:TypedArray}}
let samplesJson = null;
let sf2Loaded = null;     // promise: sf2 bank sent + overlays parsed (lazy, ~24 MB)
let sf2Overlays = null;   // {songName: {voices, map}}
const songCache = {};     // per (file + mode) prepared song objects
let playingBtn = null;

function setStatus(msg) {
  const el = document.getElementById("status");
  if (el) el.textContent = msg || "";
}

async function fetchJson(p) { return (await fetch(DATA + p)).json(); }

function b64ToInt8(b64) {
  const raw = atob(b64);
  const out = new Int8Array(raw.length);
  for (let i = 0; i < raw.length; i++) out[i] = raw.charCodeAt(i) << 24 >> 24;
  return out;
}

function b64ToInt16(b64) {
  const raw = atob(b64);
  const bytes = new Uint8Array(raw.length);
  for (let i = 0; i < raw.length; i++) bytes[i] = raw.charCodeAt(i);
  return new Int16Array(bytes.buffer);   // little-endian, as written by extract_sf2.py
}

function decodeBank() {
  bank = {};
  for (const [label, v] of Object.entries(samplesJson)) {
    bank[label] = {
      rate: v.rate, loop: v.loop, loopStart: v.loopStart,
      scale: 1 / 128,                    // 8-bit signed PCM -> [-1, 1)
      data: b64ToInt8(v.b64),
    };
  }
}

async function ensureAudio() {
  if (!ctx) {
    // 'playback' asks for a large output buffer: this is a music player, not
    // an instrument, so trade untimeable latency for underrun-proof delivery.
    ctx = new (window.AudioContext || window.webkitAudioContext)({ latencyHint: "playback" });
    await ctx.audioWorklet.addModule("m4a-worklet.js");
    node = new AudioWorkletNode(ctx, "m4a-processor", { outputChannelCount: [2] });
    node.connect(ctx.destination);
    node.port.onmessage = (e) => {
      if (e.data && e.data.type === "ended") clearPlaying();
    };
  }
  if (ctx.state === "suspended") await ctx.resume();
  if (!bankSent) {
    if (!bank) decodeBank();
    // transfer the sample ArrayBuffers to the audio thread (one-time)
    const transfer = Object.values(bank).map((s) => s.data.buffer);
    node.port.postMessage({ type: "bank", bank }, transfer);
    bankSent = true;
    bank = null;             // buffers are now owned by the worklet
  }
}

// Fetch + send the soundfont bank once, on first sf2 play (it's ~24 MB, so
// the original mode never pays for it).
function ensureSf2() {
  if (!sf2Loaded) {
    sf2Loaded = (async () => {
      const samples = await fetchJson("sf2/samples.json");
      const sfBank = {};
      for (const [label, v] of Object.entries(samples)) {
        sfBank[label] = {
          rate: v.rate, loop: v.loop, loopStart: v.loopStart,
          scale: 1 / 32768,              // 16-bit signed PCM -> [-1, 1)
          data: b64ToInt16(v.b64),
        };
      }
      const transfer = Object.values(sfBank).map((s) => s.data.buffer);
      node.port.postMessage({ type: "bank", bank: sfBank }, transfer);
    })();
  }
  return sf2Loaded;
}

// The sf2 variant of a song: same events, same envelopes — only the PCM
// voices are remapped to the overlay's soundfont voices (per key range).
function sf2Song(song) {
  const ov = sf2Overlays[song.name];
  if (!ov) return song;
  const voices = Object.assign({}, song.voices, ov.voices);
  const tracks = song.tracks.map((ev) => ev.map((e) => {
    if (e[1] !== "n") return e;
    const zones = ov.map[e[5]];
    if (!zones) return e;
    for (const [lo, hi, sid] of zones) {
      if (e[2] >= lo && e[2] <= hi) {
        const c = e.slice(); c[5] = sid; return c;
      }
    }
    return e;
  }));
  return Object.assign({}, song, { voices, tracks });
}

function clearPlaying() {
  document.querySelectorAll("button.playing").forEach((b) => b.classList.remove("playing"));
  playingBtn = null;
}

async function main() {
  const list = document.getElementById("songs");
  const [manifest, samples, overlays] = await Promise.all([
    fetchJson("manifest.json"), fetchJson("samples.json"),
    fetchJson("sf2/overlays.json").catch(() => ({})),   // small; the 24 MB bank stays lazy
  ]);
  samplesJson = samples;
  sf2Overlays = overlays;

  const play = async (btn, entry, mode) => {
    try {
      const wasPlaying = btn === playingBtn;
      await ensureAudio();
      node.port.postMessage({ type: "stop" });
      clearPlaying();
      if (wasPlaying) return;
      if (mode === "sf2") {
        setStatus("loading soundfont…");
        await ensureSf2();
        setStatus("");
      }
      if (!songCache[entry.file]) songCache[entry.file] = await fetchJson(entry.file);
      const key = entry.file + ":" + mode;
      if (!songCache[key]) {
        songCache[key] = mode === "sf2" ? sf2Song(songCache[entry.file])
                                        : songCache[entry.file];
      }
      node.port.postMessage({ type: "play", song: songCache[key] });
      btn.classList.add("playing");
      playingBtn = btn;
      setStatus("");
    } catch (err) {
      console.error(err);
      // AudioWorklet needs a secure context (https/localhost) — most likely cause here.
      setStatus("audio error: " + (err && err.message ? err.message : err));
    }
  };

  const rows = [];
  for (const entry of manifest) {
    const row = document.createElement("div");
    row.className = "song";
    const hasSf2 = !!sf2Overlays[entry.name];
    row.innerHTML = `<span class="title">${entry.title}</span>
      <span class="file">${entry.name}</span>
      <span class="btns"><button class="gba">play</button>${hasSf2
        ? '<button class="sf2" title="Roland/Charm soundfont samples instead of the GBA\'s — same sequence, same engine">sf2</button>' : ""}</span>`;
    list.appendChild(row);
    const gbaBtn = row.querySelector("button.gba");
    gbaBtn.onclick = () => play(gbaBtn, entry, "gba");
    const sf2Btn = row.querySelector("button.sf2");
    if (sf2Btn) sf2Btn.onclick = () => play(sf2Btn, entry, "sf2");
    rows.push({ row, text: (entry.title + " " + entry.name).toLowerCase() });
  }

  const search = document.getElementById("search");
  if (search) {
    search.oninput = () => {
      const q = search.value.trim().toLowerCase();
      for (const r of rows) r.row.style.display = !q || r.text.includes(q) ? "" : "none";
    };
  }

  document.getElementById("stop").onclick = () => {
    if (node) node.port.postMessage({ type: "stop" });
    clearPlaying();
  };
}

main();
