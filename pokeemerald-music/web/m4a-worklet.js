/* m4a-worklet.js — AudioWorklet host for the m4a engine (m4a.wasm).
 *
 * The engine itself is src/m4a.c in the pokeemerald_SDL3 repo — the same C
 * code the game plays music with — compiled standalone by
 * tools/build-m4a-wasm.sh. This file only hosts it: it receives the wasm
 * binary + music.pak from the main thread, instantiates, and per 128-sample
 * block asks the engine to render and forwards its per-GBA-frame viz
 * snapshots. No synthesis logic lives here.
 *
 * Messages in:  {type:'init', wasm, pak}  {type:'origpak', pak}
 *               {type:'play', i, orig}    {type:'stop'}
 * Messages out: {type:'ready', songs:[{i,name,title,cat}]}
 *               {type:'origready', has:{name:true,...}}
 *               {type:'playing', song:{name,title,reverb,range,voices}}
 *               {type:'viz', t, wrapped, v:[vid,pitch,level,pan,...]}
 *               {type:'ended'}  {type:'error', message}
 */
"use strict";

const VOICE_T = ["pcm", "sq", "wave", "noise"];

class M4AHost extends AudioWorkletProcessor {
  constructor() {
    super();
    this.e = null;                       // wasm exports
    this.wasPlaying = false;
    this.lastVizSeq = 0;
    this.port.onmessage = (ev) => this.onMessage(ev.data);
  }

  // memory views must be re-taken after growth; cache per buffer identity
  views() {
    const buf = this.e.memory.buffer;
    if (this._buf !== buf) {
      this._buf = buf;
      this._u8 = new Uint8Array(buf);
      this._f32 = new Float32Array(buf);
    }
    return this;
  }

  cstr(ptr) {
    // manual ASCII decode: AudioWorkletGlobalScope has no TextDecoder
    const u8 = this.views()._u8;
    let s = "";
    for (let q = ptr; u8[q]; q++) s += String.fromCharCode(u8[q]);
    return s;
  }

  async onMessage(msg) {
    if (msg.type === "init") {
      try {
        const { instance } = await WebAssembly.instantiate(msg.wasm, {
          env: { emscripten_notify_memory_growth: () => {} },
        });
        const e = (this.e = instance.exports);
        if (e._initialize) e._initialize();
        const pak = new Uint8Array(msg.pak);
        const p = e.malloc(pak.length);
        this.views()._u8.set(pak, p);
        if (!e.m4a_init_mem(p, pak.length, sampleRate))
          throw new Error("bad music.pak");
        e.m4a_set_viz(1);
        this.renderPtr = e.malloc(128 * 2 * 4);
        const songs = [];
        for (let i = 0; i < e.m4a_song_count(); i++) {
          songs.push({ i, name: this.cstr(e.m4a_song_name(i)),
                       title: this.cstr(e.m4a_song_title(i)),
                       cat: this.cstr(e.m4a_song_cat(i)) });
        }
        this.port.postMessage({ type: "ready", songs });
      } catch (err) {
        this.e = null;
        this.port.postMessage({ type: "error", message: String(err && err.message || err) });
      }
    } else if (msg.type === "origpak" && this.e) {
      try {
        const e = this.e;
        const pak = new Uint8Array(msg.pak);
        const p = e.malloc(pak.length);
        this.views()._u8.set(pak, p);
        if (!e.m4a_orig_mem(p, pak.length))
          throw new Error("bad music-orig.pak");
        const has = {};
        for (let i = 0; i < e.m4a_orig_count(); i++)
          has[this.cstr(e.m4a_orig_name(i))] = true;
        this.port.postMessage({ type: "origready", has });
      } catch (err) {
        this.port.postMessage({ type: "error", message: String(err && err.message || err) });
      }
    } else if (msg.type === "play" && this.e) {
      const e = this.e, i = msg.i;
      const voices = {};
      let origIdx = -1;
      if (msg.orig) {
        const namePtr = e.m4a_song_name(i);
        origIdx = e.m4a_orig_find(namePtr);
        if (origIdx < 0) {
          this.port.postMessage({ type: "error",
            message: "no original for " + this.cstr(namePtr) });
          return;
        }
        for (let v = 0; v < e.m4a_orig_nvids(origIdx); v++) {
          voices["v" + v] = { t: "pcm", duty: 0, rhythm: false, base: 60,
                              sample: this.cstr(e.m4a_orig_vid_label(origIdx, v)) };
        }
        e.m4a_play_orig_index(origIdx);
      } else {
        for (let v = 0; v < e.m4a_song_nvoices(i); v++) {
          voices["v" + v] = {
            t: VOICE_T[e.m4a_voice_type(i, v)],
            duty: e.m4a_voice_duty(i, v),
            rhythm: !!e.m4a_voice_rhythm(i, v),
            base: e.m4a_voice_base(i, v),
            sample: this.cstr(e.m4a_voice_label(i, v)),
          };
        }
        e.m4a_play_index(i);
      }
      this.wasPlaying = true;
      this.lastVizSeq = e.m4a_viz_seq();
      this.port.postMessage({ type: "playing", song: {
        name: this.cstr(e.m4a_song_name(i)),
        title: this.cstr(e.m4a_song_title(i)) + (msg.orig ? " (original)" : ""),
        reverb: e.m4a_song_reverb(i),
        range: [e.m4a_song_key_lo(i), e.m4a_song_key_hi(i)],
        voices,
      }});
    } else if (msg.type === "stop" && this.e) {
      this.e.m4a_stop();
      this.wasPlaying = false;
    }
  }

  process(_inputs, outputs) {
    const out = outputs[0];
    const L = out[0], R = out[1] || out[0];
    if (!this.e) { L.fill(0); if (R !== L) R.fill(0); return true; }
    const e = this.e;

    e.m4a_render(this.renderPtr, L.length);
    const f32 = this.views()._f32, base = this.renderPtr >> 2;
    for (let n = 0; n < L.length; n++) {
      L[n] = f32[base + 2 * n];
      R[n] = f32[base + 2 * n + 1];
    }

    const seq = e.m4a_viz_seq();
    if (seq !== this.lastVizSeq) {
      this.lastVizSeq = seq;
      const d = e.m4a_viz_data() >> 2;
      const n = f32[d + 2];
      const v = [];
      for (let k = 0; k < n; k++) {
        v.push("v" + f32[d + 3 + 4 * k], f32[d + 4 + 4 * k],
               f32[d + 5 + 4 * k], f32[d + 6 + 4 * k]);
      }
      this.port.postMessage({ type: "viz", t: f32[d], wrapped: !!f32[d + 1], v });
    }

    if (this.wasPlaying && !e.m4a_is_playing()) {
      this.wasPlaying = false;
      this.port.postMessage({ type: "ended" });
    }
    return true;
  }
}

registerProcessor("m4a-processor", M4AHost);
