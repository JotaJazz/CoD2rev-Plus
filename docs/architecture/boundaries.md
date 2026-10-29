# Boundaries

Rules every change follows. The umbrella header (`src/qcommon/qcommon.h`) makes
every module visible to every other, so these rules are conventions, checked in
review, not by the compiler.

## Layers

```
platform        src/unix, src/win32
engine core     src/universal, src/qcommon, src/zlib, src/stringed
server          src/server
game            src/game, src/bgame, src/script, src/xanim
extensions      src/libcod (+ vendored libraries)
content         mods/bots (GSC only)
```

## Dependency matrix

Calls allowed from the row module to the column module.

| From \ To | platform | core | server | game | libcod |
|---|---|---|---|---|---|
| platform | — | yes | `SV_*` entry points only | no | no |
| core | `Sys_*` | — | as in the original binary | as in the original binary | no |
| server | `Sys_*` | yes | — | yes (game interface) | inside `#ifdef LIBCOD` |
| game | no | yes | yes (`SV_*`) | — | inside `#ifdef LIBCOD` |
| libcod | `Sys_*` | yes | yes | yes | — |

"As in the original binary": calls the reversed code already makes are kept;
new ones are not added.

## Rules

1. **Reversed code mirrors the binary.** Function names, structure layouts,
   constants and control flow match the original 1.0/1.3 binaries. A fix
   changes the smallest part and says in the commit why the original was wrong.
2. **Script extensions live in `src/libcod`.** Engine files get a hook inside
   `#ifdef LIBCOD` that calls into libcod; the stock path (without `LIBCOD`)
   must still build and behave as the original. Hardening, admin dvars, logging
   and platform fixes go directly into the engine file they fix, as small
   changes.
3. **Both game versions.** Code under `#if PROTOCOL_VERSION` is checked and built
   for 115 and 118. Behaviour for protocol 119 clients is kept but cannot be
   tested.
4. **Both platforms.** A change under `src/` builds for Linux and Windows
   before it is committed.
5. **Fixed limits stay.** `MAX_CLIENTS`, `MAX_GENTITIES`, snapshot sizes and
   the network formats are those of the stock clients; changing them breaks
   compatibility with the game.
6. **Client prediction is not desynced.** Anything the client also computes
   (`src/bgame`, movement parameters) is not changed per player from the
   server.
7. **Vendored libraries are not edited.** `src/zlib`, `src/misc/sse_mathfun.h` and the libraries under
   `src/libcod` (`sqlite`, `yyjson`, `speex`, `mbedtls`, `mysql`) are kept as
   upstream; configuration goes in our own files
   (`src/libcod/mbedtls/cod2rev_mbedtls_config.h`).
8. **Script-facing names are stable.** GSC functions ported from zk_libcod keep
   its names and arguments ([ADR 0003](adr/0003-zk-libcod-names.md)).

## Forbidden

- New dynamic dependencies (shared libraries) for the default build.
- Running commands or reaching the network from scripts without a dvar the
  admin controls.
- Copying code from projects with an incompatible license (see
  [ADR 0004](adr/0004-upstream-and-references.md)).

## Accepted exceptions

| Exception | Where | Why |
|---|---|---|
| libcod reads and writes engine structures directly | `src/libcod/*.cpp` | It is the extension layer; the engine has no API for it |
| Per-recipient entity state in snapshots | `SV_BuildClientSnapshot` | Per-player visibility and collision need it |
| `LISTEN_SERVER` client calls | `src/server/server.h`, `sv_init_mp.cpp` | Lets a client embed this server; not in this server's own builds |
| MySQL headers vendored but off | `src/libcod/mysql/include` | `WITH_MYSQL=false` by default; the build needs the system client library |
