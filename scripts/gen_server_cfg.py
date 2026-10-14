#!/usr/bin/env python3
"""Regenerates scripts/server.cfg.example from the dvars registered in src/.

Run it from anywhere after adding, removing or changing a dvar:

    python3 scripts/gen_server_cfg.py

The "Common settings" block and the descriptions are kept in this file; names,
types, defaults, ranges and flags are read from the Dvar_Register* calls.
"""
import glob, os, re, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OUTPUT = os.path.join(ROOT, 'scripts', 'server.cfg.example')


def read_dvars():
    pat = re.compile(r'Dvar_Register(Bool|Int|Float|String|Enum|Vec2|Vec3|Vec4|Color)\s*\(')
    out = []
    for path in sorted(glob.glob(os.path.join(ROOT, 'src/**/*.cpp'), recursive=True)):
        rel = os.path.relpath(path, ROOT).replace(os.sep, '/')
        # dvar.cpp and dvar_cmds.cpp define the register functions themselves
        if rel in ('src/universal/dvar.cpp', 'src/universal/dvar_cmds.cpp'):
            continue
        text = open(path, encoding='latin-1').read()
        for m in pat.finditer(text):
            i = m.end(); depth = 1; args = []; cur = ''; instr = False
            while depth and i < len(text):
                c = text[i]
                if instr:
                    cur += c
                    if c == '\\': cur += text[i+1]; i += 1
                    elif c == '"': instr = False
                elif c == '"': instr = True; cur += c
                elif c == '(': depth += 1; cur += c
                elif c == ')':
                    depth -= 1
                    if depth: cur += c
                elif c == ',' and depth == 1: args.append(cur.strip()); cur = ''
                else: cur += c
                i += 1
            args.append(cur.strip())
            args = [re.sub(r'\s+', ' ', a) for a in args]
            line = text.count('\n', 0, m.start()) + 1
            out.append(dict(type=m.group(1), file=rel, line=line, args=args))
    return out


d = read_dvars()
MAC = {'PROTOCOL_VERSION':'115','MAX_CLIENTS':'64','DEFAULT_GRAVITY':'800','PORT_SERVER':'28960','ENTITYNUM_NONE':'1023',
       'MAX_LANGUAGES - 1':'13','UINT16_MAX':'65535','0x7FFFFFFF':'2147483647',
       'MASTER_SERVER_NAME':'"cod2master.activision.com"','COMMUNITY_MASTER_SERVER_NAME':'"master.cod2x.me"',
       'GAMEVERSION':'"Call of Duty 2"'}
UNB = {'INT_MAX','INT_MIN','FLT_MAX','-FLT_MAX'}

COMMON = '''// --- Identity ---
set sv_hostname "My CoD2 server"
set sv_maxclients 20            // 1-64, takes effect on the next map load
set sv_privateClients 0         // slots reserved for sv_privatePassword
set sv_privatePassword ""
set g_password ""               // password to join; empty = public

// --- Remote console ---
// Leave rcon_password empty to keep rcon unusable.
set rcon_password ""
set sv_allowRcon 1              // 0 = ignore every rcon packet
set sv_rconMaxAttempts 5        // wrong passwords per IP before the lockout, 0 = no lockout
set sv_rconLockTime 600         // lockout length in seconds

// --- Master servers (only used with dedicated 2) ---
// sv_master1 defaults to cod2master.activision.com and sv_master2 to master.cod2x.me.
// sv_master2 is set here because the server saves it in config_mp_server.cfg, and a
// file saved by an older build can hold an empty value that overrides the default.
// sv_master3 to sv_master5 are free; host:port is accepted.
set sv_master2 "master.cod2x.me"
set sv_master3 ""
set sv_master4 ""
set sv_master5 ""

// --- Network ---
set sv_fps 20
set sv_maxRate 25000
set sv_minPing 0
set sv_maxPing 0                // 0 = no limit
set sv_timeout 240
set sv_floodProtect 1
set g_antilag 1

// --- Files and downloads ---
set sv_pure 1                   // clients must use the same .iwd files as the server
set sv_allowDownload 1
set sv_wwwDownload 0            // 1 = send clients to sv_wwwBaseURL for downloads
set sv_wwwBaseURL ""

// --- Game ---
set g_gametype "tdm"            // dm, tdm, sd, ctf, hq
set g_allowVote 0
set sv_kickBanTime 300          // seconds a kicked player stays banned
set sv_disableClientConsole 0

// --- Logging ---
set g_log "games_mp.log"
set g_logSync 0

// --- Map rotation ---
// The launcher starts the first map itself; the rotation is used from then on.
set sv_mapRotation "gametype tdm map mp_toujane gametype tdm map mp_carentan gametype tdm map mp_dawnville"
'''
common_names = set(re.findall(r'^set (\S+)', COMMON, re.M))

