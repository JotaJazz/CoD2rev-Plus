# Project memory

Operational memory: new stages are added at the top, old stages are never
rewritten, at most three bullets each. It records why work mattered, what was
rejected and what is left open; for what changed in detail see
[../CHANGES.md](../CHANGES.md) and `git log`.

## 2026-10-30 — logs, CI and docs

- From the audit. A fatal error's reason now reaches the console log and Linux flushes the logs before `_exit`; the launchers keep five old console logs. CI starts each published Linux binary without game files and expects exit code 1; the Windows binary is still only built.
- The docs caught up with the 64-bit build, which they still called impossible.

## 2026-10-30 — script callbacks

- From the audit. `CodeCallback_DPrintf` cleared the arguments of the running builtin, because `stackGetParams` prints on a type mismatch; `getArrayKeys` dropped integer keys; re-entry guards stayed set after a `Com_Error`, so a callback never ran again.
- `connectionlessPacket` was reported broken and is not: the three stray `0xff` bytes read as whitespace by the tokenizer.

## 2026-10-30 — libcod data safety

- From the audit. `ATTACH` and `VACUUM INTO` wrote files past `sqlite_open`'s folder; `map_restart` dropped queued async SQLite writes (498 of 500 in a test); on Windows `json_save_async` only worked once, confirmed by running the `.exe` through WSL; big JSON or a 64 KB cell ended the map; `mysql_*` had no gate, timeouts or queue cap.
- Outside developer mode a script error does not stop the thread: the statement goes on with `undefined`. Tests must check return values, and the `stackPushUndefined()` after `stackError()` is needed.
- Left open: many distinct cells of a few KB in one sync query can still run the string pool out; only each cell is cut.

## 2026-10-30 — network floods

- From an audit of the whole repo. `donedl` before entering the game cost 95% of a core and 130 MB in 6 s; `connect` had no limit; `getchallenge` shared the 10 replies a second of `getinfo`; a full bucket table refused every new address; bans were only read in the authorize reply. Fixed; the `donedl` run uses 1% of a core now.
- Rejected: limiting `donedl` by `messageAcknowledge`, which the client chooses. A per-slot budget covers every gamestate a client can trigger.
- rcon's global limit runs before the password now, at the maintainer's choice: blind guessing ends, and during a flood only `sv_rconWhitelist` and local tools get rcon. The entry eviction change could not be tested, since 127.x is exempt from the lockout.

## 2026-10-29 — bot script error cascade located; repaths and grenade suicides

- The "rare cascade when a bot joins a freed slot during a grenade kill" was misdescribed: 120 `developer 2` runs (tdm, dm, sd, ctf) showed one error source, stock `sd.gsc` (defuser killed on the last tick still defuses, lines 1925–1926). A grenade never passes to the next player in a slot: `G_FreeEntityRefs` clears `parent`, so a gone player's grenade does no damage. `bots_test.py` leaves out exactly that stock race.
- The same runs showed DM failing `bots search paths sparingly` in 16 of 40: repath on every goal-node change and after every combat tick. Fixed in the mod (moving goal twice a second, combat repath at most 0.5 s away). Grenade suicides were measured with a probe: most walked into their own grenade, the rest threw at cover past the 150-unit check; both fixed, 2.8 to 1.4 per 100 throws. Fight runs failing any check: 19 of 80 before, 1 of 24 after.

## 2026-10-29 — bot graph reload per SD round measured

- Rejected a graph cache across `map_restart`: `graphLoad` of a saved graph takes 0.36–0.40 ms on mp_toujane (1370 nodes), 0.44–0.51 ms on mp_burgundy (1659) and 0.57–1.08 ms on mp_carentan (2040), against a 50 ms frame. Building a graph the first time a map is played costs more, but happens once and is saved.
- If ever needed, `graphLoad(path, 1)` already keeps a graph across map restarts and map changes; the mod would only have to find its id again (for example in a cvar).

## 2026-10-29 — MySQL on 64-bit Linux

- `make ARCH=x64 WITH_MYSQL=true` links the system client (`mariadb_config`/`mysql_config`, or `MYSQL_LIBS`); the vendored 5.7 headers work with MariaDB's library (only `field->name`, the first member, is read). CI builds it without publishing.
- Fixed while testing against a private `mariadbd`: a 0 handle crashed the server; the async worker wrote `done`/`result` without the lock (TSan: 15 races before, none after); `mysql_close` left `mysql_reuse_connection` pointing at the closed connection; `int64 == int` was a type error in the VM, which broke `handle == 0` in every build.
- Local harness: `t_mysql.py` added; `t_zk4` placed `a` next to b's random spawn, sometimes inside a wall, and now picks a clear spot.

