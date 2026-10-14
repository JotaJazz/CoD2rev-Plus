#!/usr/bin/env python3
"""Starts the Linux server against a real game folder and checks that it answers.

    python3 scripts/smoke_test.py /path/to/CoD2 [--protocol 118] [--binary bin/cod2rev_lnxded]

The game folder is only read: logs and configs go to a temporary home folder.
The server runs in LAN mode (dedicated 1), so nothing is sent to the master
servers. Exits with 0 when every check passes.
"""
import argparse, os, socket, subprocess, sys, tempfile, time

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
PREFIX = b'\xff\xff\xff\xff'


def query(port, command, timeout=2.0):
    """Sends a connectionless command and returns the reply as (header, info dict), or None."""
    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as s:
        s.settimeout(timeout)
        s.sendto(PREFIX + command.encode(), ('127.0.0.1', port))
        try:
            data, _ = s.recvfrom(65535)
        except OSError:
            return None
    lines = data[len(PREFIX):].decode('latin-1').split('\n')
    parts = lines[1].split('\\')[1:] if len(lines) > 1 else []
    return lines[0], dict(zip(parts[0::2], parts[1::2]))


def free_port():
    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as s:
        s.bind(('127.0.0.1', 0))
        return s.getsockname()[1]


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('game_dir', help='game folder that contains "main" with the .iwd files')
    ap.add_argument('--binary', default=os.path.join(ROOT, 'bin', 'cod2rev_lnxded'))
    ap.add_argument('--protocol', type=int, help='protocol the build must report (115 or 118)')
    ap.add_argument('--map', default='mp_toujane')
    ap.add_argument('--timeout', type=int, default=120, help='seconds to wait for the map to load')
    args = ap.parse_args()

    if not os.path.isdir(os.path.join(args.game_dir, 'main')):
        sys.exit(f'{args.game_dir} has no "main" folder')
    if not os.path.isfile(args.binary):
        sys.exit(f'{args.binary} not found, build it first with "make"')

    port = free_port()
    hostname = 'cod2rev smoke test'
    failures = []

    with tempfile.TemporaryDirectory(prefix='cod2rev_smoke_') as home:
        log_path = os.path.join(home, 'server.log')
        with open(log_path, 'wb') as log:
            server = subprocess.Popen(
                [args.binary,
                 '+set', 'fs_basepath', os.path.abspath(args.game_dir),
                 '+set', 'fs_homepath', home,
                 '+set', 'dedicated', '1',
                 '+set', 'net_port', str(port),
                 '+set', 'sv_cracked', '1',
                 '+set', 'sv_hostname', hostname,
                 '+set', 'g_gametype', 'tdm',
                 '+map', args.map],
                stdin=subprocess.DEVNULL, stdout=log, stderr=subprocess.STDOUT)
        try:
            info = None
            deadline = time.time() + args.timeout
            while time.time() < deadline and server.poll() is None:
                info = query(port, 'getinfo smoke')
                if info and info[1].get('mapname') == args.map:
                    break
                info = None
                time.sleep(1)

            def check(name, ok, detail=''):
                print(f'{"ok  " if ok else "FAIL"} {name}' + (f' ({detail})' if detail else ''))
                if not ok:
                    failures.append(name)

            check('server started and loaded the map', info is not None,
                  f'{args.map} on port {port}' if info else f'exit code {server.poll()}')
            if info:
                header, values = info
                check('getinfo reply', header == 'infoResponse' and values.get('challenge') == 'smoke')
                check('hostname', values.get('hostname') == hostname, values.get('hostname', ''))
                protocol = values.get('protocol', '')
                if args.protocol:
                    check('protocol', protocol == str(args.protocol), f'reports {protocol}')
                else:
                    print(f'     protocol {protocol}')

                status = query(port, 'getstatus smoke')
                check('getstatus reply', bool(status) and status[0] == 'statusResponse'
                      and status[1].get('mapname') == args.map,
                      f'version {status[1].get("shortversion", "?")}' if status else '')

                time.sleep(2)
                check('server still running', server.poll() is None)

            log_text = open(log_path, encoding='latin-1').read()
            check('no heartbeat sent in LAN mode', 'Sending heartbeat' not in log_text)
            if failures:
                print('\n--- last lines of the server log ---')
                print('\n'.join(log_text.splitlines()[-25:]))
        finally:
            if server.poll() is None:
                server.terminate()
                try:
                    server.wait(timeout=10)
                except subprocess.TimeoutExpired:
                    server.kill()

    print('\n' + ('FAILED: ' + ', '.join(failures) if failures else 'All checks passed'))
    return 1 if failures else 0


if __name__ == '__main__':
    sys.exit(main())
