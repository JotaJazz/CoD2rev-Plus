# Upstream sync

Policy: [ADR 0004](../architecture/adr/0004-upstream-and-references.md).
Remote `upstream` is `voron00/CoD2rev_Server` (push disabled).

## Steps

1. `git fetch upstream`, then `git log --oneline $(git merge-base master upstream/master)..upstream/master`
   to list the new commits; read each diff against what the fork changed in
   the same files (`git log <base>..master -- <files>`).
2. Merge in a separate worktree:
   `git worktree add -b upstream-sync <folder> master`, then
   `git merge --no-commit --no-ff upstream/master`.
3. Conflicts: a conflict covering a whole file usually means only line endings
   differ (the fork keeps some files LF). Keep ours and re-apply upstream's
   real lines by hand; keep the file's endings.
4. Build Linux 1.0, 1.3 and Windows; run the smoke test for both versions.
   For changes to `src/xanim`, `src/bgame` or collision, also run the bots
   tests and an ASan soak ([testing.md](testing.md)): hit locations and
   movement depend on them.
5. Commit the merge with a body that lists what came in and how conflicts were
   resolved; fast-forward `master`; remove the worktree.
6. No `docs/CHANGES.md` entry (it lists what the fork adds), but update any
   document the merge made wrong (for example the README's repository layout)
   and add a stage to [../ai/PROJECT_MEMORY.md](../ai/PROJECT_MEMORY.md).