DESC = {
 'sv_zombietime':'seconds a disconnected client slot is kept before it is freed',
 'sv_reconnectlimit':'minimum seconds between reconnects from the same address',
 'sv_allowAnonymous':'allow clients flagged as anonymous',
 'sv_voice':'enable in-game voice chat',
 'sv_voiceQuality':'voice chat codec quality',
 'sv_showCommands':'print every client command to the console',
 'sv_padPackets':'add padding bytes to outgoing packets (debugging)',
 'sv_mapRotationCurrent':'remaining part of the rotation, managed by the server',
 'sv_keywords':'free text shown to server browsers',
 'sv_wwwDlDisconnected':'disconnect clients while they download over HTTP',
 'sv_downloadMessage':'if set, downloads are refused and this text is shown instead',
 'sv_fastDownload':'send in-game downloads faster',
 'sv_kickbots':'drop bots when the map restarts or changes',
 'jump_bounceEnable':'keep speed when landing on slopes (bouncing)',
 'sv_cracked':'accept clients whatever the key authorization server answers',
 'sv_master1':'first master server',
 'sv_master2':'second master server',
 'fs_library':'folder used by the libcod manymaps map download',
 'g_playerCollision':'players collide with each other',
 'g_playerEject':'push apart players stuck inside each other',
 'g_fixedWeaponSpreads':'use a fixed spread pattern instead of random spread',
 'g_dropGrenadeOnDeath':'a player killed while holding a primed grenade drops it',
 'g_speed':'player movement speed',
 'g_gravity':'world gravity',
 'g_knockback':'knockback applied by damage',
 'g_inactivity':'seconds before an inactive player is kicked, 0 = never',
 'g_motd':'message of the day',
 'g_banIPs':'list of banned addresses',
 'g_deadChat':'dead players can chat with living ones',
 'voice_deadChat':'dead players can voice chat with living ones',
 'voice_global':'voice chat is heard by both teams',
 'voice_localEcho':'players hear their own voice',
 'g_smoothClients':'smooth other players between snapshots',
 'g_synchronousClients':'force clients to wait for the server each frame (for demo recording)',
 'g_maxDroppedWeapons':'maximum dropped weapons kept on the map',
 'g_TeamName_Allies':'name shown for the allied team',
 'g_TeamName_Axis':'name shown for the axis team',
 'g_TeamColor_Allies':'colour of the allied team (red green blue alpha)',
 'g_TeamColor_Axis':'colour of the axis team (red green blue alpha)',
 'player_meleeRange':'reach of the melee attack',
 'jump_height':'jump height',
 'jump_slowdownEnable':'slow players down after a jump',
 'mantle_enable':'allow climbing over obstacles',
 'friction':'ground friction',
 'stopspeed':'speed below which a player stops',
 'bg_fallDamageMinHeight':'fall height at which damage starts',
 'bg_fallDamageMaxHeight':'fall height that kills',
 'player_breath_hold_time':'seconds a sniper can hold breath',
 'player_strafeSpeedScale':'speed multiplier when strafing',
 'player_backSpeedScale':'speed multiplier when moving backwards',
 'player_spectateSpeedScale':'speed multiplier for spectators',
 'dedicated':'1 = LAN or direct IP only, 2 = internet (announced to the master servers)',
 'sv_cheats':'allow cheat-protected variables and commands',
 'net_ip':'local address to bind to',
 'net_port':'UDP port',
 'net_noudp':'disable UDP networking',
 'net_lanauthorize':'also check the keys of clients on the local network',
 'showpackets':'print every packet',
 'showdrop':'print dropped packets',
 'fs_game':'mod folder',
 'fs_basepath':'folder where the game is installed',
 'fs_homepath':'folder where logs and downloaded files are written',
 'fs_cdpath':'extra read-only game folder',
 'fs_debug':'print file system activity',
 'fs_restrict':'restrict the server to demo content',
 'fs_ignoreLocalized':'ignore localized .iwd files',
 'developer':'developer mode: extra console output',
 'developer_script':'enable script debugging features',
 'logfile':'write the console to console_mp.log: 0 = off, 1 = buffered, 2 = flushed on every line',
 'com_maxfps':'frame rate cap',
 'com_hunkMegs':'megabytes of memory reserved for game data',
 'timescale':'game speed multiplier',
 'loc_language':'language index: 0 English, 1 French, 2 German, 3 Italian, 4 Spanish, 5 British, 6 Russian, 7 Polish',
 'loc_forceEnglish':'always use English strings',
 'ttycon':'enable the terminal console',
 'viewlog':'show the console window',
 'protocol':'network protocol of this build: 115 = 1.0, 118 = 1.3',
 'shortversion':'game version of this build',
 'version':'full version string of this build',
 'mapname':'map currently loaded',
 'sv_mapname':'map currently loaded',
 'nextmap':'command run when the map ends',
 'sv_running':'1 while a map is loaded',
 'sv_serverid':'changes on every map load',
 'sv_iwds':'checksums of the loaded .iwd files',
 'sv_iwdNames':'names of the loaded .iwd files',
 'sv_referencedIwds':'checksums of the .iwd files the map uses',
 'sv_referencedIwdNames':'names of the .iwd files the map uses',
 'gamename':'game name reported to browsers',
}

