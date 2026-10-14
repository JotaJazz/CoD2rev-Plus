#!/bin/sh
# Starts the CoD2rev server using the game files in this folder.
# Usage: ./iniciar_cod2rev.sh [lan|internet]
#   lan       (default) LAN or direct IP only
#   internet  also announces the server to the master servers
cd "$(dirname "$0")" || exit 1

MODE=1
[ "$1" = "internet" ] && MODE=2

exec ./cod2rev_lnxded +set dedicated "$MODE" +set net_port 28960 +exec server.cfg +set logfile 2 +set sv_cracked 1 +map mp_toujane
