# 0003. zk_libcod port keeps its names; what is left out

- Status: Accepted
- Date: 2026-10-16

## Context

[zk_libcod](https://github.com/ibuddieat/zk_libcod) (MIT) offers many GSC
functions and callbacks, implemented as `LD_PRELOAD` hooks on the stock 1.3
binary. Server mods are written against its names.

## Decision

Port its behaviour into our functions (reading its `custom_X`/`hook_X` code
in `code/libcod.cpp`), keeping function names, arguments and callbacks, so
scripts written for it run here. Fix its bugs on the way and note them in
`docs/CHANGES.md`.

Left out on purpose:

| Left out | Why |
|---|---|
| Client-predicted movement setters (jump height, step size, weapon move speed, `setHoldingWeaponDown`, `setRechambering`) | Desync client prediction |
| `executeClientCommand` | Runs commands on players' machines |
| `setPing` | Lets bots pose as players |
| Manual snapshot entity list | zk's version breaks delta compression |
| Talker icons | Need fake voice packets |
| Bullet visual models, `useEntity`, `getCallStack`, CoD2x-only functions | Low value or need the CoD2x client |
| MySQL async, soundalias functions (`getSoundDuration`, `getSoundInfo`, `getSoundAliasesFromFile`) | Deferred, not rejected |

## Consequences

- New functions from zk_libcod are added under the same name, never renamed.
- Functions we add that zk_libcod lacks must not reuse a name it might add.
- The left-out list is the answer to "why is X missing".
