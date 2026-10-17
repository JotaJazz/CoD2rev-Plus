# CoD2rev-Plus

<p align="center"><img src="docs/banner.svg" alt="CoD2rev-Plus" width="100%"></p>

[![Build](https://github.com/JotaJazz/CoD2rev-Plus/actions/workflows/build.yml/badge.svg)](https://github.com/JotaJazz/CoD2rev-Plus/actions/workflows/build.yml)
[![License: AGPL-3.0](https://img.shields.io/badge/license-AGPL--3.0-blue)](LICENSE)
![Platforms: Linux | Windows](https://img.shields.io/badge/platform-Linux%20%7C%20Windows-lightgrey)
![CoD2 1.0 | 1.3](https://img.shields.io/badge/CoD2-1.0%20%7C%201.3-c9b37e)
[![Fork of voron00/CoD2rev_Server](https://img.shields.io/badge/fork%20of-voron00%2FCoD2rev__Server-555)](https://github.com/voron00/CoD2rev_Server)
[![Last commit](https://img.shields.io/github/last-commit/JotaJazz/CoD2rev-Plus)](https://github.com/JotaJazz/CoD2rev-Plus/commits/master)

[Español](README.es.md) · **English** · [Português](README.pt.md) · [Français](README.fr.md) · [Deutsch](README.de.md) · [Русский](README.ru.md)

Reverse-engineered source code of the **Call of Duty 2** dedicated server (versions 1.0 and 1.3).
It fixes several bugs in the original binaries and lets developers extend the server's functionality.

This repository is a fork of [voron00/CoD2rev_Server](https://github.com/voron00/CoD2rev_Server), which is where all the reverse-engineering work was done. This fork only adds the changes described in [Changes in this fork](#changes-in-this-fork).

## Transparency

- **Who maintains this:** I'm JotaJazz. I like old games, and that is why this fork exists: I want to keep being able to run a CoD2 server on my own machines.
- **How the work is done:** I work with [Claude Code](https://claude.com/claude-code), Anthropic's coding assistant. The changes in this fork, including this README and its translations, are written with its help. Commits made this way carry a `Co-Authored-By: Claude` line in their message, so they can be identified in the history with `git log`.
- **Relationship with the original:** this fork pulls changes from the original repository when there are any, but does not send its own back. It will probably drift away from it over time.
- **What it is not:** it is not an official project and is not affiliated with Activision, Infinity Ward or the author of the original repository. It contains no game files.

## Changes in this fork

- **Windows build with MinGW** (32-bit), including cross-compiling from Linux.
- **Passwords kept out of the log:** `rcon_password` and `sv_privatePassword` are shown as `********` in the console log.
- **Rcon lockout after failed attempts:** after several wrong passwords, the IP address is ignored by rcon for a while. A correct password resets the counter, and `127.x.x.x` addresses are never locked.
- **Master servers:**
  - `sv_master2` is preset to `master.cod2x.me`, the CoD2x community master.
  - The `host:port` format in `sv_master1`–`sv_master5` now works.
  - A DNS failure is retried instead of clearing the master.
- **Game version selectable at build time:** the server can be built as 1.0 or as 1.3.
- **Overflow fix for Game Pass clients:** sending the system info to a client using protocol 119 could overflow a buffer and crash the server.
- **Large messages from 1.3 clients:** fragmented messages sent by 1.3 clients were misread and dropped; they are now read in the 1.3 format.
- **Bounded strings in libcod script functions:** the libcod `printf` and `sprintf`, the asynchronous SQLite and exec calls and the player rename no longer write past their buffers when a script passes long strings.
- **Simpler builds:** `make` builds the 32-bit Linux binary directly, `make windows` cross-compiles for Windows, and GitHub Actions builds every variant on each push.
- **Example `server.cfg`:** `scripts/server.cfg.example` lists every variable the server registers; `scripts/gen_server_cfg.py` regenerates it from the source.
- **`dedicated` is honoured:** `+set dedicated 1` was ignored and every server announced itself to the master servers. Now `1` keeps the server off the public list and `2`, the default, announces it.
- **Game files on Windows drives:** the Linux build could not see the game files on file systems with large inode numbers, such as Windows drives under WSL. It is now built with 64-bit file offsets.
- **Steady game clock on Linux:** the server measured time with the wall clock, so every clock correction (NTP, virtual machine time sync) froze the game for a moment. It now uses the monotonic clock.
- **Network input hardening:** a connected player could crash the server or overwrite its memory with an oversized compressed message, a fragmented message, a name containing `%` (written to the log by the stock gametypes), `callvote clientkick` with a negative number or voice packets. These paths are now bounds-checked, and the file handles of interrupted HTTP downloads are closed.
- **Bots after a map change:** after a map change, bots stayed connected but out of the game, holding their slots, so the bots mod kept adding more until the server was full. They now come back into the game as soon as the new map loads.
- **libcod proxy address and downloads:** the real address that a local proxy passes in the `ip` userinfo key is only trusted from `127.0.0.1`, must be an IPv4 address and is cleared on every connect. A `donedl` from a player already in the game no longer makes the server resend the whole gamestate.
- **Downloads and passwords:** the check meant to stop clients from downloading the stock `iw_*.iwd` files never matched, so any client could fetch them (hundreds of MB each); they and the server-only `_svr_` iwds are now refused, also before an HTTP redirect. Downloads no longer get stuck when the map changes during one, a client message too large to decompress drops the client instead of being cut short, and rcon, `g_password` and `sv_privatePassword` are compared in constant time.
- **Bans, game log and other fixes found in zk_libcod:** `addip` and `g_banIPs` kept a list of banned addresses that nothing checked; it is now enforced on connect, also for players behind a local proxy. The last address in `g_banIPs` is no longer skipped, and a long `addip` no longer overflows the stack. Line breaks in chat messages and player names could write fake lines into `games_mp.log` and split `getstatus` replies; they are now removed. A wrong rcon password no longer holds up the right one, a `userinfo` no longer swallows the next chat message, and holding the grenade button without grenades no longer hides the player's shots and footsteps from the others. The effect entities of grenade explosions are now freed instead of piling up until the map ends, a script that deletes a turret in use no longer ends the map, and a long map name passed to a script function no longer stops the server.
- **Crash reports:** when the server crashes, it writes the signal or exception, the faulting address and the call addresses to the console and to `crash.log` in `fs_homepath`. `addr2line -f -e <binary> <address>` turns them into source locations. On Linux the process then dies with the same signal as before, so core dumps still work.
- **Logging and admin settings:** the console log can be renamed (`logfileName`), keep the logs of previous runs (`logfileRotate`) and start each line with the date and time (`logTimestamps`). Accepted rcon commands can be left out of the log (`sv_logRcon`), the CD key authorization server can be changed (`sv_authorizeServer`, `sv_authorizePort`), the server can impose a rate and snapshot rate on every client (`g_forceRate`, `g_forceSnaps`), and the messages announcing a kick or a timeout can be turned off (`sv_kickMessages`, `sv_timeoutMessages`). The names are the same as in zk_libcod, so its configs keep working.
- **More libcod callbacks and player methods (from zk_libcod, same names):** `_callbacksetup.gsc` can define `CodeCallback_Error(critical, function, message)` (script runtime errors, delivered on the next frame), `CodeCallback_HitchWarning(msec)`, `CodeCallback_Pickup(type, weapon, name, quantity, clip, origin, angles)` (replaces the stock pickup), `CodeCallback_Suicide()` (replaces `suicide()`; `processSuicide()` kills), `CodeCallback_WeaponChange(index, name)` and one callback per button press: `Attack`, `Melee`, `Use`, `Reload`, `LeanLeft`, `LeanRight`, `Prone`, `Crouch`, `Stand`, `Ads`, `MeleeBreath`, `HoldBreath`, `Frag` and `Smoke` (`CodeCallback_<name>Button()`). New player methods: `isFiring`, `isMeleeing`, `isReloading`, `isThrowingGrenade`, `isUsingBinoculars`, `isRechambering`, `isShellShocked`, `isChatting`, `holdBreathButtonPressed`, `get`/`setCurrentWeaponAmmo`, `get`/`setCurrentWeaponClipAmmo`, `getCurrentWeaponSlot`, `get`/`setSpeed`, `get`/`setGravity`, `getGroundEntity`, `getViewOrigin`, `getPlayerstateFlags`, `getInactivityTime`, `getEnterTime` and `processSuicide`. Unlike zk_libcod, the weapon change and error callbacks never run inside player movement or the script VM, and a `suicide()` inside `CodeCallback_Suicide` does not loop.
- **Waypoint graphs with A\* and more script functions (from zk_libcod):** `graphCreate`, `graphAddNode`, `graphAddEdge`, `graphFindPath`, `graphFindClosestNode`, `graphAutodiscover`, `graphSave`/`graphLoad` and the rest of zk_libcod's `graph*` functions let bot scripts navigate a map (`src/libcod/gsc_graph.cpp`, ported under its MIT license). Unlike in zk_libcod, graphs are also cleared on a fast `map_restart` (every round of SD), a bad id or file returns undefined without ending the script thread, `graphLoad` refuses files over 64 MB and `graphSave` refuses `.iwd`, `.cfg` and `.gsc` names. Also new: `abs`, `floor`, `ceil`, `pow`, `atan2`, `roundTo`, `getMilliseconds`, `getMicroseconds`, `toHex`, `fromHex`, `ord`, `chr`, `getEntityCount`, `logPrintConsole` and the `getUserinfo`/`setUserinfo` methods; `bulletTrace` and `bulletTracePassed` take an optional content mask, and `bulletTrace` also returns `flags`, `contents` and `material`; with `sv_botUseTriggerUse 1`, a bot standing in a use trigger (such as an SD bomb zone) gets `self waittill("bot_trigger", trigger)`.
- **Smoke test:** `scripts/smoke_test.py` starts the Linux server against a real game folder and checks that it loads a map, answers queries and lets a minimal client (`scripts/cod2client.py`) connect, receive the gamestate, enter the game and send commands, including in fragmented messages.
- **Simple bots:** `mods/bots` is a small script mod whose bots navigate each map over a waypoint graph built with the libcod `graph*` functions: the first time a map is played the graph is generated from its spawn points and saved as `waypoints/<map>_v1.graph` in the mod folder, and later loads read it. With nobody in sight they follow what they last saw or heard shooting, patrol towards the enemy side, and only after 20 s without contact head for the nearest enemy. They aim down the sight at range, throw grenades at mid range, knife up close and reload between fights. In SD and CTF each life a bot draws a role: attackers plant the bomb or hunt, defenders guard a bomb site or hunt, and once the bomb is planted the defenders go to defuse it while the planters guard it; in CTF bots steal and bring home the enemy flag, or defend their own, chase its carrier and return it when it is dropped (SD in dual bomb mode is played without objectives). `scripts/install_bots.py` installs it; `bots_count`, `bots_skill` and `bots_debug` control them.
- **More bot buttons in libcod:** the script methods `useButton`, `throwSmoke` and `holdBreath` make a bot press use (to plant or defuse the bomb), throw a smoke grenade or hold its breath, like the existing `fireWeapon` and `throwNade`. Each takes `1` to press and `0` to release.
- **Fewer stalls and a cleaner startup:** the master server name is resolved once instead of on every authorization timeout, and the server no longer warns about every `+set` on the command line.
- **Launchers:** `scripts/windows/iniciar_cod2rev.bat` and `scripts/linux/iniciar_cod2rev.sh`, with a LAN mode and an internet mode.

### New variables

| Variable | Default | Range | Description |
| --- | --- | --- | --- |
| `sv_rconMaxAttempts` | `5` | 0–100 | Wrong rcon passwords allowed per IP before the lockout. `0` disables the lockout. |
| `sv_rconLockTime` | `600` | 0–86400 | Lockout duration in seconds. It is also the window in which attempts are counted. |
| `logfileName` | `""` | — | Console log file, in `fs_homepath/<mod>`. Empty uses `console_mp_server.log`; names with `..` or a full path are ignored. |
| `logfileRotate` | `0` | 0–1000 | Logs of previous runs kept when the server starts, as `<name>.1` (newest) to `<name>.N`. |
| `logTimestamps` | `0` | 0–1 | `1` starts each console log line with the date and time. |
| `sv_logRcon` | `1` | 0–1 | `0` leaves accepted rcon commands out of the log. Wrong passwords are always logged. |
| `sv_authorizeServer` | `cod2master.activision.com` | — | CD key authorization server. A new value is used from the next connection. |
| `sv_authorizePort` | `20700` | 1–65535 | Port of the authorization server. |
| `g_forceRate` | `0` | 0–25000 | Rate given to every client instead of its own (minimum 1000). `0` keeps the client's. |
| `g_forceSnaps` | `0` | 0–30 | Snapshots per second given to every client. `0` keeps the client's. |
| `sv_kickMessages` | `1` | 0–1 | `0` hides the message that tells everyone a player was kicked. |
| `sv_timeoutMessages` | `1` | 0–1 | `0` hides the message that tells everyone a player timed out. |
| `sv_botUseTriggerUse` | `0` | 0–1 | `1` sends a bot a `bot_trigger` notify, with the trigger, while it stands in a use trigger it could use. |
| `g_debugGraph` | `0` | 0–1 | `1` prints debug messages from the graph functions. |
| `sv_graphAstarMaxIterations` | `0` | 0–100000 | Maximum nodes one `graphFindPath` expands. `0` means no limit. |
| `sv_graphAstarWeight` | `1` | 1–2 | A\* heuristic weight. Above `1`, paths are found faster but can be longer. |
| `sv_graphMaxGraphs` | `8` | 1–64 | Maximum number of graphs at once. |
| `sv_graphMaxNodes` | `8192` | 64–65535 | Maximum nodes in one graph. |

## Game versions

- **The version is chosen when building.** By default the server is built as 1.0 (protocol 115); with `PROTOCOL=118` it is built as 1.3. The server announces that version to the master servers, so it shows up in the server list of clients of that same version.
- **The 1.0 build also accepts other versions by direct IP.** With libcod, which is enabled by default, it accepts connections using protocols 115 to 119, which includes 1.3 (protocol 118) and the Game Pass version (protocol 119). Direct IP connections to this build have been tested, including with a CoD2x 1.4.x client.
- **The 1.3 build only accepts protocol 118 clients.** It has been started on Linux and Windows, where it loads a map and answers as 1.3, and the Linux build has been played on with a CoD2x 1.4 client. [CoD2x](https://github.com/callofduty2x/CoD2x) 1.4 clients also connect with protocol 118, and their server browser only lists servers that report protocol 118 or 120, so this is the build they can see.

## Building

The output goes to `bin/` and intermediate objects to `obj/`.

### Windows (cross-compiled from Linux with MinGW)

Requires the 32-bit MinGW-w64 compiler (`i686-w64-mingw32`).

```bash
make windows
```

Produces `bin/cod2rev_win32.exe`.

### Linux

The server is a 32-bit x86 binary. On a 64-bit system you need GCC's 32-bit support (on Debian and Ubuntu, the `gcc-multilib` and `g++-multilib` packages):

```bash
make
```

Produces `bin/cod2rev_lnxded`.

### Game version

Add `PROTOCOL=118` to the `make` command to build the server as 1.3; without it, or with `PROTOCOL=115`, it is built as 1.0. Each platform and version keeps its own objects under `obj/`, so you can switch between them without `make clean`. The automated builds publish both versions for each platform.

### Automated builds

Every push to `master` that changes the code builds both versions on GitHub Actions ([`.github/workflows/build.yml`](.github/workflows/build.yml)). The resulting binaries can be downloaded as artifacts from the repository's [Actions](https://github.com/JotaJazz/CoD2rev-Plus/actions) tab.

### Makefile options

| Option | Default | Description |
| --- | --- | --- |
| `WITH_LIBCOD` | `true` | Includes the libcod script extensions (bots, players, entities, weapons, utilities, etc.). |
| `WITH_SQLITE` | `true` | Script functions for SQLite. |
| `WITH_MYSQL` | `false` | Script functions for MySQL. Requires the MySQL client library. |

`make clean` removes the binary and the objects.

## Running

The server needs the original game files (the `main` folder with its `.iwd` files), which are not in this repository. You need your own copy of Call of Duty 2.

### Windows

1. Copy `cod2rev_win32.exe` and `scripts/windows/iniciar_cod2rev.bat` into the game folder, next to `main`.
2. Have a `server.cfg` in `main`. [`scripts/server.cfg.example`](scripts/server.cfg.example) sets the usual options and lists every other variable the server registers with its default value.
3. Run `iniciar_cod2rev.bat`.

The launcher starts the server on port `28970`, to avoid clashing with the game client that uses `28960` on the same PC, and loads the map `mp_toujane`. By default it starts in LAN mode (`dedicated 1`, LAN or direct IP); run `iniciar_cod2rev.bat internet` to announce the server on the public list (`dedicated 2`).

### Key authorization

The server queries `cod2master.activision.com` to validate the key of every client that is not on the local network. With `sv_cracked 1` it still sends the query, but accepts the client whatever the answer is. The launchers turn it on; remove it from the command line if you want a valid key to be required.

### Linux

Copy `cod2rev_lnxded` and `scripts/linux/iniciar_cod2rev.sh` into the game folder, next to `main`, with a `server.cfg` in `main`. Run `./iniciar_cod2rev.sh` for LAN mode or `./iniciar_cod2rev.sh internet` to announce the server on the public list. It uses port `28960` and loads the map `mp_toujane`.

`run_bin.sh` is the original project's test script: it starts `bin/cod2rev_lnxded` in developer mode with cheats enabled and test bots. It is useful as an example of parameters, not as a configuration for a real server.

## Testing

`scripts/smoke_test.py` needs a built server and a game folder:

```bash
python3 scripts/smoke_test.py /path/to/CoD2
make PROTOCOL=118 && python3 scripts/smoke_test.py /path/to/CoD2 --protocol 118
```

It starts the server in LAN mode with a temporary home folder, so the game folder is only read. It checks that the server loads a map and answers `getinfo`, `getstatus` and rcon, and that `scripts/cod2client.py`, a minimal client, can connect, receive the gamestate, enter the game and send commands, including a message split into fragments. Use a game folder with the stock `.iwd` files: mods in `main` that replace the gametype scripts can stop the map from loading.

`scripts/bots_test.py` installs the bots mod in a temporary folder, plays a map with six bots and checks that the waypoint graph is built, saved and read back, that the bots follow paths and use sights, grenades and knives, and that no script error occurs:

```bash
python3 scripts/bots_test.py /path/to/CoD2 --map mp_carentan
```

With `--gametype sd` or `--gametype ctf` it checks instead that the bots plant the bomb or pick up the enemy flag.

To look for memory errors, build the server with AddressSanitizer in a separate folder and pass that binary with `--binary`:

```bash
make OBJ_ROOT=/tmp/asan/obj BIN_DIR=/tmp/asan/bin \
  CFLAGS="-fno-pie -O1 -g -fno-omit-frame-pointer -mfpmath=387 -DNDEBUG -DPROTOCOL_VERSION=115 -MMD -MP -m32 -D_FILE_OFFSET_BITS=64 -fsanitize=address" \
  LFLAGS="-no-pie -m32 -fsanitize=address"
```

`CFLAGS` given on the command line replaces the Makefile's, so `-m32` and `-D_FILE_OFFSET_BITS=64` must be repeated. Running that build for a while with the bots mod and a short `sv_mapRotation` also covers map changes and gameplay.

## Repository layout

| Path | Contents |
| --- | --- |
| `src/server` | Server logic: clients, networking, rcon, master servers. |
| `src/game`, `src/bgame` | Game logic. |
| `src/script` | GSC script virtual machine. |
| `src/qcommon`, `src/universal` | Shared code: commands, variables (dvars), file system, networking. |
| `src/xanim` | Models and animations. |
| `src/stringed` | Localized strings. |
| `src/libcod` | libcod script extensions, with SQLite and the MySQL headers. |
| `src/win32`, `src/unix` | Platform-specific code. |
| `src/zlib` | Copy of zlib. |
| `test` | Reverse-engineering material: IDA database, offset tables and test loader. |
| `scripts` | Launchers for Windows and Linux, the example `server.cfg` and its generator, the bots installer, the smoke test with its minimal client, and the bots test. |
| `mods/bots` | The simple bots script mod. |

## License

[GNU Affero General Public License v3.0](LICENSE), the same as the original project.

## Credits

- [voron00](https://github.com/voron00/CoD2rev_Server), author of the original project
- id Software
- Activision / Infinity Ward
- [CoD4x Server](https://github.com/callofduty4x/CoD4x_Server)
- [OpenBO2](https://github.com/builtbyxeno/OpenBO2)
- [zk_libcod](https://github.com/ibuddieat/zk_libcod) (ibuddieat), where several of the fixes in this fork were found
