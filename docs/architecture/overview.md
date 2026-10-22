# Architecture overview

Reverse-engineered Call of Duty 2 multiplayer dedicated server, one 32-bit x86
binary per platform and game version. The structure follows the original
binaries (Quake 3 lineage), not a design of this fork.

## Modules

| Path | Role | Key files |
|---|---|---|
| `src/universal` | Base types, dvars, memory, math, file system | `q_shared.h`, `dvar.cpp`, `com_files.cpp` |
| `src/qcommon` | Engine core: commands, common loop, collision (`cm_*`), netchan, messages, BSP load | `qcommon.h`, `common.cpp`, `net_chan_mp.cpp`, `msg_mp.cpp` |
| `src/server` | Server: clients, snapshots, connectionless commands, rcon, master servers, map spawn | `sv_main_mp.cpp`, `sv_client_mp.cpp`, `sv_snapshot_mp.cpp`, `sv_init_mp.cpp` |
| `src/game` | Server-side game logic: entities, combat, GSC builtins of the stock game | `g_main_mp.cpp`, `g_active_mp.cpp`, `g_scr_main_mp.cpp` |
| `src/bgame` | Code shared with the client: player movement, weapons, animations | `bg_pmove.cpp`, `bg_weapons.cpp`, `bg_animation_mp.cpp` |
| `src/script` | GSC compiler and virtual machine | `scr_parser.cpp`, `scr_vm.cpp` |
| `src/xanim` | Models (XModel), skeletons (DObj) and animations; hit locations depend on it | `xanim.cpp`, `dobj.cpp`, `xmodel.cpp` |
| `src/stringed` | Localized strings | |
| `src/libcod` | Fork's script extensions (libcod) and their vendored libraries | `libcod.cpp`, `gsc*.cpp`, `net_http.cpp` |
| `src/unix`, `src/win32` | Platform layer: entry point, sockets, threads, console | `unix_main.cpp`, `win_main.cpp` |
| `src/zlib` | Vendored zlib (iwd archives) | |
| `src/ui`, `src/misc` | Single headers (`keycodes.h`, `hexrays_defs.h`) | |
| `mods/bots` | GSC bots mod, runs on the libcod graph and bot functions | `maps/mp/bots/_bots.gsc` |
| `scripts` | Launchers, systemd unit, `server.cfg` generator, bots installer, tests | see [../development/testing.md](../development/testing.md) |

## How the code links

- `src/qcommon/qcommon.h` is the umbrella header: it includes `universal`,
  `game/g_shared.h`, `server/server.h`, the collision headers, `xanim` and
  `libcod/gsc.hpp`. Almost every `.cpp` includes it, so the compiler does not
  enforce module boundaries; [boundaries.md](boundaries.md) states the rules.
- All modules link into one binary; there are no shared libraries and no game
  module DLL (`game` is linked in, unlike Quake 3's `qagame`).
- Frame: `Com_Frame` (`src/qcommon/common.cpp`) runs `Com_EventLoop`, which
  hands each packet to `SV_PacketEvent`, then `SV_Frame`
  (`src/server/sv_main_mp.cpp`): bots (`SV_UpdateBots` →
  `SV_BotUserMove_libcod`), game frames (`SV_RunFrame` → `G_RunFrame`),
  timeouts, snapshots (`SV_SendClientMessages`) and master heartbeats.
- Packets: `SV_PacketEvent` → `Netchan_Process` (fragments) →
  `SV_ExecuteClientMessage`; connectionless packets go to
  `SV_ConnectionlessPacket`. See [networking.md](networking.md).
- Map load: `SV_SpawnServer` (`src/server/sv_init_mp.cpp`) loads the BSP,
  starts the game (`G_InitGame`, which calls `Libcod_InitLevel`) and compiles
  the gametype scripts.
- libcod hooks are inline `#ifdef LIBCOD` blocks in `server` and `game`
  (most in `sv_client_mp.cpp`, `g_scr_main_mp.cpp`, `g_active_mp.cpp`,
  `g_weapon_mp.cpp`), not function patches. See [libcod.md](libcod.md).

## Build variants

| Variant | Switch | Effect |
|---|---|---|
| Game 1.0 / 1.3 | `PROTOCOL=115` / `118` | `PROTOCOL_VERSION` changes netchan, messages, file system and some game code |
| Linux / Windows | `make` / `make windows` | Platform layer and link flags |
| libcod | `WITH_LIBCOD`, `WITH_SQLITE`, `WITH_SPEEX`, `WITH_HTTP`, `WITH_MYSQL` | `-D LIBCOD` and `LIBCOD_COMPILE_*` |
| Dedicated / listen server | `LISTEN_SERVER` (not set by the Makefile) | Without it `DEDICATED` is defined; with it the server runs inside a client for local games |

## Subsystem notes

- [boundaries.md](boundaries.md): rules every change follows.
- [libcod.md](libcod.md): script extensions, per-client and per-entity state, VM facts.
- [networking.md](networking.md): netchan, protocols, hardening.
- [adr/](adr/README.md): decisions.
