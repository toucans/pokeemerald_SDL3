#!/bin/sh
# build-m4a-wasm.sh — compile src/m4a.c to a standalone wasm module for the
# pokeemerald-music site (web/m4a-worklet.js hosts it in an AudioWorklet).
#
# Standalone (no emscripten JS runtime): the worklet instantiates it with
# plain WebAssembly.instantiate. M4A_NO_STDIO drops the fopen loader so the
# module needs no WASI file imports — the worklet passes the pak as memory.
# The committed web/m4a.wasm is regenerable output of this script; rebuild it
# whenever src/m4a.c changes.
set -eu
cd "$(dirname "$0")/.."
emcc -O2 -std=c17 -DM4A_NO_STDIO --no-entry -sSTANDALONE_WASM \
    -sALLOW_MEMORY_GROWTH=1 \
    -sEXPORTED_FUNCTIONS=_malloc,_free,_m4a_init_mem,_m4a_play_name,_m4a_play_index,_m4a_stop,_m4a_is_playing,_m4a_render,_m4a_song_count,_m4a_song_name,_m4a_song_title,_m4a_song_cat,_m4a_song_reverb,_m4a_song_key_lo,_m4a_song_key_hi,_m4a_song_nvoices,_m4a_voice_type,_m4a_voice_duty,_m4a_voice_rhythm,_m4a_voice_base,_m4a_voice_label,_m4a_set_viz,_m4a_viz_seq,_m4a_viz_data,_m4a_sc88_mem,_m4a_sc88_count,_m4a_sc88_name,_m4a_sc88_find,_m4a_play_sc88_index,_m4a_sc88_is_current,_m4a_sc88_nvids,_m4a_sc88_vid_label \
    src/m4a.c -o pokeemerald-music/web/m4a.wasm
ls -la pokeemerald-music/web/m4a.wasm
