#!/usr/bin/env python3
"""Checks the repository's Markdown documentation.

    python3 scripts/check_docs.py

It reports, as file:line: message,
  - relative links to files that do not exist,
  - repository paths in backticks that do not exist,
  - personal data: /home and /mnt paths, Windows drive paths, IP addresses and keys
    (placeholders such as /home/<user> and <drive>:\\ are allowed, and so are
    loopback and documentation addresses),
  - stages of docs/ai/PROJECT_MEMORY.md with more than 3 bullets, a heading that
    is not "## YYYY-MM-DD — topic", or that are not newest first.

Local files are skipped: CLAUDE.md, docs/superpowers, .claude, .superpowers,
and the vendored code under src. Tests: scripts/test_check_docs.py.
"""
import os, re, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SKIP_DIRS = {'.git', '.claude', '.superpowers', 'docs/superpowers', 'src', 'obj', 'bin'}
SKIP_FILES = {'CLAUDE.md'}
MEMORY = 'docs/ai/PROJECT_MEMORY.md'
# build output, which a fresh clone does not have
BUILD_PREFIXES = ('bin/', 'obj/')
MAX_STAGE_BULLETS = 3

LINK = re.compile(r'\[[^\]]*\]\(([^)\s]+)(?:\s+"[^"]*")?\)')
CODE = re.compile(r'`([^`\n]+)`')
PERSONAL = [
    (re.compile(r'/home/(?!<)[\w.-]+'), 'home folder path'),
    (re.compile(r'/mnt/(?!<)[\w.-]+'), 'mount path'),
    (re.compile(r'(?<![\w<>])[A-Za-z]:\\\\?[\w]'), 'Windows drive path'),
    (re.compile(r'-----BEGIN [A-Z ]*PRIVATE KEY-----'), 'private key'),
    (re.compile(r'\b(?:ghp|gho|ghs|github_pat)_[A-Za-z0-9_]{20,}'), 'GitHub token'),
    (re.compile(r'\bglpat-[A-Za-z0-9_-]{20,}'), 'GitLab token'),
    (re.compile(r'\bAKIA[0-9A-Z]{16}\b'), 'AWS key'),
]
IP = re.compile(r'(?<![\d.])(\d{1,3})\.(\d{1,3})\.(\d{1,3})\.(\d{1,3})(?![\d.])')
ALLOWED_IPS = {'0.0.0.0', '255.255.255.255'}
# loopback and the documentation ranges of RFC 5737
ALLOWED_IP_PREFIXES = ('127.', '192.0.2.', '198.51.100.', '203.0.113.')
STAGE = re.compile(r'^## (\d{4}-\d{2}-\d{2}) — \S')


def doc_files(root):
    found = []
    for dirpath, dirnames, filenames in os.walk(root):
        rel_dir = os.path.relpath(dirpath, root).replace(os.sep, '/')
        rel_dir = '' if rel_dir == '.' else rel_dir + '/'
        dirnames[:] = [d for d in dirnames if rel_dir + d not in SKIP_DIRS]
        for name in filenames:
            rel = rel_dir + name
            if name.endswith('.md') and rel not in SKIP_FILES:
                found.append(rel)
    return sorted(found)


def is_repo_path(root, token):
    if '/' not in token or token.startswith(BUILD_PREFIXES + ('./', '../')):
        return False
    if re.search(r'[\s*<>{}$?=~|]', token):
        return False
    first = token.split('/', 1)[0]
    return first != '' and os.path.exists(os.path.join(root, first))


def check_links(root, rel, lineno, line):
    found = []
    base = os.path.dirname(os.path.join(root, rel))
    for target in LINK.findall(CODE.sub('', line)):
        if re.match(r'[a-z][a-z0-9+.-]*:', target) or target.startswith('#'):
            continue
        path = target.split('#', 1)[0]
        if not os.path.exists(os.path.normpath(os.path.join(base, path))):
            found.append((lineno, f'broken link: {target}'))
    return found


def check_paths(root, lineno, line):
    found = []
    for token in CODE.findall(line):
        path = re.sub(r':\d+(?:-\d+)?$', '', token.strip())
        if is_repo_path(root, path) and not os.path.exists(os.path.join(root, path.rstrip('/'))):
            found.append((lineno, f'path does not exist: {path}'))
    return found


def check_personal(lineno, line):
    found = [(lineno, f'personal data ({what}): {m.group(0)}')
        for pattern, what in PERSONAL for m in pattern.finditer(line)]
    for m in IP.finditer(line):
        ip = m.group(0)
        if all(int(part) <= 255 for part in m.groups()) and ip not in ALLOWED_IPS \
                and not ip.startswith(ALLOWED_IP_PREFIXES):
            found.append((lineno, f'personal data (IP address): {ip}'))
    return found


def check_memory(lines):
    found = []
    stage, bullets, previous = None, 0, None

    def close():
        if stage is not None and bullets > MAX_STAGE_BULLETS:
            found.append((stage, f'memory stage has {bullets} bullets (at most {MAX_STAGE_BULLETS})'))

    for lineno, line in enumerate(lines, 1):
        if line.startswith('## '):
            close()
            stage, bullets = lineno, 0
            m = STAGE.match(line)
            if not m:
                found.append((lineno, 'memory stage heading must be "## YYYY-MM-DD — topic"'))
                continue
            if previous is not None and m.group(1) > previous:
                found.append((lineno, 'memory stages must be newest first'))
            previous = m.group(1)
        elif stage is not None and line.startswith('- '):
            bullets += 1
    close()
    return found


def check_file(root, rel):
    with open(os.path.join(root, rel), encoding='utf-8') as f:
        lines = f.read().splitlines()
    found = []
    in_code = False
    for lineno, line in enumerate(lines, 1):
        if line.lstrip().startswith('```'):
            in_code = not in_code
            continue
        if not in_code:
            found += check_links(root, rel, lineno, line)
        found += check_paths(root, lineno, line)
        found += check_personal(lineno, line)
    if rel == MEMORY:
        found += check_memory(lines)
    return sorted(found)


def main():
    problems = 0
    for rel in doc_files(ROOT):
        for lineno, message in check_file(ROOT, rel):
            print(f'{rel}:{lineno}: {message}')
            problems += 1
    if problems:
        sys.exit(f'{problems} problem(s) found')
    print('Documentation OK')


if __name__ == '__main__':
    main()
