# Architecture notes

Normative summary. Details: [../architecture/boundaries.md](../architecture/boundaries.md).

## Must

- Keep reversed code identical to the binary except for the fix; explain the
  bug in the commit.
- Put script extensions in `src/libcod`, attached by `#ifdef LIBCOD` hooks;
  make hardening and admin fixes as small changes in the engine file.
- Build and check both `PROTOCOL_VERSION` 115 and 118, and both platforms.
- Bound every length read from the network.
- Keep per-client and per-entity libcod state in `libcodClients` and
  `libcodEntities`, so slot and entity resets clear it.
- Check `libcodLevelGeneration` before running an async callback.
- Keep zk_libcod names for ported script functions.
- Keep each file's line endings.

## Must not

- Change network formats, `MAX_CLIENTS`, `MAX_GENTITIES` or anything clients
  predict.
- Edit vendored libraries or add runtime dependencies.
- Run commands or reach the network from scripts without an admin dvar.
- Copy code from incompatibly licensed projects.
- Run a map command synchronously from inside a script.

## Irreversible or costly decisions

| Decision | ADR |
|---|---|
| One announced version per binary | [0001](../architecture/adr/0001-game-version-at-build-time.md) |
| Vendored, statically built libraries | [0002](../architecture/adr/0002-vendored-libraries.md) |
| zk_libcod names as the script API | [0003](../architecture/adr/0003-zk-libcod-names.md) |
| Upstream merges, no copying incompatible code | [0004](../architecture/adr/0004-upstream-and-references.md) |
| READMEs summarize, CHANGES.md details; README front page | [0005](../architecture/adr/0005-readme-and-changes.md), [0007](../architecture/adr/0007-readme-front-page.md) |
| Documentation as a continuity system | [0006](../architecture/adr/0006-ai-documentation-layer.md) |
