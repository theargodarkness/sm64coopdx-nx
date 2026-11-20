#!/bin/sh
# Builds the Lua 5.3 static library for the Nintendo Switch target.
#
# The game links against a prebuilt liblua53.a on every other platform, there
# is no such prebuilt library for devkitA64, so it is built from the official
# source release, using the exact luaconf.h the game is compiled against.
#
# Usage: tools/switch/build_lua.sh   (needs DEVKITPRO set, curl, tar, sha256sum)
set -e

LUA_VERSION=5.3.5
LUA_SHA256=0c2eed3f960446e1a3e4b9a1ca2f3ff893b6ce41942cf54d5dd59ab4b3b058ac
LUA_URL="https://www.lua.org/ftp/lua-$LUA_VERSION.tar.gz"

ROOT=$(cd "$(dirname "$0")/../.." && pwd)
OUT="$ROOT/lib/lua/switch"
WORK="$ROOT/build/lua_nx"

if [ -f "$OUT/liblua53.a" ]; then
    exit 0
fi

if [ -z "$DEVKITPRO" ]; then
    echo "build_lua.sh: DEVKITPRO is not set" >&2
    exit 1
fi
PATH="$DEVKITPRO/devkitA64/bin:$PATH"
CC=aarch64-none-elf-gcc
AR=aarch64-none-elf-ar

mkdir -p "$OUT" "$WORK"
cd "$WORK"

# LUA_SRC_TARBALL lets you provide the archive yourself (offline builds)
if [ -n "$LUA_SRC_TARBALL" ]; then
    cp "$LUA_SRC_TARBALL" lua.tar.gz
elif [ ! -f lua.tar.gz ]; then
    echo "Downloading Lua $LUA_VERSION..."
    curl -fsSL -o lua.tar.gz "$LUA_URL"
fi

ACTUAL=$(sha256sum lua.tar.gz | cut -d' ' -f1)
if [ "$ACTUAL" != "$LUA_SHA256" ]; then
    echo "build_lua.sh: checksum mismatch for lua-$LUA_VERSION.tar.gz" >&2
    echo "  expected $LUA_SHA256" >&2
    echo "  actual   $ACTUAL" >&2
    rm -f lua.tar.gz
    exit 1
fi

rm -rf "lua-$LUA_VERSION"
tar xzf lua.tar.gz
cd "lua-$LUA_VERSION/src"

# same configuration as the headers in lib/lua/include
cp "$ROOT/lib/lua/include/luaconf.h" luaconf.h

CFLAGS="-O2 -march=armv8-a+crc+crypto -mtune=cortex-a57 -mtp=soft -fPIE -ffunction-sections -D__SWITCH__ -DLUA_COMPAT_5_2"

OBJS=""
for src in *.c; do
    case "$src" in
        lua.c|luac.c) continue ;; # standalone interpreter and compiler
    esac
    echo "Compiling Lua: $src"
    $CC $CFLAGS -c "$src" -o "${src%.c}.o"
    OBJS="$OBJS ${src%.c}.o"
done

$AR rcs liblua53.a $OBJS
cp liblua53.a "$OUT/liblua53.a"
echo "Built $OUT/liblua53.a"
