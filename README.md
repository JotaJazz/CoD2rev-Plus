# CoD2rev_Server (JotaJazz's fork)

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
- **Simpler builds:** `make` builds the 32-bit Linux binary directly, `make windows` cross-compiles for Windows, and GitHub Actions builds every variant on each push.
- **Example `server.cfg`:** `scripts/server.cfg.example` lists every variable the server registers.
- **Windows launcher:** `scripts/windows/iniciar_cod2rev.bat`.

### New variables

| Variable | Default | Range | Description |
| --- | --- | --- | --- |
| `sv_rconMaxAttempts` | `5` | 0–100 | Wrong rcon passwords allowed per IP before the lockout. `0` disables the lockout. |
| `sv_rconLockTime` | `600` | 0–86400 | Lockout duration in seconds. It is also the window in which attempts are counted. |

## Game versions

- **The version is chosen when building.** By default the server is built as 1.0 (protocol 115); with `PROTOCOL=118` it is built as 1.3. The server announces that version to the master servers, so it shows up in the server list of clients of that same version.
- **The 1.0 build also accepts other versions by direct IP.** With libcod, which is enabled by default, it accepts connections using protocols 115 to 119, which includes 1.3 (protocol 118) and the Game Pass version (protocol 119). Direct IP connections to this build have been tested, including with a CoD2x 1.4.x client.
- **The 1.3 build only accepts protocol 118 clients.** It compiles for Linux and Windows, but it has not been tested in play yet. [CoD2x](https://github.com/callofduty2x/CoD2x) 1.4 clients also connect with protocol 118, and their server browser only lists servers that report protocol 118 or 120, so this is the build they can see.

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

Add `PROTOCOL=118` to the `make` command to build the server as 1.3; without it, or with `PROTOCOL=115`, it is built as 1.0. Run `make clean` before switching from one version to the other. The automated builds publish both versions for each platform.

### Automated builds

Every push to `master` builds both versions on GitHub Actions ([`.github/workflows/build.yml`](.github/workflows/build.yml)). The resulting binaries can be downloaded as artifacts from the repository's [Actions](https://github.com/JotaJazz/CoD2rev_Server/actions) tab.

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

The launcher starts the server on port `28970`, to avoid clashing with the game client that uses `28960` on the same PC, and loads the map `mp_toujane`. It uses `dedicated 1` (LAN or direct IP); with `dedicated 2` the server is announced on the public list.

### Key authorization

The server queries `cod2master.activision.com` to validate the key of every client that is not on the local network. With `sv_cracked 1` it still sends the query, but accepts the client whatever the answer is. The Windows launcher turns it on; remove it from the command line if you want a valid key to be required.

### Linux

`run_bin.sh` is the original project's test script: it starts `bin/cod2rev_lnxded` in developer mode with cheats enabled and test bots. It is useful as an example of parameters, not as a configuration for a real server.

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
| `scripts/windows` | Windows launcher. |

## License

[GNU Affero General Public License v3.0](LICENSE), the same as the original project.

## Credits

- [voron00](https://github.com/voron00/CoD2rev_Server), author of the original project
- id Software
- Activision / Infinity Ward
- [CoD4x Server](https://github.com/callofduty4x/CoD4x_Server)
- [OpenBO2](https://github.com/builtbyxeno/OpenBO2)
