@echo off
rem Starts the CoD2rev server using the game files in this folder.
rem Usage: iniciar_cod2rev.bat [lan|internet]
rem   lan       (default) LAN or direct IP only
rem   internet  also announces the server to the master servers
rem Port 28970 avoids clashing with the CoD2 client, which uses 28960 on the same PC.
cd /d "%~dp0"
set MODE=1
if /i "%~1"=="internet" set MODE=2
start "" cod2rev_win32.exe +set dedicated %MODE% +set net_port 28970 +exec server.cfg +set logfile 2 +set sv_cracked 1 +map mp_toujane
