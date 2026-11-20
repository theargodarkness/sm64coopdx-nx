# Nintendo Switch build

This fork adds a homebrew build of sm64coopdx for the Nintendo Switch
(`TARGET_SWITCH=1`). It needs a console that can run homebrew, and your own
copy of the US version of Super Mario 64: no game assets are part of the build.

## Status

| Area | State |
| --- | --- |
| Build (`.nro`) | Builds in CI with devkitA64 + libnx |
| Rendering | OpenGL ES 2 through mesa, 720p handheld / 1080p docked |
| Mods (Lua, DynOS) | Compiled in, loaded from the SD card |
| Online play | Not available yet. Hosting starts an offline game |
| CoopNet, Discord, Mumble, auto-updater | Not built on Switch |
| Split screen | In progress |

## Installing

1. Download the `sm64coopdx-switch` artifact from the latest
   [Build Switch](../../actions/workflows/build-switch.yaml) run and unzip it.
2. Copy the `switch` folder to the root of your SD card. You should end up with
   `sdmc:/switch/sm64coopdx/sm64coopdx.nro` next to the `mods`, `lang`,
   `palettes` and `dynos` folders.
3. Copy your ROM (`Super Mario 64 (U) [!].z64`, any file name ending in
   `.z64`) into `sdmc:/switch/sm64coopdx/`.
4. Start the game from the homebrew menu **through a game title** (hold R
   while launching a game). Album/applet mode does not give homebrew enough
   memory.

Save data, the config file and downloaded or added mods are all kept in
`sdmc:/switch/sm64coopdx/`. Put extra mods in `sdmc:/switch/sm64coopdx/mods/`.

## Building

### GitHub Actions

Every push to `main` or `switch-port` runs `.github/workflows/build-switch.yaml`
in the `devkitpro/devkita64` container and uploads the packaged build.

### Locally

Install [devkitPro](https://devkitpro.org/wiki/Getting_Started) with the
`switch-dev`, `switch-sdl2`, `switch-mesa` and `switch-zlib` packages, plus the
usual host tools (`gcc`, `g++`, `make`, `python3`, `hexdump`, `curl`). Then:

```sh
export DEVKITPRO=/opt/devkitpro
make TARGET_SWITCH=1 -j$(nproc)
```

The result is `build/us_nx/sm64coopdx.nro`. Copy it together with
`build/us_nx/{mods,lang,palettes,dynos}` to `sdmc:/switch/sm64coopdx/`.

The first build downloads the Lua 5.3.5 source from lua.org and builds it for
the Switch (`tools/switch/build_lua.sh`). For an offline build, point
`LUA_SRC_TARBALL` at a copy of `lua-5.3.5.tar.gz`.

### Debugging

When the game is started with `nxlink -s sm64coopdx.nro`, everything it prints
is sent back to the PC.
