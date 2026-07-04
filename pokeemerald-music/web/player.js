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
let bank = null;          // {label:{rate,loop,loopStart,scale,data:Int8Array}}
let samplesJson = null;
const songCache = {};
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
      if (!e.data) return;
      if (e.data.type === "ended") { clearPlaying(); M4AViz.stop(); }
      else if (e.data.type === "viz") M4AViz.frame(e.data);
    };
    node.port.postMessage({ type: "viz", on: true });
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

function clearPlaying() {
  document.querySelectorAll("button.playing").forEach((b) => b.classList.remove("playing"));
  playingBtn = null;
}

async function main() {
  const list = document.getElementById("songs");
  const [manifest, samples] = await Promise.all([
    fetchJson("manifest.json"), fetchJson("samples.json"),
  ]);
  samplesJson = samples;

  const play = async (btn, entry) => {
    try {
      const wasPlaying = btn === playingBtn;
      await ensureAudio();
      node.port.postMessage({ type: "stop" });
      clearPlaying();
      if (wasPlaying) { M4AViz.stop(); return; }
      if (!songCache[entry.file]) songCache[entry.file] = await fetchJson(entry.file);
      node.port.postMessage({ type: "play", song: songCache[entry.file] });
      M4AViz.start({ song: songCache[entry.file], title: entry.title, actx: ctx });
      btn.classList.add("playing");
      playingBtn = btn;
      setStatus("");
    } catch (err) {
      console.error(err);
      // AudioWorklet needs a secure context (https/localhost) — most likely cause here.
      setStatus("audio error: " + (err && err.message ? err.message : err));
    }
  };

  // one section per category, in manifest (= category) order
  const sections = [];
  const rows = [];
  let cat = null, section = null;
  for (const entry of manifest) {
    if (entry.cat !== cat) {
      cat = entry.cat;
      const head = document.createElement("div");
      head.className = "cat";
      head.textContent = cat || "Other";
      list.appendChild(head);
      section = { head, rows: [] };
      sections.push(section);
    }
    const row = document.createElement("div");
    row.className = "song";
    row.innerHTML = `<span class="title">${entry.title}</span>
      <span class="file">${entry.name}</span>
      <span class="btns"><button>play</button></span>`;
    list.appendChild(row);
    const btn = row.querySelector("button");
    btn.onclick = () => play(btn, entry);
    const r = { row, text: (entry.title + " " + entry.name).toLowerCase() };
    rows.push(r);
    section.rows.push(r);
  }

  const search = document.getElementById("search");
  if (search) {
    search.oninput = () => {
      const q = search.value.trim().toLowerCase();
      for (const r of rows) r.row.style.display = !q || r.text.includes(q) ? "" : "none";
      for (const s of sections) {
        s.head.style.display = s.rows.some((r) => r.row.style.display !== "none") ? "" : "none";
      }
    };
  }

  document.getElementById("stop").onclick = () => {
    if (node) node.port.postMessage({ type: "stop" });
    clearPlaying();
    M4AViz.stop();
  };
}

M4AViz.attach(document.getElementById("viz-wrap"));
main();
