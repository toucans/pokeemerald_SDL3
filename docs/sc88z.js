/* sc88z.js — rebuild music-sc88.pak from music-sc88-compressed.pak.
 *
 * The compressed pak ("M4AZ", tools/compress_sc88.py) is the GitHub Pages
 * transport format: the M4AO pak with each sample's i16 PCM replaced by an
 * Ogg/Opus blob. This reconstructs the exact M4AO byte layout in memory, so
 * the engine (m4a.wasm) never knows the samples travelled as Opus.
 *
 * The layout logic is pure; Opus decoding is injected via decodeBlob(blob,
 * rate) -> Float32Array (decodeAudioData in the browser at the pak's own
 * sample rate, an opusdec subprocess in the node test). Decoded audio is
 * trimmed / zero-padded to each sample's recorded frame count, so loop
 * points survive the codec's 48 kHz round-trip exactly.
 */
"use strict";

async function reconstructSC88(buf, decodeBlob) {
  const dv = new DataView(buf);
  const u8 = new Uint8Array(buf);
  if (dv.getUint32(0, true) !== 0x5a41344d /* "M4AZ" */ || dv.getUint32(4, true) !== 1)
    throw new Error("not an M4AZ v1 pak");
  const nSamples = dv.getUint32(8, true);

  // walk the sample records, laying out the M4AO output as we go
  const recs = [];
  let off = 20;
  let w = 20;
  for (let i = 0; i < nSamples; i++) {
    const rate = dv.getFloat32(off, true);
    const n = dv.getUint32(off + 16, true);
    const blobLen = dv.getUint32(off + 20, true);
    recs.push({ hdr: off, rate, n, blob: off + 24, blobLen, out: w });
    off += 24 + blobLen + (-blobLen & 3);
    w += 20 + n * 2 + (-(n * 2) & 3);
  }
  const tail = off; // progs + songs sections, byte-identical in both formats

  const out = new Uint8Array(w + (buf.byteLength - tail));
  const odv = new DataView(out.buffer);
  out.set(u8.subarray(0, 20));
  odv.setUint32(0, 0x4f41344d, true); // "M4AO"
  out.set(u8.subarray(tail), w);

  await Promise.all(recs.map(async (r) => {
    out.set(u8.subarray(r.hdr, r.hdr + 20), r.out);
    const pcm = await decodeBlob(buf.slice(r.blob, r.blob + r.blobLen), r.rate);
    const n = Math.min(r.n, pcm.length); // rest stays zero-padded
    let p = r.out + 20;
    for (let i = 0; i < n; i++, p += 2) {
      const v = Math.round(pcm[i] * 32768);
      odv.setInt16(p, v > 32767 ? 32767 : v < -32768 ? -32768 : v, true);
    }
  }));
  return out;
}

if (typeof module !== "undefined") module.exports = { reconstructSC88 };
