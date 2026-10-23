# 0007. README front page

- Status: Accepted
- Date: 2026-10-22

## Context

The README opened with the Transparency notes and read as a build manual,
while most visitors want to know what the server is for and how to run it.
OpenCoD2-Plus, the maintainer's client project, moved to a front page that
starts from that.

## Decision

The README (in all six languages) is a front page, in this order: a short
pitch, "What it does today", a note and a no-game-files warning, "How to
run", game versions, "For contributors", then building, running, testing and
the repository layout folded in `<details>`, and Transparency, Notice,
License and Credits at the end.

ADR 0005's split stays: "What it does today" has one line per milestone,
details go in `docs/CHANGES.md`, in English only, and the six READMEs change
together.

## Consequences

- The milestone section's anchor is `#what-it-does-today`.
- "How to run" points to a Releases page that does not exist yet; until it
  does, it tells users to build.
- The Notice section states the trademark and game-data position, as in
  OpenCoD2-Plus.
