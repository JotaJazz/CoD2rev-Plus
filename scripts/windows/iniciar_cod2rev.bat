@echo off
rem Inicia el servidor CoD2rev (version de JotaJazz) usando los archivos de esta carpeta.
rem Puerto 28970 para no chocar con el cliente de CoD2, que usa 28960 en este mismo PC.
rem dedicated 1 = LAN / IP directa. Cambia a 2 para que aparezca en la lista publica.
cd /d "%~dp0"
start "" cod2rev_win32.exe +set dedicated 1 +set net_port 28970 +exec server.cfg +set logfile 2 +set sv_cracked 1 +map mp_toujane