## 2026-10-29 — smoke and sightTracePassed

- Not a gap: in the 1.3 dedicated server (`cod2_lnxded`, `pc_1.3_1_1`) `sightTracePassed` also sees through smoke. `SV_FX_GetVisibility` reads blockers that only the client adds while drawing smoke particles; the dedicated server initialises the FX system, registers effects and reads their length, but never runs them, so the answer is always 1. The stub was right; it is now documented.
- Rejected: reversing the FX system (about 10k lines, Raven's effects code) for no change on a dedicated server; OpenCoD2 has it but no license, so it is read only ([ADR 0004](../architecture/adr/0004-upstream-and-references.md)). Left open: a smoke model of our own for bots, off by default, if wanted.

## 2026-10-29 — Linux 64-bit build

- `make ARCH=x64` ([ADR 0009](../architecture/adr/0009-linux-64-bit-build.md)): upstream already builds 64-bit, so only the Makefile, Mbed TLS's AES-NI flags (`-maes -mpclmul`), `rip` in the crash report and six libcod format strings changed. 32-bit stays the default.
- Verified on x64: smoke (1.0, 1.3), bots dm/sd/ctf (1.0) and dm/sd (1.3), HTTP, an 11-minute ASan rotation with 10 bots. The local GSC harness: 17 of 22 pass; the other 5 fail the same way on 32-bit (tests not updated for `sv_libcodUnsafe`, an old scratch path, changed messages; `t_collide` is position-dependent).
- Left open: making x64 the default, a Windows x64 build, the engine's `-Wformat` warnings on x64 (kept as upstream has them).

## 2026-10-29 — map ambience switches

- `g_ambientSound`, `g_fireFx`, `g_smokeFx`, `g_weatherFx`, `g_fog`: zPAM's `scr_allow_ambient_*` done in the engine, so they work for every mod and custom map. Looped effects are matched by the folder of their file; their entities are still spawned with `SVF_NOCLIENT`, as scripts keep and delete them.
- Played by hand: walking skipped steps on servers inside WSL2, and also on the build from before the upstream merge; the stock `CoD2MP_s.exe` and the fork's Windows build, both run natively, played smoothly ([../development/testing.md](../development/testing.md)).

## 2026-10-29 — upstream sync: q_shared and com_math review

- Merged voron00 up to `672a220`: `q_shared` and `com_math` reviewed (string helpers split into `com_shared.cpp`), double literals turned into floats across `bgame` and `game`, `sv_fastDownload` loop fix, SQLite 3.54.1.
- Conflicts: `bg_animation_mp.cpp` kept the fork's 1.3 constants (folded `46.25f`, float start-time division, which upstream still divides as integers); `q_shared` took upstream's and re-applied `I_strcmpConstantTime`, the `>=` info-string checks and the printf attributes; the fast download loop kept `sv_fastDownloadSpeed`.
- Rejected for now: upstream's SSE math flags; the Makefile stays on x87 and `sse_mathfun.h` is only included with `USE_SSE2` ([../roadmap/debt.md](../roadmap/debt.md)). Verified with smoke tests and ASan bots runs (dm, sd, ctf on 1.0; dm, sd on 1.3) and a rotation soak.

## 2026-10-24 — animation script as the 1.3 client

- Compared with the 1.3 client (CoD2MP_s.exe) while porting it into OpenCoD2-Plus: the parser set the strafe flags 0x10/0x20 for every condition but STRAFING (1.3 `0x4f7f54` checks STRAFING only), which turned the legs of 55 anims (the MG42 ones too) as strafes; `BG_IsCrouchingAnim` counted the crouched runs and turns and not the crouched stumbles (`0x4f9750`), and it and `BG_IsProneAnim` answered a bit mask, not 0 or 1, so two prone anims compared as a stance change (`0x4f97b0`).
- `BG_SetNewAnimation` divided the start time as integers, so every looping anim started at its first frame (1.3 divides as floats, `0x4f9adb`); the angles, swing, blend and rate code now uses 1.3's float constants (`0x4fa000`, `0x4fa230`, `0x4f9850`, `0x4f9d80`, `0x4faff0`), with 1/130 multiplied as 1.3 does.

## 2026-10-24 — player controllers as floats

- `BG_Player_DoControllersInternal` multiplied the torso, neck and head angles by double literals (`0.69999999`, `0.30000001`, ...) and `fLeanFrac * 50.0 * 0.925`; 1.3 (`0x4fa8c4`, controllers `0x4fac80`-`0x4fad54`) uses only float constants, with `50 * 0.925` and `50 * 0.075` folded to `46.25f` and `3.7500002f`. Under x87 the double products rounded differently in the last bit now and then; the hit-location bones follow 1.3 exactly now.

## 2026-10-24 — zk_libcod batch 6: last functions

- `getAuthorizationStatus`, `setActivateOnUseButtonRelease`, `lookAtKiller`, `playScriptAnimation` ported. The authorize answer is kept per challenge and copied to the client before `ClientConnect`, so the connect callback can read it.
- Left out: `setWeaponFuseTime` and `setWeaponRaiseTime` (`PM_Weapon` predicts both on the client; ADR 0003's rule). With this batch every zk_libcod builtin is either here or in the left-out list.

## 2026-10-24 — zk_libcod batch 5: mod folders

- `fs_callbacks`, `fs_gametypes`, `fs_mapScriptDirectories`, `fs_replaceStockMaps`, `loc_loadLocalizedMods` ported; every `_callbacksetup` lookup (stock and libcod) goes through `Libcod_CallbacksScript`.
- `fs_replaceStockMaps 0` changes manymaps: a stock map's library iwd used to be linked when coming from a custom map. `loc_loadLocalizedMods` only touches the loaded lists (`sv_iwds`, `sv_iwdNames`); the referenced lists already held every non-main iwd.
- Tested in the harness except `fs_replaceStockMaps` (needs a manymaps library with a custom map).

## 2026-10-24 — zk_libcod batch 4: load and network

- `sv_isLookingAtOnDemand`, `sv_fastDownloadSpeed` and `sv_minimizeSysteminfo` ported. The minimized dvars are found by name and only moved if they have `DVAR_SYSTEMINFO` here (`sv_cheats` is `DVAR_INIT` only); turning the dvar off restores the flags on the next map.
- Not ported: `net_noFragmentationDelay`; the `#ifdef LIBCOD` loop in `SV_SendClientMessages` already sends every pending fragment before a snapshot.

## 2026-10-24 — zk_libcod batch 3: admin switches

- `g_corpseHit`, `g_spectateBots`, `g_logPickup`, `sv_botKickMessages`, `sv_noAuthorize`, `sv_authorizeTimeout`, `sv_limitLocalRcon` ported. `sv_limitLocalRcon` defaults to 0 here (zk: 1) so LAN rcon tools keep working as before.
- Not ported: `sv_disconnectMessages` (voron00 already hides that message) and `sv_botReconnectMode` (bots come back after a map change since 2026-10-17; `sv_kickbots` is mode 1).
- `sv_botKickMessages` checked with a real client in the harness; the others are one-line conditions.

## 2026-10-24 — zk_libcod batch 2: callbacks

- Nine zk_libcod callbacks in `src/libcod/libcod_callbacks.cpp`, hooked with one call each in `BG_AddPredictableEventToPlayerstate`, `G_AddEvent`, `Scr_Notify`, `VM_Notify`, `G_MoverPush`, `G_CallSpawn`, `Scr_LoadGameType`, `Com_DPrintf` and the four rate limit sites. Notify arguments are kept as `VariableValue`s with `AddRefToValue`, which covers every type; zk copies a few types by hand.
- Event callbacks do not call themselves again (a callback that plays a sound would recurse); the NotifyDebug queue drops its references in `Libcod_ShutdownLevel`.
- Tested in the harness on 1.0 and 1.3, except `CodeCallback_MoverBlockage`/`g_noMoverBlockage` (no reproducible blockage on stock maps) and `CodeCallback_CLSpam` (rate limits skip LAN addresses, and the test only has local ones).

## 2026-10-24 — zk_libcod batch 1: precache limits

- `g_safePrecache`, `g_reservedModels`, `logErrors`, `error()` and the `execute*` aliases ported from zk_libcod. The stand-ins take model and effect index 1, set in `Libcod_SpawnServerConfigstrings` right after `SV_SpawnServer` empties the configstrings; the precache dvars are latched and registered again there.
- `logErrors` prints only the message: without developer the VM keeps no source positions (zk notes its lookup tables can be stale then).
- `make WITH_LIBCOD=false` shares `obj/` with the default build; mixing them fails to link until the folder is removed.

## 2026-10-24 — fprintln is stock

- `fprintln` writing each argument followed by `,` and no newline matches the stock game: OpenCoD2-Plus reversed the same function from the 1.3 client, in its `g_scr_main_mp.c`, with the same body. Gap closed, nothing changed.

## 2026-10-24 — graph node cap

- `sv_graphMaxNodes` defaults to 16384: generated graphs need 15014 nodes on `mp_brecourt` and 1377–3788 on the other stock maps (1.0 and 1.3 maps measured with no cap). Generation adds under a second to the first load only; a bots run on `mp_brecourt` showed no hitches after loading.
- Rejected: caching the graph across SD rounds; it is read back from a binary file in milliseconds, and a persistent graph would leak on a map change.

## 2026-10-24 — snapshot counters and the clock wrap

- Risk R3 closed. The seven snapshot counter checks in `SV_Frame` reload the map instead of `Com_Shutdown`: `SV_SpawnServer` already resets every counter, so players stay connected. A fast restart resets the archived and cached ones too, so round-based modes rarely get there.
- Audit of `Sys_Milliseconds` at day 24.8: every use compares differences, which wrap safely with `-fwrapv`; `Com_Frame` loses one frame (`com_lastFrameTime` is clamped). `getSystemMilliseconds` jumps back to 0 for scripts. No change needed.
- Checked by writing the counters through `/proc/<pid>/mem` with a client and bots connected, on 1.0 and 1.3.

## 2026-10-24 — time wrap restart between maps

- Risk R2 closed: `SV_Map_f` shuts the server down before loading the map once `svs.time` passes `0x60000000` (day 18.6); `SV_Frame`'s mid-match restart at `0x70000000` stays as the fallback. The snapshot counters were already reset on every map load, so only `svs.time` grows across maps.
- `SV_Shutdown` cleared `svs.redirectAddress` with the rest of `svs`, so an rcon command that shut the server down replied to an empty address; it is kept now.
- Checked by writing `svs.time` through `/proc/<pid>/mem` from a local harness test (the test process is the server's parent, so ptrace rules allow it), on 1.0 and 1.3.

## 2026-10-24 — full dvar pool

- Risk R4 closed: dvars named by a script, the console, rcon or a config check `Dvar_CanCreate` first, so a full pool (1280) is a script error or a printed message; engine registrations stay fatal, as in the binary.
- Checked with a local harness test that fills the pool from a script and then sets new names over rcon, on 1.0 and 1.3.

## 2026-10-24 — unsafe builtins behind a dvar

- Risk R5 closed ([ADR 0008](../architecture/adr/0008-unsafe-builtins-behind-a-dvar.md)): shell, raw memory, raw path and any-address builtins fail without `+set sv_libcodUnsafe 1`; `sqlite_open` is kept under `fs_homepath/<mod>`; HTTP refuses private addresses without `sv_httpAllowPrivate`. Off by default, so old mods need the dvar.
- Both dvars are read once at startup: a script's `setCvar` uses the internal source and ignores `DVAR_INIT`. `fread` and the other handle functions are gated too, since a forged handle is a raw pointer.
- Checked with `scripts/http_test.py` (now starts with `sv_httpAllowPrivate 1`) and a local harness test on 1.0 and 1.3.

## 2026-10-24 — sv_pure left as is

- Risk R1 closed as an inherent limit, not fixed: the client (`FS_ReferencedIwdPureChecksums`, read in OpenCoD2-Plus) sends only the iwds it has referenced, in a 1024-byte command, so requiring every server iwd drops legit clients, and a cheater with the stock files computes the same pure checksums from the `checksumFeed`.
- Rejected: a draft that required all checksums and removed the protocol 119 skip.

## 2026-10-24 — walkable normal as a float

- `MIN_WALK_NORMAL` was the double `0.69999999`; every site that uses it (mantle `0x512131`, prone `0x514863`/`0x5149ea`/`0x514be8`/`0x514eaa`, pmove `0x5172b6`/`0x519db7`, missile `0x521f18`) compares with the float `0.7` at `0x5c427c` in 1.3, as `WALKABLE_NORMAL_Z` already did in the collision code. Only a normal of exactly `0.7f` changes side.

## 2026-10-23 — eye bob

- `BG_GetSpeed` used the 3D velocity, so the shot origin (`G_GetPlayerViewOrigin`) bobbed while jumping or falling; 1.3's game (`0x4fde06`) and client (`0x4cfa44`) use x and y only. Found while porting the client's view in OpenCoD2-Plus, which had the same bug.
- `BG_GetBobCycle` divided as integers, so the server's eye never bobbed at all and the `BG_GetSpeed` bug hid behind it; 1.3 (`0x4fddb6`) divides by `255.0f`. OpenCoD2-Plus's server eye test checks both through libcod's `getViewOrigin` (the stock `getEye` is the origin plus 40).

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
