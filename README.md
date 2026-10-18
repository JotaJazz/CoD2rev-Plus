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

- **Builds for Windows and Linux:** a MinGW build for Windows, `make` for 32-bit Linux, the game version (1.0 or 1.3) chosen at build time, automated builds on every push, launchers and an example `server.cfg`.
- **Security and stability:** network input that crashed the server or overwrote its memory is now bounds-checked; rcon locks out IPs after wrong passwords; downloads can't fetch the stock game files; IP bans are enforced; and a crash leaves a report in `crash.log`.
- **Compatibility:** large messages from 1.3 clients, Game Pass clients, master servers given as `host:port`, `dedicated 1`, game files on Windows drives under WSL, a steady game clock, and bots that come back after a map change.
- **Administration:** passwords kept out of the log, log naming, rotation and timestamps, a forced rate and snapshot rate, a configurable CD key authorization server and optional kick and timeout messages.
- **Scripting (libcod):** most of [zk_libcod](https://github.com/ibuddieat/zk_libcod)'s script functions and callbacks, with the same names: script errors with file and line, button presses, rcon commands, player and weapon control, admin tools, waypoint graphs with A\*, what each player sees, hears and collides with, physics, JSON, HTTP and WebSocket clients, sounds through voice chat and more, fixing several of its bugs on the way.
- **Bots mod:** `mods/bots`, bots that find their way over waypoint graphs, use sights, grenades and knives, and play the SD and CTF objectives.
- **Tests:** a smoke test with a minimal client that reads the server's snapshots, and a bots test that play real maps.

Every change in detail, with its functions, variables and fixes, is in [docs/CHANGES.md](docs/CHANGES.md) (in English).

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
| `WITH_SPEEX` | `true` | `loadSoundFile`, `playSoundFile` and the other sound functions, with Speex (`src/libcod/speex`). |
| `WITH_HTTP` | `true` | `httpFetch` and `webSocket*`, with Mbed TLS (`src/libcod/mbedtls`) for `https://` and `wss://`. |

`make clean` removes the binary and the objects.

## Running

The server needs the original game files (the `main` folder with its `.iwd` files), which are not in this repository. You need your own copy of Call of Duty 2.

### Windows

1. Copy `cod2rev_win32.exe` and `scripts/windows/iniciar_cod2rev.bat` into the game folder, next to `main`.
2. Have a `server.cfg` in `main`. [`scripts/server.cfg.example`](scripts/server.cfg.example) sets the usual options and lists every other variable the server registers with its default value.
3. Run `iniciar_cod2rev.bat`.

The launcher starts the server on port `28960` and loads the map `mp_toujane`. If the game also runs on this PC, start the server first: the game uses `28960` too, and a server started after it moves to the next free port. By default it starts in LAN mode (`dedicated 1`, LAN or direct IP); run `iniciar_cod2rev.bat internet` to announce the server on the public list (`dedicated 2`). The launchers set their values before `+exec server.cfg`, so `server.cfg` can change them.

### Key authorization

The server queries `cod2master.activision.com` to validate the key of every client that is not on the local network. With `sv_cracked 1` it still sends the query, but accepts the client whatever the answer is. The launchers only turn it on in LAN mode; in internet mode a valid key is required unless `server.cfg` turns it on.

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

With `--gametype sd` or `--gametype ctf` it checks instead that the bots plant the bomb or pick up the enemy flag. `--bombmode 1` plays SD in single bomb mode, with one bomb site. `--corrupt-cache` starts from a damaged waypoint file, which the bots must replace with a new graph.

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
