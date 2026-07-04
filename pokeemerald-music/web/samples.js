/* samples.js — the instrument A/B page.
 *
 * One row per GBA sample used anywhere in the soundtrack (data/sf2/pairs.json).
 * [gba] plays the game's own sample, [sf2] the soundfont counterpart — one
 * note through the same m4a worklet (same envelope, same mix law), so the
 * only difference heard is the sample itself.
 *
 * Clicking either button on a pair that HAS both versions also saves that
 * choice server-side (POST api/pick): the last click per sample wins. The
 * saved list is the input for a future "best-of" instrument set — read it
 * back with GET api/picks.
 */
"use strict";

const DATA = "data/";
const PICK_GATE = 2.0;          // matches extract_sf2.py's audition trim

let ctx = null, node = null;
let bankSent = false, sf2Loaded = null;
let gbaSamples = null;
let picks = {};

function setStatus(msg) {
  const el = document.getElementById("status");
  if (el) el.textContent = msg || "";
}

async function fetchJson(p) { return (await fetch(p)).json(); }

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
  return new Int16Array(bytes.buffer);
}

async function ensureAudio() {
  if (!ctx) {
    ctx = new (window.AudioContext || window.webkitAudioContext)({ latencyHint: "playback" });
    await ctx.audioWorklet.addModule("m4a-worklet.js");
    node = new AudioWorkletNode(ctx, "m4a-processor", { outputChannelCount: [2] });
    node.connect(ctx.destination);
  }
  if (ctx.state === "suspended") await ctx.resume();
  if (!bankSent) {
    const bank = {};
    for (const [label, v] of Object.entries(gbaSamples)) {
      bank[label] = { rate: v.rate, loop: v.loop, loopStart: v.loopStart,
                      scale: 1 / 128, data: b64ToInt8(v.b64) };
    }
    node.port.postMessage({ type: "bank", bank },
      Object.values(bank).map((s) => s.data.buffer));
    bankSent = true;
  }
}

function ensureSf2() {
  if (!sf2Loaded) {
    sf2Loaded = (async () => {
      const samples = await fetchJson(DATA + "sf2/samples.json");
      const bank = {};
      for (const [label, v] of Object.entries(samples)) {
        bank[label] = { rate: v.rate, loop: v.loop, loopStart: v.loopStart,
                        scale: 1 / 32768, data: b64ToInt16(v.b64) };
      }
      node.port.postMessage({ type: "bank", bank },
        Object.values(bank).map((s) => s.data.buffer));
    })();
  }
  return sf2Loaded;
}

function playOne(voice, key) {
  const gate = voice.rhythm ? 0.3 : PICK_GATE;
  node.port.postMessage({ type: "stop" });
  node.port.postMessage({ type: "play", song: {
    name: "_pick", reverb: 0, loopStart: null, loopEnd: null,
    voices: { v0: voice },
    tracks: [[[0, "v", 90], [0, "p", 0], [0, "n", key, 110, gate, "v0"]]],
  } });
}

async function savePick(sample, choice, badge) {
  picks[sample] = choice;
  badge.textContent = choice;
  badge.className = "badge " + choice;
  try {
    const res = await fetch("api/pick", { method: "POST",
      body: JSON.stringify({ sample, choice }) });
    if (!res.ok) throw new Error(res.status);
  } catch (e) {
    setStatus("could not save pick (" + e.message + ") — is the shim serving api/?");
  }
}

function cleanName(label) {
  return label.replace(/^DirectSoundWaveData_/, "").replace(/_/g, " ");
}

async function main() {
  const list = document.getElementById("pairs");
  const [pairs, samples] = await Promise.all([
    fetchJson(DATA + "sf2/pairs.json"), fetchJson(DATA + "samples.json"),
  ]);
  gbaSamples = samples;
  try { picks = (await fetchJson("api/picks")).picks || {}; } catch { picks = {}; }

  let nPicked = 0;
  for (const p of pairs) {
    const row = document.createElement("div");
    row.className = "song";
    const picked = picks[p.gba];
    if (picked) nPicked++;
    row.innerHTML = `<span class="title">${cleanName(p.gba)}</span>
      <span class="badge ${picked || ""}">${picked || ""}</span>
      <span class="btns">
        <button class="gba">gba</button>
        ${p.sf2Voice ? '<button class="sf2">sf2</button>'
                     : '<span class="none" title="no GM equivalent in the soundfont">no sf2</span>'}
      </span>`;
    list.appendChild(row);
    const badge = row.querySelector(".badge");
    row.querySelector("button.gba").onclick = async () => {
      try {
        await ensureAudio();
        playOne(p.gbaVoice, p.key);
        if (p.sf2Voice) savePick(p.gba, "gba", badge);   // a choice only if there IS one
      } catch (err) { setStatus("audio error: " + err.message); }
    };
    const sf2Btn = row.querySelector("button.sf2");
    if (sf2Btn) sf2Btn.onclick = async () => {
      try {
        await ensureAudio();
        setStatus("loading soundfont…");
        await ensureSf2();
        setStatus("");
        playOne(p.sf2Voice, p.key);
        savePick(p.gba, "sf2", badge);
      } catch (err) { setStatus("audio error: " + err.message); }
    };
  }
  document.getElementById("count").textContent =
    `${pairs.length} samples · ${pairs.filter((p) => p.sf2Voice).length} with an sf2 counterpart`;
}

main();
