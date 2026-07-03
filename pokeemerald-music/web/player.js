/* player.js — main-thread shim for the m4a engine (ES module).
 *
 * Loads the extracted data (manifest + samples + song JSON), decodes the base64
 * sample bank once, and drives the shared M4AEngine via one of two audio paths:
 *   - AudioWorklet, when the page is a secure context (https/localhost) — the
 *     engine runs off the main thread in m4a-worklet.js.
 *   - ScriptProcessorNode fallback, on plain http (e.g. the VPN dashboard at
 *     http://10.7.0.1) where AudioWorklet is not exposed — the same M4AEngine
 *     runs here on the main thread (workload is ~1% of a core, so it's fine).
 *
 * No build step, no deps. See WORKLET_HANDOFF.md.
 */
import { M4AEngine } from './m4a-engine.js';

const DATA = "data/";

let ctx = null;
let node = null;          // AudioWorkletNode OR ScriptProcessorNode
let engine = null;        // local M4AEngine for the ScriptProcessorNode fallback
let usingWorklet = false;
let bankSent = false;
let bank = null;          // {label:{rate,loop,loopStart,data:Int8Array}}
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
    bank[label] = { rate: v.rate, loop: v.loop, loopStart: v.loopStart, data: b64ToInt8(v.b64) };
  }
}

async function ensureAudio() {
  if (ctx) {
    if (ctx.state === "suspended") await ctx.resume();
    return;
  }
  ctx = new (window.AudioContext || window.webkitAudioContext)();
  if (!bank) decodeBank();

  // Prefer AudioWorklet (secure contexts only); else ScriptProcessorNode.
  let workletOk = false;
  if (ctx.audioWorklet) {
    try {
      await ctx.audioWorklet.addModule("m4a-worklet.js");
      node = new AudioWorkletNode(ctx, "m4a-processor", { outputChannelCount: [2] });
      node.port.onmessage = (e) => { if (e.data && e.data.type === "ended") clearPlaying(); };
      node.connect(ctx.destination);
      workletOk = true;
    } catch (err) {
      console.warn("AudioWorklet unavailable, falling back to ScriptProcessorNode:", err);
    }
  }

  if (workletOk) {
    usingWorklet = true;
    node.port.postMessage({ type: "bank", bank }, Object.values(bank).map((s) => s.data.buffer));
    bank = null;                       // buffers transferred to the audio thread
    bankSent = true;
  } else {
    // Fallback: run the same engine on the main thread via ScriptProcessorNode.
    usingWorklet = false;
    engine = new M4AEngine(ctx.sampleRate);
    engine.onended = () => clearPlaying();
    engine.loadBank(bank);             // main-thread engine keeps the bank (no transfer)
    bankSent = true;
    // 1 input channel (ignored): some browsers won't fire onaudioprocess with 0 inputs.
    node = (ctx.createScriptProcessor || ctx.createJavaScriptNode).call(ctx, 2048, 1, 2);
    node.onaudioprocess = (ev) => {
      const out = ev.outputBuffer;
      engine.render(out.getChannelData(0), out.getChannelData(1), out.length);
    };
    node.connect(ctx.destination);
  }

  if (ctx.state === "suspended") await ctx.resume();
}

function sendPlay(song) {
  if (usingWorklet) node.port.postMessage({ type: "play", song });
  else engine.play(song);
}
function sendStop() {
  if (!node) return;
  if (usingWorklet) node.port.postMessage({ type: "stop" });
  else if (engine) engine.stop();
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

  for (const entry of manifest) {
    const row = document.createElement("div");
    row.className = "song";
    row.innerHTML = `<span class="title">${entry.title}</span>
      <span class="file">${entry.name}</span>
      <span class="btns"><button>play</button></span>`;
    list.appendChild(row);
    const btn = row.querySelector("button");
    btn.onclick = async () => {
      try {
        const wasPlaying = btn === playingBtn;
        await ensureAudio();
        sendStop();
        clearPlaying();
        if (wasPlaying) return;
        if (!songCache[entry.file]) songCache[entry.file] = await fetchJson(entry.file);
        sendPlay(songCache[entry.file]);
        btn.classList.add("playing");
        playingBtn = btn;
        setStatus(usingWorklet ? "" : "");
      } catch (err) {
        console.error(err);
        setStatus("audio error: " + (err && err.message ? err.message : err));
      }
    };
  }

  document.getElementById("stop").onclick = () => { sendStop(); clearPlaying(); };
}

main();