GROUPS = [
 ('Server', lambda f: f.startswith('src/server/sv_init')),
 ('libcod extensions', lambda f: 'libcod' in f),
 ('Game rules', lambda f: f.startswith('src/game/')),
 ('Player movement and physics', lambda f: f.startswith('src/bgame/')),
 ('Network', lambda f: 'net_chan' in f or f.endswith('_net.cpp')),
 ('File system', lambda f: 'com_files' in f or 'sv_client' in f),
 ('Engine', lambda f: f.startswith('src/qcommon/') or 'com_memory' in f or 'stringed' in f),
 ('Platform', lambda f: f.startswith('src/unix/') or f.startswith('src/win32/')),
]

seen = {}
for x in d:
    n = x['args'][0]
    if not n.startswith('"'): continue
    n = n.strip('"')
    seen.setdefault(n, []).append(x)
# the engine registers some dvars before the game does; its registration is the one that counts
for xs in seen.values():
    xs.sort(key=lambda x: x['file'] != 'src/qcommon/common.cpp')

def val(v):
    v = MAC.get(v, v)
    if v in ('true','qtrue'): return '1'
    if v in ('false','qfalse'): return '0'
    if re.fullmatch(r'-?[0-9.]+f?', v): return v.rstrip('f') if '.' in v else v
    if v.startswith('"'): return v
    return None

def rng(lo, hi, kind):
    lo = MAC.get(lo, lo); hi = MAC.get(hi, hi)
    lo = lo.rstrip('f') if '.' in lo else lo
    hi = hi.rstrip('f') if '.' in hi else hi
    if lo in UNB and hi in UNB: return kind
    if lo == hi: return kind
    if hi in UNB: return f'{kind}, {lo} or more'
    if lo in UNB: return f'{kind}, {hi} or less'
    return f'{kind}, {lo} to {hi}'

def entry(n):
    xs = seen[n]; x = xs[0]; a = x['args']; t = x['type']; flags = a[-1]
    plats = {('Windows' if e['file'].startswith('src/win32/') else 'Linux' if e['file'].startswith('src/unix/') else 'all') for e in xs}
    if t == 'Bool': v, info = val(a[1]), 'bool'
    elif t == 'Int': v, info = val(a[1]), rng(a[2], a[3], 'int')
    elif t == 'Float': v, info = val(a[1]), rng(a[2], a[3], 'float')
    elif t == 'String': v, info = val(a[1]), 'string'
    elif t == 'Color': v, info = '"' + ' '.join(val(c) for c in a[1:5]) + '"', 'colour'
    else: raise SystemExit('unhandled type ' + t)
    tags = []
    if 'DVAR_ROM' in flags: tags.append('read-only')
    if 'DVAR_INIT' in flags: tags.append('command line only')
    if 'DVAR_CHEAT' in flags: tags.append('needs sv_cheats 1')
    if 'DVAR_LATCH' in flags: tags.append('applies on the next map or restart')
    if plats == {'Windows'}: tags.append('Windows only')
    if plats == {'Linux'}: tags.append('Linux only')
    note = info + (', ' + ', '.join(tags) if tags else '')
    if n in DESC: note += ': ' + DESC[n]
    if 'DVAR_ROM' in flags:
        left = f'// {n}' + (f' = {v}' if v is not None and v != '""' else '')
    else:
        left = f'// set {n} ' + (v if v is not None else '<set at startup>')
    return left.ljust(46) + ' // ' + note

out = []
out.append('''// Example server configuration for CoD2rev_Server.
// Copy it to the "main" folder of the game as server.cfg and edit it.
// Do not commit your real server.cfg: it holds passwords.
//
// The first part sets the variables most servers change.
// The second part lists every other variable the server registers, commented
// out and with its default value, so copying this file changes nothing else.
// Remove the leading "//" to use one.
//
// Not listed here: the scr_* variables of each gametype, which are defined by
// the game scripts inside the .iwd files and not by the server.

// =============================================================================
// Common settings
// =============================================================================

''')
out.append(COMMON)
out.append('''
// =============================================================================
// Every other variable (defaults shown)
// =============================================================================
// Notes after each variable:
//   read-only             set by the server, cannot be changed
//   command line only     must be given as "+set name value" when starting
//   needs sv_cheats 1     ignored unless cheats are enabled
''')
listed = set()
for title, pred in GROUPS:
    names = [n for n in seen if n not in common_names and n not in listed and pred(seen[n][0]['file'])]
    listed.update(names)
    out.append(f'\n// --- {title} ---\n')
    for n in names: out.append(entry(n) + '\n')
missing = [n for n in seen if n not in common_names and n not in listed]
assert not missing, missing
unknown = common_names - set(seen)
assert not unknown, unknown
open(OUTPUT, 'w', encoding='utf-8', newline='\n').write(''.join(out))
print(f'{OUTPUT}: {len(seen)} dvars, {len(common_names)} set, {len(listed)} listed with their defaults')
