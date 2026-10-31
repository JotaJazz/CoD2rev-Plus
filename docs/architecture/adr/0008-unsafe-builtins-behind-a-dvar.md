# 0008. Unsafe libcod builtins behind a startup dvar

- Status: Accepted
- Date: 2026-10-24

## Context

libcod gives scripts `system`, `exec`, raw memory access, file functions that
take any path and `sendPacket`, and `httpFetch` could reach any address. A
server owner installs mods written by others, so a mod could run commands,
read or replace any file the server user can, or reach services on the
server's own network.

## Decision

- The builtins that reach the shell, raw memory, files by raw path or any
  network address (`exec*`, `system`, `memory_*`, `binarybuffer_*`, `fopen`
  and the functions that take its handle, `file_link`, `file_unlink`,
  `file_exists`, `remove_file`/`fremove`, `scandir`, `fs_loaddir`/`loadDir`,
  `sendPacket`, `printOutOfBand`) stay registered, but a call is a script
  error unless the server starts with `+set sv_libcodUnsafe 1`.
- `sqlite_open` opens files under `fs_homepath/<mod>`, as `json_save` does;
  `:memory:` is kept. `ATTACH` and `VACUUM INTO` are refused, since SQL could
  otherwise write anywhere.

- `httpFetch` and `webSocketConnect` refuse loopback, private, link-local,
  shared and multicast addresses, checked after resolving the name, unless
  the server starts with `+set sv_httpAllowPrivate 1`.
- Both dvars are `DVAR_INIT` and read once at startup, because a script's
  `setCvar` sets any dvar.
- `executeCommand` and `processRemoteCommand` stay allowed: they run console
  commands, which scripts already reach through stock functions.

## Consequences

- Mods that use these functions need the dvar set by the server owner, who
  then trusts the mod.
- The functions keep their zk_libcod names, so enabling the dvar restores the
  old behaviour, except for `sqlite_open` paths.
- A new builtin that reaches the OS, memory or the network is wrapped with
  `gsc_unsafe<>` in `src/libcod/gsc.cpp`.
