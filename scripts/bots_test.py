#!/usr/bin/env python3
"""Runs the bots mod on a real map and checks that the bots navigate and fight.

    python3 scripts/bots_test.py /path/to/CoD2 [--map mp_toujane] [--seconds 180]

The mod is installed in a temporary folder with bots_debug 1, so it writes
BOTNAV and BOTCOMBAT lines to games_mp.log. The map is loaded twice: the first
load must build and save the waypoint graph, the second must read it back.
Exits with 0 when every check passes.
"""
import argparse, os, re, secrets, subprocess, sys, tempfile, time

from smoke_test import free_port, query, rcon

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
MOD = 'bottest'
ERROR_CALLBACK = '''
CodeCallback_Error(critical, function, message)
{
	logprint("BOTERROR " + function + " " + message + "\\n");
}
'''


def log_lines(path):
    if not os.path.exists(path):
        return []
    # each line starts with the game time, as in " 1:23 K;..."
    return [re.sub(r'^\s*\d+:\d+ ', '', l) for l in open(path, encoding='latin-1').read().splitlines()]


def wait_lines(path, pattern, count, seconds):
    deadline = time.time() + seconds
    while time.time() < deadline:
        if sum(1 for l in log_lines(path) if re.search(pattern, l)) >= count:
            return True
        time.sleep(1)
    return False


def main():
    ap = argparse.ArgumentParser(description=__doc__.split('\n')[0])
    ap.add_argument('game_dir')
    ap.add_argument('--binary', default=os.path.join(ROOT, 'bin', 'cod2rev_lnxded'))
    ap.add_argument('--map', default='mp_toujane')
    ap.add_argument('--gametype', default='tdm')
    ap.add_argument('--seconds', type=int, default=180, help='play time on the first load')
    ap.add_argument('--corrupt-cache', action='store_true',
                    help='start with a garbage waypoint file, which must be rebuilt')
    args = ap.parse_args()

    failures = []

    def check(name, ok, detail=''):
        print(f'{"ok  " if ok else "FAIL"} {name}' + (f' ({detail})' if detail else ''))
        if not ok:
            failures.append(name)

    with tempfile.TemporaryDirectory(prefix='cod2rev_bots_') as home:
        subprocess.run([sys.executable, os.path.join(ROOT, 'scripts', 'install_bots.py'), args.game_dir,
                        '--dest', home, '--name', MOD], check=True, stdout=subprocess.DEVNULL)
        with open(os.path.join(home, MOD, 'maps', 'mp', 'gametypes', '_callbacksetup.gsc'), 'a',
                  encoding='latin-1') as f:
            f.write(ERROR_CALLBACK)

        cache = os.path.join(home, MOD, 'waypoints', args.map + '_v1.graph')
        if args.corrupt_cache:
            os.makedirs(os.path.dirname(cache))
            with open(cache, 'wb') as f:
                f.write(b'not a graph file' * 4)

        game_log = os.path.join(home, MOD, 'games_mp.log')
        port = free_port()
        password = secrets.token_hex(8)
        with open(os.path.join(home, 'server.log'), 'wb') as out:
            server = subprocess.Popen(
                [args.binary, '+set', 'fs_basepath', os.path.abspath(args.game_dir), '+set', 'fs_homepath', home,
                 '+set', 'fs_game', MOD, '+set', 'dedicated', '1', '+set', 'net_port', str(port),
                 '+set', 'sv_cracked', '1', '+set', 'rcon_password', password,
                 '+set', 'g_gametype', args.gametype, '+set', f'scr_{args.gametype}_timelimit', '0',
                 '+set', f'scr_{args.gametype}_scorelimit', '0', '+set', 'g_logSync', '1',
                 '+set', 'sv_maxclients', '8', '+set', 'bots_count', '6', '+set', 'bots_debug', '1',
                 '+map', args.map],
                stdin=subprocess.DEVNULL, stdout=out, stderr=subprocess.STDOUT)
        try:
            first = wait_lines(game_log, r'^BOTNAV graph ', 1, 120)
            check('graph ready on the first load', first)
            if first:
                time.sleep(args.seconds)
                rcon(port, password, 'map ' + args.map)
                check('graph ready on the second load', wait_lines(game_log, r'^BOTNAV graph ', 2, 120))
                time.sleep(15)
            check('server still running', server.poll() is None and query(port, 'getinfo') is not None)
        finally:
            if server.poll() is None:
                server.terminate()
                try:
                    server.wait(timeout=10)
                except subprocess.TimeoutExpired:
                    server.kill()

        lines = log_lines(game_log)
        graphs = [l for l in lines if l.startswith('BOTNAV graph ')]
        check('first load generates and saves the graph', len(graphs) >= 1 and graphs[0].startswith('BOTNAV graph generated')
              and os.path.exists(cache), ', '.join(graphs))
        check('second load reads the saved graph', len(graphs) >= 2 and graphs[1].startswith('BOTNAV graph loaded'))

        nodes = [l for l in lines if re.match(r'BOTNAV \d+ node ', l)]
        bots = {l.split()[1] for l in nodes}
        check('bots follow paths', len(nodes) >= 60 and len(bots) >= 3, f'{len(nodes)} nodes by {len(bots)} bots')
        goals = {l.split()[3] for l in lines if re.match(r'BOTNAV \d+ goal ', l)}
        check('bots choose patrol and hint goals', {'patrol', 'hint'} <= goals, ', '.join(sorted(goals)))
        stuck = sum(1 for l in lines if re.match(r'BOTNAV \d+ stuck ', l))
        print(f'     {stuck} stuck events')

        combat = {l.split()[2] for l in lines if re.match(r'BOTCOMBAT \d+ ', l)}
        for action in ('ads', 'nade', 'melee'):
            check(f'bots use {action}', action in combat)
        kills = [l for l in lines if l.startswith('K;')]
        check('bots kill each other', len(kills) >= 5, f'{len(kills)} kills')
        nade_kills = sum(1 for l in kills if 'MOD_GRENADE_SPLASH' in l)
        melee_kills = sum(1 for l in kills if 'MOD_MELEE' in l)
        # K;victimguid;victimnum;team;name;attackerguid;attackernum;...
        self_nades = sum(1 for l in kills if 'MOD_GRENADE_SPLASH' in l and l.split(';')[2] == l.split(';')[6])
        print(f'     {nade_kills} grenade kills, {melee_kills} melee kills')
        check('few grenade suicides', self_nades <= 2, f'{self_nades}')

        errors = [l for l in lines if l.startswith('BOTERROR')]
        allowed = 1 if args.corrupt_cache else 0
        check('no script errors', len(errors) <= allowed, '; '.join(errors[:3]))

    print('\n' + ('FAILED: ' + ', '.join(failures) if failures else 'All checks passed'))
    return 1 if failures else 0


if __name__ == '__main__':
    sys.exit(main())
