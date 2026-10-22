# 0006. Documentation as a continuity system

- Status: Accepted
- Date: 2026-10-22

## Context

Most work is done with an AI assistant across many sessions. The project map,
conventions, engine facts and open leads lived in a local, uncommitted
`CLAUDE.md` and in the assistant's private memory, so they were lost to any
other tool or contributor.

## Decision

- `AGENTS.md` (committed) is the entry point for agents; a local `CLAUDE.md`
  may only point to it and add machine-specific paths.
- `docs/architecture`, `docs/development`, `docs/roadmap` and `docs/glossary.md`
  hold the stable technical context; `docs/ai/` holds the AI memory layer,
  with a live snapshot (`SYSTEM_SUMMARY.md`) and an append-only memory
  (`PROJECT_MEMORY.md`).
- `scripts/check_docs.py` checks links, paths, personal data and memory
  stages. It runs locally with the tests, not in CI, so documentation edits
  do not start builds.
- Documentation is in English, like the code and `docs/CHANGES.md`.

## Consequences

- A change that makes a document wrong updates it in the same commit
  ([../../ai/DEVELOPMENT_GUIDE.md](../../ai/DEVELOPMENT_GUIDE.md)).
- Personal paths (game folders, home folders) stay in the local `CLAUDE.md`.
