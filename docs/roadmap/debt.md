# Debt, risks and open leads

What is known to be missing or fragile, with what a fix needs. Remove an
entry in the commit that resolves it and record the stage in
[../ai/PROJECT_MEMORY.md](../ai/PROJECT_MEMORY.md).

## Risks

| ID | Risk | Where | Fix needs |
|---|---|---|---|
| R2 | `svs.time` reaches `0x70000000` at about day 21.7 and the restart kicks everyone mid-match | `src/server/sv_main_mp.cpp` | Move the restart to the next map rotation after about day 18 |
| R3 | `Sys_Milliseconds` wraps at day 24.8; one map running more than 11 h hits the snapshot buffer counter checks, which restart the map | `src/unix`, `src/win32`, `SV_Frame` in `src/server/sv_main_mp.cpp` | Audit the time differences that survive a wrap |

## Known gaps

| Gap | Where | Notes |
|---|---|---|
| `SV_FX_GetVisibility` unimplemented: `sightTracePassed` ignores smoke | `src/server/sv_client_mp.cpp` | Returns 1.0 |
| `sv_pure` only proves the client has the server's iwds, not that it uses no others: the client reports only the iwds it has read from (`FS_ReferencedIwdPureChecksums`), so the server cannot require them all, and anyone with the stock files can compute the pure checksums from the `checksumFeed` in the gamestate; protocol 119 skips the check | `src/server/sv_client_mp.cpp` | Inherent to the Quake 3 design; a stricter check drops legit clients |
| `fprintln` adds no newline | `src/game/g_scr_main_mp.cpp` | Check against stock behaviour first |
| zPAM 4.x needs the CoD2x API (`getHWID`, `match*`, `g_cod2x` ≥ 5) | — | zPAM 3.36 runs on 1.0 and 1.3 |
| Protocol 119 fragment offset format | `src/qcommon/net_chan_mp.cpp` | Needs a Game Pass client |
| Script runtime errors cascade outside developer mode | `src/script/scr_vm.cpp` | Stock behaviour |

## Bots mod

| Item | Notes |
|---|---|
| Rare cascade of script errors when a bot joins a freed slot during a grenade kill (about 1 in 60 runs) | Not located; rerun with `+set developer 2` so `CodeCallback_Error` gets file and line |
| `mp_brecourt` graph hits the 8192-node cap (`sv_graphMaxNodes`) | Raise the cap or thin the grid |
| SD reloads the graph every round; graphs are not kept across `map_restart` | Cache in libcod by map name |
| Multi-floor cells in the graph builder | 1–2 days of work |
| HQ objectives not played | Only dm, tdm, sd, ctf have roles |
| Bullet antilag rewinds are not grouped per frame | Performance only |

## Deferred decisions

| Lead | Notes |
|---|---|
| Second listening port answering as 1.3, so one process is listed for 1.0 and 1.3 | zk_libcod's `code/proxy/proxy.c` (BSD) is a base; smaller step: do not treat `127.0.0.1` clients with an `ip` key as LAN |
| Stats mod (kills with positions into SQLite) | Compatible with Lonsofore/CoD2MapStats |
| MySQL async functions | No 32-bit MySQL client in the default toolchain |
| soundalias functions | See [ADR 0003](../architecture/adr/0003-zk-libcod-names.md) |
| CPU profile with 20 bots | Not measured yet |
| Releases page | The READMEs' "How to run" points to it; CI already builds the four binaries as artifacts |

## Accepted simplifications

- `playFxOnTagForPlayer` filters spectators by the followed player's
  `clientNum`: kept on purpose.
- One announced version per binary ([ADR 0001](../architecture/adr/0001-game-version-at-build-time.md)).
