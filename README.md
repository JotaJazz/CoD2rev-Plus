# CoD2rev_Server

## About

This is a reverse-engenerred Call of Duty 2 dedicated server code.
It fixes several bugs in the original binaries and allows developers to extend server functionality.

## Cambios de este fork (JotaJazz)

Fork de [voron00/CoD2rev_Server](https://github.com/voron00/CoD2rev_Server) con estos cambios:

- **Compilación para Windows con MinGW** (32 bits). Desde Linux:
  `make OS=Windows_NT CC=i686-w64-mingw32-gcc-posix WINDRES=i686-w64-mingw32-windres bin/cod2rev_win32.exe`
- **Contraseñas fuera del log:** `rcon_password` y `sv_privatePassword` se muestran como `********` en el registro de consola.
- **Bloqueo de rcon por intentos fallidos** (por IP; 127.x.x.x nunca se bloquea):
  - `sv_rconMaxAttempts` (por defecto 5, 0 = desactivado)
  - `sv_rconLockTime` (segundos, por defecto 600)
- **Master servers:** `sv_master2` viene con `master.cod2x.me` (master de la comunidad CoD2x), el formato `host:puerto` en `sv_master1`–`5` ahora funciona y un fallo de DNS se reintenta en vez de borrar el master.
- **Lanzador para Windows:** `scripts/windows/iniciar_cod2rev.bat` (copiar junto al juego).

## Credits:

id Software

Activision / Infinity Ward

CoD4x Server: https://github.com/callofduty4x/CoD4x_Server

OpenBO2: https://github.com/builtbyxeno/OpenBO2