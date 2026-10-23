# Project memory

Operational memory: new stages are added at the top, old stages are never
rewritten, at most three bullets each. It records why work mattered, what was
rejected and what is left open; for what changed in detail see
[../CHANGES.md](../CHANGES.md) and `git log`.

## 2026-10-22 — README front page

- The six READMEs now open with what the server is for, "What it does today", how to run it and a section for contributors; build, running, testing and layout fold into `<details>`, with Notice and License at the end ([ADR 0007](../architecture/adr/0007-readme-front-page.md)), following OpenCoD2-Plus.
- "How to run" promises a Releases page that does not exist yet: publishing releases is open.

## 2026-10-22 — documentation system

- Project map, conventions, engine facts and open leads moved from a local `CLAUDE.md` and the assistant's private memory into `AGENTS.md` and `docs/` ([ADR 0006](../architecture/adr/0006-ai-documentation-layer.md)).
- `scripts/check_docs.py` runs locally, not in CI, so doc edits do not start builds.
- Rejected: committing `CLAUDE.md`; it stays local for machine-specific paths.

## 2026-10-22 — upstream sync: xanim review

- Merged voron00 up to `8301c00`: xanim code reviewed (`xanim_calc.cpp` folded into `xanim.cpp`), the old `test/` folder removed, named walkable-normal constants.
- `src/qcommon/qcommon.h` conflicted only by line endings; ours (LF) kept plus upstream's one new line.
- Verified with ASan bots runs (dm, sd, ctf on 1.0; dm, sd on 1.3) and a 10-minute rotation; hit locations spread over the whole skeleton.

## 2026-10-18 — listen server and rcon errors

- `LISTEN_SERVER` lets a client embed this server for local games (FX system recreated after `dedicated 1`); this repo's builds are unchanged.
- An error raised inside an rcon command now ends the redirect before unwinding, instead of writing over a freed stack buffer.

## 2026-10-17 — base hardening

- Format strings checked at build time, map name overflows refused, stack protector, FORTIFY, RELRO, `-fno-strict-aliasing -fwrapv`.
- Clean SIGTERM/SIGINT/SIGHUP quit, `g_logRotateSize`, `net_strictPort`, per-thread `sigaltstack`, systemd unit, Windows threads through a stdcall wrapper.
- Left open: `sv_pure` bypass, long-run time wraps, unsafe builtins behind a dvar ([../roadmap/debt.md](../roadmap/debt.md)).

## 2026-10-17 — audit of the fork

- A six-area review found about 60 issues: player and bot argument checks, level state reset on every `G_InitGame` with `libcodLevelGeneration`, SQLite/exec references and locking, HTTP limits, rcon guess counting and `sv_rconWhitelist`, WAV parsing, deferred map commands.
- The fake master check added to the smoke test found that `+set x ""` was dropped on Linux.

## 2026-10-17 — zk_libcod port completed

- Batches A–H: callbacks, player/weapon/admin functions, per-player visuals, script model physics and bullet drop, JSON, HTTP/WebSocket, sounds through voice chat.
- Libraries vendored and built in ([ADR 0002](../architecture/adr/0002-vendored-libraries.md)); names kept, unsafe or prediction-breaking functions left out ([ADR 0003](../architecture/adr/0003-zk-libcod-names.md)).
- Detailed change list moved from the READMEs to `docs/CHANGES.md` ([ADR 0005](../architecture/adr/0005-readme-and-changes.md)).

## 2026-10-16 — bots mod

- Bots navigate per-map waypoint graphs with A\*, hunt and patrol, use sights, grenades and knives; SD and CTF roles followed on 2026-10-17.
- `scripts/bots_test.py` checks graphs, paths, combat and objectives; CTF pickups are infrequent and can flake.
- Left open: HQ, multi-floor cells, graph node cap on `mp_brecourt`, a rare script error cascade.

## 2026-10-13 — fork started

- MinGW Windows build, game version chosen at build time ([ADR 0001](../architecture/adr/0001-game-version-at-build-time.md)), CI for four builds, launchers and `server.cfg` generator.
- First hardening: rcon lockout, bounded libcod strings, 1.3 fragment offsets, protocol 119 systeminfo overflow, `dedicated 1`, 64-bit file offsets, monotonic clock.
- Smoke test against a real game folder, six-language README with a Transparency section.
