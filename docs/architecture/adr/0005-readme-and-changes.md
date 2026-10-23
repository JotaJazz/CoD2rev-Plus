# 0005. READMEs summarize, docs/CHANGES.md details

- Status: Superseded by 0007
- Date: 2026-10-17

## Context

The README exists in six languages (`README.md` plus `.es .pt .fr .de .ru`).
Keeping a detailed change list in six languages made every change six edits,
and the translations drifted.

## Decision

The READMEs' "Changes in this fork" has one line per milestone. Details
(functions, variables, fixes) go in `docs/CHANGES.md`, in English only.
English is the main README.

## Consequences

- A user-visible change adds a `docs/CHANGES.md` entry; the six READMEs change
  only when a milestone summary changes, and then all six together.
- `docs/CHANGES.md` records what the fork adds, not upstream merges.
