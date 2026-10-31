# 0009. Linux 64-bit build next to the 32-bit one

- Status: Superseded by [0010](0010-64-bit-default-on-linux.md)
- Date: 2026-10-29

## Context

The fork forced `-m32` (`c07116c`) while upstream builds natively, so 64-bit
on x86-64 hosts, since 2026-09-30 (`bdbe7fc`). The 32-bit build needs
`gcc-multilib` and 32-bit libraries, the hardest part of installing on new
distributions and containers. A test build without `-m32` compiled and passed
the smoke, bots and HTTP tests.

## Decision

`make ARCH=x64` builds a 64-bit Linux binary with the same name; `ARCH=x86`
(32-bit) stays the default, so existing builds, scripts and CI artifacts do
not change. Math stays on x87 (`-mfpmath=387`) on both. Windows stays 32-bit.

## Consequences

- CI builds and publishes both Linux architectures.
- The 64-bit build gets less play testing; a crash report there shows `rip`.
- `memory_*` builtins return 64-bit addresses; mods written for 32-bit
  offsets need care.
- Making x64 the default, or a Windows x64 build, is a later decision.
