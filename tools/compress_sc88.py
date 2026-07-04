#!/usr/bin/env python3
"""compress_sc88.py — docs/music-sc88.pak -> docs/music-sc88-compressed.pak.

The GitHub Pages transport format, nothing else: the same pak with every
sample's i16 PCM replaced by an Ogg/Opus blob (opusenc, 96 kbps mono VBR).
docs/sc88z.js decodes the blobs with WebAudio decodeAudioData in the browser
and rebuilds the exact M4AO byte layout of music-sc88.pak before handing it
to the engine — src/m4a.c never sees this file, and the distinct "M4AZ"
magic makes it unloadable by mistake. Everything local (game, dashboard
shim) keeps using music-sc88.pak.

Format "M4AZ" v1 (little-endian; counts u32):
  header: "M4AZ" u32 ver=1, u32 nSamples, u32 nProgs, u32 nSongs  (as M4AO)
  samples[n]: f32 rate, u32 loopMode, u32 loopStart, u32 loopEnd, u32 length
              (the M4AO sample header verbatim; length = PCM frames the
              decoded blob must be trimmed/zero-padded to),
              u32 blobLen, blob bytes (Ogg/Opus), pad to 4
  tail: the M4AO progs + songs sections, copied verbatim.

Needs opusenc (Debian: opus-tools). Python stdlib only. Rerun after any
music-sc88.pak regeneration (opus bytes may also differ across opusenc
versions, so the output is only build-to-build deterministic).
"""
import struct
import subprocess
import sys
import tempfile
import wave
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

WEB = Path(__file__).resolve().parent.parent / "docs"
BITRATE_KBPS = 96


def encode_opus(pcm, rate, tmpdir, idx):
    wav = tmpdir / f"{idx}.wav"
    opus = tmpdir / f"{idx}.opus"
    with wave.open(str(wav), "wb") as w:
        w.setnchannels(1)
        w.setsampwidth(2)
        w.setframerate(int(rate))
        w.writeframes(pcm)
    subprocess.run(["opusenc", "--quiet", "--bitrate", str(BITRATE_KBPS),
                    str(wav), str(opus)], check=True)
    blob = opus.read_bytes()
    wav.unlink()
    opus.unlink()
    return blob


def main():
    src = (WEB / "music-sc88.pak").read_bytes()
    magic, ver, n_samp, n_prog, n_song = struct.unpack_from("<4sIIII", src, 0)
    if magic != b"M4AO" or ver != 1:
        sys.exit(f"unexpected pak header: {magic} v{ver}")

    headers = []
    pcms = []
    off = 20
    for _ in range(n_samp):
        rate, _loop, _ls, _le, n = struct.unpack_from("<fIIII", src, off)
        headers.append(src[off:off + 20])
        pcms.append((src[off + 20:off + 20 + n * 2], rate))
        off += 20 + n * 2 + (-(n * 2)) % 4
    tail = src[off:]

    with tempfile.TemporaryDirectory() as td:
        tmpdir = Path(td)
        with ThreadPoolExecutor() as pool:
            blobs = list(pool.map(
                lambda a: encode_opus(a[1][0], a[1][1], tmpdir, a[0]),
                enumerate(pcms)))

    out = bytearray()
    out += struct.pack("<4sIIII", b"M4AZ", 1, n_samp, n_prog, n_song)
    for hdr, blob in zip(headers, blobs):
        out += hdr
        out += struct.pack("<I", len(blob))
        out += blob
        out += b"\0" * (-len(blob) % 4)
    out += tail

    dest = WEB / "music-sc88-compressed.pak"
    dest.write_bytes(out)
    print(f"{dest}: {len(out)/1e6:.1f} MB "
          f"(from {len(src)/1e6:.1f} MB: {sum(len(p) for p, _ in pcms)/1e6:.1f} MB "
          f"PCM -> {sum(len(b) for b in blobs)/1e6:.1f} MB opus, "
          f"{len(tail)/1e6:.1f} MB songs/progs verbatim)")


if __name__ == "__main__":
    main()
