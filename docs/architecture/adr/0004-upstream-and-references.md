# 0004. Pull from upstream, never copy incompatible code

- Status: Accepted
- Date: 2026-10-13

## Context

The reverse-engineering of the engine is done in
[voron00/CoD2rev_Server](https://github.com/voron00/CoD2rev_Server) (AGPL-3.0),
which keeps reviewing the reversed code. Other projects (xtnded/cod2,
CoD2x, openmohaa) solve similar problems under other licenses.

## Decision

- Upstream is merged into `master` when it has changes; nothing is sent back.
  Procedure: [../../development/upstream-sync.md](../../development/upstream-sync.md).
- Code is only taken from projects with a compatible license and credited
  (zk_libcod, MIT). xtnded/cod2 (GPLv2) is read for understanding only and
  never copied.
- Files this fork converted to LF stay LF, even if upstream uses CRLF.

## Consequences

- Upstream merges conflict on files both sides touched; whole-file conflicts
  usually mean only line endings differ.
- The fork drifts from upstream over time; changes to reversed code stay
  small to keep merges cheap ([../boundaries.md](../boundaries.md)).
