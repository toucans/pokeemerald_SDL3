/* sc88z-prejs.js — web game build glue (emcc --pre-js, after docs/sc88z.js).
 *
 * The web build preloads docs/music-sc88-compressed.pak (the Opus transport
 * pak, tools/compress_sc88.py) instead of the 50 MB original. Before main()
 * runs, decode it back to the exact music-sc88.pak bytes in MEMFS, so
 * src/m4a.c opens the same file it opens natively and never knows the
 * samples travelled as Opus. Native builds read the original from disk and
 * never see this file.
 */
var Module = typeof Module != "undefined" ? Module : {};
Module.preRun = Module.preRun || [];
Module.preRun.push(function () {
  addRunDependency("sc88z"); // hold main() until the pak is rebuilt
  var ctxs = {};
  var decode = function (blob, rate) {
    var c = ctxs[rate] || (ctxs[rate] = new OfflineAudioContext(1, 1, rate));
    return c.decodeAudioData(blob).then(function (ab) { return ab.getChannelData(0); });
  };
  var z = FS.readFile("docs/music-sc88-compressed.pak");
  reconstructSC88(z.buffer, decode).then(function (out) {
    FS.writeFile("docs/music-sc88.pak", out);
    FS.unlink("docs/music-sc88-compressed.pak");
    removeRunDependency("sc88z");
  }, function (err) {
    console.warn("sc88z:", err); // audio.c logs; the GBA soundtrack still plays
    removeRunDependency("sc88z");
  });
});
