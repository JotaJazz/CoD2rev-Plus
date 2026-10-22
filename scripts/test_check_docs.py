#!/usr/bin/env python3
"""Tests for check_docs.py:

    python3 -m unittest discover -s scripts -p 'test_*.py'

The samples use placeholder names and documentation addresses only.
"""
import os, tempfile, textwrap, unittest

import check_docs


class CheckDocsTest(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.root = self.tmp.name
        self.write('src/server/sv_main_mp.cpp', '')
        self.write('scripts/smoke_test.py', '')

    def tearDown(self):
        self.tmp.cleanup()

    def write(self, rel, text):
        path = os.path.join(self.root, rel)
        os.makedirs(os.path.dirname(path), exist_ok=True)
        with open(path, 'w', encoding='utf-8') as f:
            f.write(textwrap.dedent(text))

    def check(self, rel, text):
        self.write(rel, text)
        return check_docs.check_file(self.root, rel)

    def messages(self, rel, text):
        return [m for _, m in self.check(rel, text)]

    # relative links

    def test_link_to_existing_file(self):
        self.write('docs/a.md', '')
        self.assertEqual(self.check('docs/b.md', '[a](a.md) [up](../scripts/smoke_test.py)\n'), [])

    def test_broken_link(self):
        found = self.check('docs/b.md', 'text\n[a](missing.md#part)\n')
        self.assertEqual(len(found), 1)
        self.assertEqual(found[0][0], 2)
        self.assertIn('missing.md', found[0][1])

    def test_link_anchor_is_ignored(self):
        self.write('docs/a.md', '')
        self.assertEqual(self.check('docs/b.md', '[a](a.md#section) [b](#local)\n'), [])

    def test_external_links_are_ignored(self):
        self.assertEqual(self.check('a.md', '[x](https://example.com/a.md) [m](mailto:a@example.com)\n'), [])

    def test_links_in_code_blocks_are_ignored(self):
        self.assertEqual(self.check('a.md', '```\n[a](missing.md)\n```\n'), [])

    # paths in backticks

    def test_existing_backtick_path(self):
        self.assertEqual(self.check('a.md', 'see `src/server/sv_main_mp.cpp` and `scripts/`\n'), [])

    def test_missing_backtick_path(self):
        found = self.messages('a.md', 'see `src/server/sv_gone.cpp`\n')
        self.assertEqual(len(found), 1)
        self.assertIn('src/server/sv_gone.cpp', found[0])

    def test_backtick_path_with_line_number(self):
        self.assertEqual(self.check('a.md', '`src/server/sv_main_mp.cpp:120`\n'), [])

    def test_backtick_placeholders_and_globs_are_ignored(self):
        text = '`src/libcod/*.cpp` `scripts/<name>.py` `src/{a,b}` `bin/cod2rev_lnxded` `obj/x`\n'
        self.assertEqual(self.check('a.md', text), [])

    def test_paths_relative_to_the_current_folder_are_ignored(self):
        self.assertEqual(self.check('a.md', '`./iniciar_cod2rev.sh` `../CoD2/main`\n'), [])

    def test_backtick_text_that_is_not_a_repo_path(self):
        self.assertEqual(self.check('a.md', '`maps/mp/bots/_bots.gsc` `a/b` `make PROTOCOL=118`\n'), [])

    def test_backtick_paths_in_code_blocks_are_checked(self):
        found = self.messages('a.md', '```\npython3 scripts/gone.py\n```\n')
        self.assertEqual(found, [])
        found = self.messages('a.md', '```\n`scripts/gone.py`\n```\n')
        self.assertEqual(len(found), 1)

    # personal data

    def test_home_and_mnt_paths(self):
        found = self.messages('a.md', 'in /home/alice/x and /mnt/e/Games\n')
        self.assertEqual(len(found), 2)

    def test_placeholder_paths_are_allowed(self):
        self.assertEqual(self.check('a.md', '/home/<user>/x /mnt/<drive>/Games <drive>:\\Games\n'), [])

    def test_windows_drive_path(self):
        found = self.messages('a.md', 'installed in D:\\Games\\CoD2\n')
        self.assertEqual(len(found), 1)

    def test_ip_addresses(self):
        found = self.messages('a.md', 'server at 10.20.30.40:28960\n')
        self.assertEqual(len(found), 1)

    def test_allowed_ip_addresses(self):
        text = '127.0.0.1 0.0.0.0 255.255.255.255 192.0.2.10 198.51.100.7 203.0.113.99\n'
        self.assertEqual(self.check('a.md', text), [])

    def test_version_numbers_are_not_ips(self):
        self.assertEqual(self.check('a.md', 'mbedTLS 3.6.7 and 1.2.3.4.5\n'), [])

    def test_keys(self):
        found = self.messages('a.md', '-----BEGIN RSA PRIVATE KEY-----\ntoken ghp_abcdefghijklmnopqrstuvwxyz0123456789\n')
        self.assertEqual(len(found), 2)

    # project memory

    MEMORY = 'docs/ai/PROJECT_MEMORY.md'

    def test_memory_stage_with_three_bullets(self):
        text = '# Memory\n\n## 2026-10-02 — b\n\n- 1\n- 2\n  - nested\n- 3\n\n## 2026-10-01 — a\n\n- 1\n'
        self.assertEqual(self.check(self.MEMORY, text), [])

    def test_memory_stage_with_four_bullets(self):
        found = self.check(self.MEMORY, '# Memory\n\n## 2026-10-01 — a\n\n- 1\n- 2\n- 3\n- 4\n')
        self.assertEqual(len(found), 1)
        self.assertEqual(found[0][0], 3)

    def test_memory_stage_heading_format(self):
        found = self.messages(self.MEMORY, '# Memory\n\n## October 1 — a\n\n- 1\n')
        self.assertEqual(len(found), 1)

    def test_memory_stages_newest_first(self):
        found = self.messages(self.MEMORY, '# Memory\n\n## 2026-10-01 — a\n\n- 1\n\n## 2026-10-02 — b\n\n- 1\n')
        self.assertEqual(len(found), 1)
        self.assertIn('newest', found[0])

    def test_bullet_rule_only_applies_to_memory(self):
        self.assertEqual(self.check('docs/other.md', '## 2026-10-01 — a\n\n- 1\n- 2\n- 3\n- 4\n'), [])

    # which files are scanned

    def test_doc_files_skip_local_and_vendored(self):
        for rel in ('README.md', 'AGENTS.md', 'docs/a.md', 'docs/superpowers/plan.md',
                    'CLAUDE.md', 'src/libcod/mbedtls/README.md', '.claude/x.md'):
            self.write(rel, '')
        self.assertEqual(check_docs.doc_files(self.root), ['AGENTS.md', 'README.md', 'docs/a.md'])


if __name__ == '__main__':
    unittest.main()
