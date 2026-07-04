/* player.js — main-thread shim for the m4a AudioWorklet.
 *
 * Fetches the engine (m4a.wasm, built from the game's src/m4a.c) and the
 * soundtrack (music.pak — the same file the game plays), hands both to the
 * worklet, and builds the song list from what the worklet reports back.
 * All synthesis happens in the engine; this file is just UI + data plumbing.
 *
 * No build step, no deps — vanilla, served static. AudioWorklet needs a
 * secure context (https/localhost).
 */
"use strict";

let ctx = null;
let node = null;          // the AudioWorkletNode hosting the engine
let playingBtn = null;
let onPlaying = null;     // one-shot: viz start once the worklet confirms
let origAudio = null;     // <audio> for the original-MIDI comparison renders

function setStatus(msg) {
  const el = document.getElementById("status");
  if (el) el.textContent = msg || "";
}

function clearPlaying() {
  document.querySelectorAll("button.playing").forEach((b) => b.classList.remove("playing"));
  playingBtn = null;
}

function stopOriginal() {
  if (origAudio) { origAudio.pause(); origAudio.src = ""; origAudio = null; }
}

async function initAudio() {
  // 'playback' asks for a large output buffer: this is a music player, not
  // an instrument, so trade untimeable latency for underrun-proof delivery.
  ctx = new (window.AudioContext || window.webkitAudioContext)({ latencyHint: "playback" });
  await ctx.audioWorklet.addModule("m4a-worklet.js");
  node = new AudioWorkletNode(ctx, "m4a-processor", { outputChannelCount: [2] });
  node.connect(ctx.destination);

  const [wasm, pak] = await Promise.all([
    fetch("m4a.wasm").then((r) => r.arrayBuffer()),
    fetch("music.pak").then((r) => r.arrayBuffer()),
  ]);
  const ready = new Promise((resolve, reject) => {
    node.port.onmessage = (e) => {
      const m = e.data;
      if (!m) return;
      if (m.type === "ready") resolve(m.songs);
      else if (m.type === "error") reject(new Error(m.message));
      else if (m.type === "playing") { if (onPlaying) onPlaying(m.song); onPlaying = null; }
      else if (m.type === "ended") { clearPlaying(); M4AViz.stop(); }
      else if (m.type === "viz") M4AViz.frame(m);
    };
  });
  node.port.postMessage({ type: "init", wasm, pak }, [wasm, pak]);
  return ready;
}

async function main() {
  const list = document.getElementById("songs");
  let songs, compare = {};
  try {
    songs = await initAudio();
  } catch (err) {
    console.error(err);
    // AudioWorklet needs a secure context (https/localhost) — likeliest cause.
    setStatus("audio error: " + (err && err.message ? err.message : err));
    return;
  }
  // local-only A/B renders of the composers' original MIDIs (gitignored;
  // tools/render_compare.py). Absent on a fresh checkout — buttons just
  // don't appear.
  try {
    const r = await fetch("compare/index.json");
    if (r.ok) compare = await r.json();
  } catch (_) { /* no comparison renders present */ }

  const play = async (btn, entry) => {
    try {
      const wasPlaying = btn === playingBtn;
      if (ctx.state === "suspended") await ctx.resume();
      stopOriginal();
      node.port.postMessage({ type: "stop" });
      clearPlaying();
      if (wasPlaying) { M4AViz.stop(); return; }
      onPlaying = (song) => M4AViz.start({ song, title: entry.title, actx: ctx });
      node.port.postMessage({ type: "play", i: entry.i });
      btn.classList.add("playing");
      playingBtn = btn;
      setStatus("");
    } catch (err) {
      console.error(err);
      setStatus("audio error: " + (err && err.message ? err.message : err));
    }
  };

  const playOriginal = (btn, entry) => {
    const wasPlaying = btn === playingBtn;
    stopOriginal();
    node.port.postMessage({ type: "stop" });
    clearPlaying();
    M4AViz.stop();
    if (wasPlaying) return;
    origAudio = new Audio(compare[entry.name].file);
    origAudio.play().catch((err) => setStatus("audio error: " + err.message));
    origAudio.onended = clearPlaying;
    btn.classList.add("playing");
    playingBtn = btn;
    setStatus("");
  };

  // one section per category, in pak (= category) order
  const sections = [];
  const rows = [];
  let cat = null, section = null;
  for (const entry of songs) {
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
    const hasOrig = !!compare[entry.name];
    row.innerHTML = `<span class="title">${entry.title}</span>
      <span class="file">${entry.name}</span>
      <span class="btns"><button>play</button>${hasOrig ? "<button>original</button>" : ""}</span>`;
    list.appendChild(row);
    const [btn, origBtn] = row.querySelectorAll("button");
    btn.onclick = () => play(btn, entry);
    if (origBtn) origBtn.onclick = () => playOriginal(origBtn, entry);
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
    stopOriginal();
    clearPlaying();
    M4AViz.stop();
  };
}

M4AViz.attach(document.getElementById("viz-wrap"));
main();
