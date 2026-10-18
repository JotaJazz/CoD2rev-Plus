#!/bin/sh
# Starts the CoD2rev server using the game files in this folder.
# Usage: ./iniciar_cod2rev.sh [lan|internet]
#   lan       (default) LAN or direct IP only; accepts clients without a valid CD key
#   internet  also announces the server to the master servers
# The +set values come before +exec, so server.cfg can change them.
cd "$(dirname "$0")" || exit 1

PORT=28960
MODE=1
CRACKED="+set sv_cracked 1"
if [ "$1" = "internet" ]; then
	MODE=2
	CRACKED=
fi

exec ./cod2rev_lnxded +set dedicated "$MODE" +set net_port "$PORT" +set logfile 2 $CRACKED +exec server.cfg +map mp_toujane
