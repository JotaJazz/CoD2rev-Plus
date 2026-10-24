# Architecture decision records

One file per decision, `NNNN-title.md`, MADR-lite. An accepted ADR is not
edited: a later ADR supersedes it, and only its Status line changes
(`Superseded by NNNN`). ADRs 0001–0005 record decisions taken before this
folder existed, with the date the decision was made.

| ADR | Decision | Status |
|---|---|---|
| [0001](0001-game-version-at-build-time.md) | Game version chosen at build time | Accepted |
| [0002](0002-vendored-libraries.md) | Libraries vendored and compiled into the binary | Accepted |
| [0003](0003-zk-libcod-names.md) | zk_libcod port keeps its names; what is left out | Accepted |
| [0004](0004-upstream-and-references.md) | Pull from upstream, never copy incompatible code | Accepted |
| [0005](0005-readme-and-changes.md) | READMEs summarize, `docs/CHANGES.md` details | Superseded by 0007 |
| [0006](0006-ai-documentation-layer.md) | Documentation as a continuity system | Accepted |
| [0007](0007-readme-front-page.md) | README front page | Accepted |
| [0008](0008-unsafe-builtins-behind-a-dvar.md) | Unsafe libcod builtins behind a startup dvar | Accepted |

## Template

```markdown
# NNNN. Title

- Status: Proposed | Accepted | Superseded by NNNN
- Date: YYYY-MM-DD

## Context

What forces the decision, in a few lines.

## Decision

What was decided.

## Consequences

What it makes easier, what it costs, what it rules out.
```
