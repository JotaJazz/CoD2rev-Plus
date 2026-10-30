# Debt, risks and open leads

What is known to be missing or fragile, with what a fix needs. Remove an
entry in the commit that resolves it and record the stage in
[../ai/PROJECT_MEMORY.md](../ai/PROJECT_MEMORY.md).

## Risks

None open. R1 became a known gap; R2–R5 were fixed on 2026-10-24
(see [../ai/PROJECT_MEMORY.md](../ai/PROJECT_MEMORY.md)).

## Known gaps

| Gap | Where | Notes |
|---|---|---|
| `sv_pure` only proves the client has the server's iwds, not that it uses no others: the client reports only the iwds it has read from (`FS_ReferencedIwdPureChecksums`), so the server cannot require them all, and anyone with the stock files can compute the pure checksums from the `checksumFeed` in the gamestate; protocol 119 skips the check | `src/server/sv_client_mp.cpp` | Inherent to the Quake 3 design; a stricter check drops legit clients |
| zPAM 4.x needs the CoD2x API (`getHWID`, `match*`, `g_cod2x` ≥ 5) | — | zPAM 3.36 runs on 1.0 and 1.3 |
| Protocol 119 fragment offset format | `src/qcommon/net_chan_mp.cpp` | Needs a Game Pass client |
| Script runtime errors cascade outside developer mode | `src/script/scr_vm.cpp` | Stock behaviour |

## Bots mod

| Item | Notes |
|---|---|
| Rare cascade of script errors when a bot joins a freed slot during a grenade kill (about 1 in 60 runs) | Not located; rerun with `+set developer 2` so `CodeCallback_Error` gets file and line |
| Multi-floor cells in the graph builder | 1–2 days of work |
| HQ objectives not played | Only dm, tdm, sd, ctf have roles |
| Bullet antilag rewinds are not grouped per frame | Performance only |

## Deferred decisions

| Lead | Notes |
|---|---|
| Second listening port answering as 1.3, so one process is listed for 1.0 and 1.3 | zk_libcod's `code/proxy/proxy.c` (BSD) is a base; smaller step: do not treat `127.0.0.1` clients with an `ip` key as LAN |
| Stats mod (kills with positions into SQLite) | Compatible with Lonsofore/CoD2MapStats |
| MySQL on Windows | The Makefile's Windows branch names a `libmysql.lib` that is not vendored; the 64-bit Linux build works ([../development/building.md](../development/building.md#mysql)) |
| soundalias functions | See [ADR 0003](../architecture/adr/0003-zk-libcod-names.md) |
| CPU profile with 20 bots | Not measured yet |
| Windows x64 build | Windows is LLP64 (`long` stays 32-bit) and upstream's script VM assumes LP64: GSC int64 values are a `long int` (`src/script/script_public.h`, `Scr_AddInt64`/`Scr_GetInt64`, which also carry libcod's handles) and the bytecode is read and written with `sizeof(unsigned long)` (`Scr_ReadUnsigned`, `EmitUnsigned`, `SWITCH_CASE_ENTRY_SIZE`). About 60 `long`s in reversed files would become `int64_t`/`intptr_t`, a conflict on every upstream merge; the 32-bit `.exe` already runs on 64-bit Windows. Needs `gcc-mingw-w64-x86-64-posix` ([ADR 0009](../architecture/adr/0009-linux-64-bit-build.md)) |
| SSE math (`-msse2 -mfpmath=sse -DUSE_SSE2`, upstream since `8319e9b`/`672a220`) | Kept x87 to stay with the original binary's rounding; `USE_SSE2` also swaps `FastSinCos` (animation, hit bones, weapons) for the `sse_mathfun.h` approximation. Adopting it needs bots and ASan runs and a look at hit locations |
| Releases page | The READMEs' "How to run" points to it; CI already builds the six binaries as artifacts |

## Accepted simplifications

- `playFxOnTagForPlayer` filters spectators by the followed player's
  `clientNum`: kept on purpose.
- One announced version per binary ([ADR 0001](../architecture/adr/0001-game-version-at-build-time.md)).
