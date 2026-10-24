# Building

User-facing build instructions are in the [README](../../README.md#building).
This page covers what a contributor needs on top.

## Variants

| Command | Output |
|---|---|
| `make` | Linux 32-bit, 1.0 (protocol 115) → `bin/cod2rev_lnxded` |
| `make PROTOCOL=118` | Linux 32-bit, 1.3 → same name in `bin/` |
| `make windows` | MinGW cross-build → `bin/cod2rev_win32.exe` (`PROTOCOL=118` for 1.3) |
| `make BIN_DIR=<folder>` | Same build into another folder, to keep 1.0 and 1.3 side by side |
| `make clean` | Removes the binary and the objects |

- Objects go to `obj/<binary>_<protocol>`, so switching platform or version
  does not rebuild everything; the binary name in `bin/` is shared.
- Linux needs `gcc-multilib` and `g++-multilib`; Windows needs the posix
  thread variant of MinGW-w64 (`i686-w64-mingw32-gcc-posix`; Debian/Ubuntu
  package `gcc-mingw-w64-i686-posix`).
- `-D LIBCOD` and the `LIBCOD_COMPILE_*` switches are on by default (see the
  README's Makefile options). Code differs between `#ifdef LIBCOD` and the
  stock path, and between `PROTOCOL_VERSION` 115 and 118: check both sides of
  an `#if`. `make WITH_LIBCOD=false` uses the same `obj/` folder, so delete
  `obj/cod2rev_lnxded_115` before and after it, or stale objects fail to link.
- Hardening flags are on in every Makefile build: `-fstack-protector-strong`,
  `-D_FORTIFY_SOURCE=2`, `-fno-strict-aliasing`, `-fwrapv`, full RELRO on
  Linux. The reversed code relies on `-fno-strict-aliasing` and `-fwrapv`. A
  command-line `CFLAGS`/`LFLAGS`, as in the ASan build below, replaces them.
- Known warnings: four (`-Wwrite-strings`, `-Wmultichar`) in reversed files;
  new code adds none.

## Before committing a change under src/

1. `make` and `make PROTOCOL=118`.
2. `make windows`.
3. The smoke test for both versions ([testing.md](testing.md)).
4. For memory-sensitive changes, an ASan run with bots.

CI (`.github/workflows/build.yml`) builds the four variants on pushes to
`master` and on pull requests that change `src/`, the `Makefile` or the
workflow, and on a manual run (`workflow_dispatch`).

## AddressSanitizer build

A command-line `CFLAGS` replaces the Makefile's, so `-m32` and
`-D_FILE_OFFSET_BITS=64` must be repeated:

```bash
make OBJ_ROOT=<scratch>/asan_obj BIN_DIR=<scratch>/asan_bin \
  CFLAGS="-fno-pie -O1 -g -fno-omit-frame-pointer -mfpmath=387 -DNDEBUG -DPROTOCOL_VERSION=115 -MMD -MP -m32 -D_FILE_OFFSET_BITS=64 -fsanitize=address" \
  LFLAGS="-no-pie -m32 -fsanitize=address"
```

Use `-DPROTOCOL_VERSION=118` and other folders for 1.3.

## Line endings

`.gitattributes` is `* -text`: git never converts line endings. Most files
under `src/` are CRLF; `src/libcod/*`, `src/qcommon/msg_mp.cpp`,
`src/qcommon/qcommon.h`, the scripts and the docs are LF. Keep each file's
endings (`grep -c $'\r' <file>` counts CR lines). An editor that converts a
file makes the whole file a diff and a conflict with upstream.

## Code style

Quake 3 style in `src/`: tabs, braces on their own line, and a comment block
with the function name between `=====` lines above each function. Comments only where the reason is not
obvious. Python scripts: 4 spaces, standard library only, a module
docstring with the usage line.
