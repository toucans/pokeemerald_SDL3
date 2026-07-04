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
let onSC88Ready = null;   // one-shot: resolves the lazy music-sc88.pak load
let fileAudio = null;     // <audio> for the fluidsynth "rendered" comparisons

function setStatus(msg) {
  const el = document.getElementById("status");
  if (el) el.textContent = msg || "";
}

function clearPlaying() {
  document.querySelectorAll("button.playing").forEach((b) => b.classList.remove("playing"));
  playingBtn = null;
}

function stopFileAudio() {
  if (fileAudio) { fileAudio.pause(); fileAudio.src = ""; fileAudio = null; }
}

// The SC-88 pak is fetched only on the first "SC-88" click, then lives in
// the engine for the session. It travels as the Opus transport pak (~17 MB,
// rebuilt to the original bytes by sc88z.js); ?sc88full fetches the full
// 50 MB original instead, for A/B'ing the codec.
const sc88Compressed = !new URLSearchParams(location.search).has("sc88full");
const sc88DecodeCtxs = {};
function sc88DecodeBlob(blob, rate) {
  // an OfflineAudioContext at the pak's own rate pins decodeAudioData's
  // resampled output to that rate (Opus itself is always 48 kHz inside)
  const c = sc88DecodeCtxs[rate] ||= new OfflineAudioContext(1, 1, rate);
  return c.decodeAudioData(blob).then((ab) => ab.getChannelData(0));
}
let sc88State = null;     // null -> "loading" -> "ready" | "failed"
async function ensureSC88() {
  if (sc88State === "ready") return;
  if (sc88State === "loading") throw new Error("still loading the SC-88 soundtrack…");
  sc88State = "loading";
  setStatus(`loading SC-88 soundtrack (~${sc88Compressed ? 17 : 50} MB)…`);
  try {
    let pak;
    if (sc88Compressed) {
      const z = await (await fetch("music-sc88-compressed.pak")).arrayBuffer();
      setStatus("decoding SC-88 samples…");
      pak = (await reconstructSC88(z, sc88DecodeBlob)).buffer;
    } else {
      pak = await (await fetch("music-sc88.pak")).arrayBuffer();
    }
    const done = new Promise((res, rej) => { onSC88Ready = { res, rej }; });
    node.port.postMessage({ type: "sc88pak", pak }, [pak]);
    await done;
    sc88State = "ready";
    setStatus("");
  } catch (err) {
    sc88State = "failed";
    throw err;
  }
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
  let initDone = false;
  const ready = new Promise((resolve, reject) => {
    node.port.onmessage = (e) => {
      const m = e.data;
      if (!m) return;
      if (m.type === "ready") { initDone = true; resolve(m.songs); }
      else if (m.type === "sc88ready") { if (onSC88Ready) onSC88Ready.res(); onSC88Ready = null; }
      else if (m.type === "error") {
        if (onSC88Ready) { onSC88Ready.rej(new Error(m.message)); onSC88Ready = null; }
        else if (!initDone) reject(new Error(m.message));
        else { clearPlaying(); setStatus(m.message); }
      }
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
  let songs, rendered = {};
  try {
    songs = await initAudio();
  } catch (err) {
    console.error(err);
    // AudioWorklet needs a secure context (https/localhost) — likeliest cause.
    setStatus("audio error: " + (err && err.message ? err.message : err));
    return;
  }
  try {
    const r = await fetch("compare/index.json");
    if (r.ok) rendered = await r.json();
  } catch (_) { /* no local renders present: no "rendered" buttons */ }

  const play = async (btn, entry, sc88) => {
    try {
      const wasPlaying = btn === playingBtn;
      if (ctx.state === "suspended") await ctx.resume();
      if (sc88 && !wasPlaying) await ensureSC88();
      stopFileAudio();
      node.port.postMessage({ type: "stop" });
      clearPlaying();
      if (wasPlaying) { M4AViz.stop(); return; }
      onPlaying = (song) => M4AViz.start({ song, title: song.title, actx: ctx });
      node.port.postMessage({ type: "play", i: entry.i, sc88: !!sc88 });
      btn.classList.add("playing");
      playingBtn = btn;
      setStatus("");
    } catch (err) {
      console.error(err);
      setStatus("audio error: " + (err && err.message ? err.message : err));
    }
  };

  // fluidsynth renders of the same SC-88 MIDIs (local-only opus files;
  // tools/render_compare.py). For comparing against the engine's synthesis.
  const playRendered = (btn, entry, file) => {
    const wasPlaying = btn === playingBtn;
    stopFileAudio();
    node.port.postMessage({ type: "stop" });
    clearPlaying();
    M4AViz.stop();
    if (wasPlaying) return;
    fileAudio = new Audio(file);
    fileAudio.play().catch((err) => setStatus("audio error: " + err.message));
    fileAudio.onended = clearPlaying;
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
    const hasRender = !!rendered[entry.name];
    row.innerHTML = `<span class="title">${entry.title}</span>
      <span class="file">${entry.name}</span>
      <span class="btns"><button>GBA</button><button>SC-88</button>${
        hasRender ? "<button>fluidsynth</button>" : ""}</span>`;
    list.appendChild(row);
    const [gbaBtn, sc88Btn, flBtn] = row.querySelectorAll("button");
    gbaBtn.onclick = () => play(gbaBtn, entry, false);
    sc88Btn.onclick = () => play(sc88Btn, entry, true);
    if (flBtn) flBtn.onclick = () => playRendered(flBtn, entry, rendered[entry.name].file);
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
    stopFileAudio();
    clearPlaying();
    M4AViz.stop();
  };
}

M4AViz.attach(document.getElementById("viz-wrap"));
main();
