# System summary

Live snapshot. Last updated: 2026-10-24 (`master` after the zk_libcod
callbacks, dvars and last functions).

## Tree

```
src/                     engine, server, game, libcod (see ../architecture/overview.md)
mods/bots/               bots mod (GSC, 4 files)
scripts/                 launchers, systemd unit, cfg generator, bots installer, tests, docs checker
  linux/                 iniciar_cod2rev.sh, cod2rev@.service
  windows/               iniciar_cod2rev.bat
docs/                    this documentation, CHANGES.md
.github/workflows/       build.yml (4 builds)
Makefile                 all builds
README*.md               user README in 6 languages
AGENTS.md                agent entry point
LICENSE                  AGPL-3.0
run_bin.sh               runs bin/ from the repository (upstream's)
```

## Outputs

| Binary | Build | Accepts |
|---|---|---|
| `bin/cod2rev_lnxded` | `make` | protocols 115–119 (libcod) |
| `bin/cod2rev_lnxded` | `make PROTOCOL=118` | protocol 118 |
| `bin/cod2rev_win32.exe` | `make windows` / `PROTOCOL=118` | as the Linux build of the same version |

## State per subsystem

| Subsystem | State |
|---|---|
| Reversed engine | Upstream's; synced to voron00 `672a220` (q_shared and com_math review merged 2026-10-29) |
| Network hardening | Done: message/fragment bounds, rate limits, rcon lockout and whitelist, download checks, format strings |
| Logging and admin | Done: log naming/rotation/timestamps, `g_logRotateSize`, forced rate/snaps, kick/timeout messages, clean SIGTERM/SIGINT/SIGHUP quit |
| Long uptime | Time wrap restart moved to a map change after day 18.6; snapshot counters reload the map; full dvar pool is not fatal for scripts or rcon |
| Crash reports | Done: `crash.log` in `fs_homepath`, Linux and Windows |
| libcod | 203 function and 200 method entries, aliases included (`src/libcod/gsc.cpp`); zk_libcod port complete, its callbacks and dvars included (left-outs in [libcod.md](../architecture/libcod.md#not-ported-on-purpose)) ([ADR 0003](../architecture/adr/0003-zk-libcod-names.md)); shell, memory, raw path and any-address builtins need `+set sv_libcodUnsafe 1` ([ADR 0008](../architecture/adr/0008-unsafe-builtins-behind-a-dvar.md)) |
| JSON, HTTP/WebSocket, sounds | Done: yyjson, Mbed TLS 3.6.7 with own HTTP client, Speex 1.2.1 |
| Bots mod | dm, tdm, sd, ctf; A\* over per-map cached graphs; no HQ roles |
| Listen server | `LISTEN_SERVER` builds the server into a client, for the maintainer's client project; off in this repo's builds |
| Windows build | MinGW, static, threads through a stdcall wrapper |
| Deployment | Launchers (LAN/internet mode, port 28960), `scripts/linux/cod2rev@.service` |
| Docs | This layer plus `scripts/check_docs.py` (2026-10-22) |

## Commands

| Task | Command |
|---|---|
| Build 1.0 / 1.3 / Windows | `make`, `make PROTOCOL=118`, `make windows` |
| Smoke test | `python3 scripts/smoke_test.py <game>` (`--protocol 118 --binary <bin>`) |
| Bots test | `python3 scripts/bots_test.py <game> [--gametype sd\|ctf]` |
| HTTP test | `python3 scripts/http_test.py <game>` |
| Docs check | `python3 scripts/check_docs.py` and `python3 -m unittest discover -s scripts -p 'test_*.py'` |
| Regenerate example cfg | `python3 scripts/gen_server_cfg.py` |
| Install bots | `python3 scripts/install_bots.py <game> --dest <home>` |

## Contracts

| Contract | Defined by | Consumers |
|---|---|---|
| Network protocol 115/118/119 | Stock clients | Every client; must not change |
| GSC builtins and `CodeCallback_*` | zk_libcod names | Server mods, `mods/bots` |
| Dvars | `scripts/server.cfg.example`, [../CHANGES.md](../CHANGES.md#new-variables) | Admins' `server.cfg` |
| `crash.log` format | `Sys_InstallCrashHandler` in `src/unix/unix_main.cpp`, `src/win32/win_main.cpp` | Admins, `addr2line` |
| Waypoint graph files | `src/libcod/gsc_graph.cpp` | `mods/bots` cache per map |
