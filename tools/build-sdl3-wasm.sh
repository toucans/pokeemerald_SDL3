#!/bin/sh
# build-sdl3-wasm.sh — one-time: build SDL3 for Emscripten into vendor/sdl3-wasm.
#
# Emscripten ships no SDL3 port (its -sUSE_SDL only knows 1 and 2); SDL's own
# docs say to build SDL3 for wasm yourself with emcmake. Pinned to the same
# 3.2.x line as Debian's native libsdl3-dev so both builds run the same SDL.
# Needs: emcc/emcmake (apt install emscripten), cmake, ninja.
set -eu
VERSION=3.2.10
cd "$(dirname "$0")/.."
mkdir -p vendor
cd vendor
[ -d "SDL3-$VERSION" ] || {
    curl -fLO "https://github.com/libsdl-org/SDL/releases/download/release-$VERSION/SDL3-$VERSION.tar.gz"
    tar xzf "SDL3-$VERSION.tar.gz"
    rm "SDL3-$VERSION.tar.gz"
}
emcmake cmake -S "SDL3-$VERSION" -B sdl3-wasm-build -G Ninja \
    -DCMAKE_BUILD_TYPE=Release -DSDL_SHARED=OFF -DSDL_STATIC=ON \
    -DCMAKE_INSTALL_PREFIX="$PWD/sdl3-wasm"
cmake --build sdl3-wasm-build
cmake --install sdl3-wasm-build
echo "SDL3 $VERSION for wasm installed in vendor/sdl3-wasm"
