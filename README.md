# CoD2rev_Server (fork de JotaJazz)

Código fuente, obtenido por ingeniería inversa, del servidor dedicado de **Call of Duty 2** (versión 1.0, protocolo 115).
Corrige varios errores de los binarios originales y permite extender la funcionalidad del servidor.

Este repositorio es un fork de [voron00/CoD2rev_Server](https://github.com/voron00/CoD2rev_Server), que es donde está hecho todo el trabajo de ingeniería inversa. Aquí solo se añaden los cambios descritos en [Cambios de este fork](#cambios-de-este-fork).

## Transparencia

- **Quién mantiene esto:** soy JotaJazz. Me gustan los juegos antiguos y este fork existe por eso: quiero seguir pudiendo levantar un servidor de CoD2 en mis propias máquinas.
- **Cómo se trabaja aquí:** trabajo con [Claude Code](https://claude.com/claude-code), el asistente de programación de Anthropic. Los cambios de este fork, incluido este README, están escritos con su ayuda. Los commits hechos así llevan la línea `Co-Authored-By: Claude` en el mensaje, de modo que se pueden identificar en el historial con `git log`.
- **Relación con el original:** este fork trae cambios del repositorio original cuando los hay, pero no le envía los suyos. Es probable que con el tiempo se aleje de él.
- **Qué no es:** no es un proyecto oficial ni está afiliado a Activision, Infinity Ward ni al autor del repositorio original. No incluye ningún archivo del juego.

## Cambios de este fork

- **Compilación para Windows con MinGW** (32 bits), también cruzada desde Linux.
- **Contraseñas fuera del log:** `rcon_password` y `sv_privatePassword` se muestran como `********` en el registro de consola.
- **Bloqueo de rcon por intentos fallidos:** tras varias contraseñas incorrectas, la dirección IP queda ignorada por rcon durante un tiempo. Una contraseña correcta reinicia el contador y las direcciones `127.x.x.x` nunca se bloquean.
- **Master servers:**
  - `sv_master2` viene configurado con `master.cod2x.me`, el master de la comunidad CoD2x.
  - El formato `host:puerto` en `sv_master1`–`sv_master5` ahora funciona.
  - Un fallo de DNS se reintenta en vez de borrar el master.
- **Lanzador para Windows:** `scripts/windows/iniciar_cod2rev.bat`.

### Variables nuevas

| Variable | Por defecto | Rango | Descripción |
| --- | --- | --- | --- |
| `sv_rconMaxAttempts` | `5` | 0–100 | Contraseñas de rcon incorrectas permitidas por IP antes del bloqueo. `0` desactiva el bloqueo. |
| `sv_rconLockTime` | `600` | 0–86400 | Segundos que dura el bloqueo. También es la ventana en la que se cuentan los intentos. |

## Compilar

El resultado queda en `bin/` y los objetos intermedios en `obj/`.

### Windows (cruzado desde Linux con MinGW)

Necesita el compilador MinGW-w64 de 32 bits (`i686-w64-mingw32`).

```bash
make OS=Windows_NT CC=i686-w64-mingw32-gcc-posix WINDRES=i686-w64-mingw32-windres bin/cod2rev_win32.exe
```

Genera `bin/cod2rev_win32.exe`.

### Linux

```bash
make
```

Genera `bin/cod2rev_lnxded`. El servidor original es un binario x86 de 32 bits, así que en un sistema de 64 bits hace falta un compilador que genere 32 bits. El `Makefile` no lo fuerza y este fork no ha tocado la compilación de Linux.

### Opciones del Makefile

| Opción | Por defecto | Descripción |
| --- | --- | --- |
| `WITH_LIBCOD` | `true` | Incluye las extensiones de script de libcod (bots, jugadores, entidades, armas, utilidades, etc.). |
| `WITH_SQLITE` | `true` | Funciones de script para SQLite. |
| `WITH_MYSQL` | `false` | Funciones de script para MySQL. Requiere la biblioteca cliente de MySQL. |

`make clean` borra el binario y los objetos.

## Ejecutar

El servidor necesita los archivos del juego original (la carpeta `main` con sus `.iwd`), que no están en este repositorio. Hay que tener una copia propia de Call of Duty 2.

### Windows

1. Copiar `cod2rev_win32.exe` y `scripts/windows/iniciar_cod2rev.bat` a la carpeta del juego, junto a `main`.
2. Tener un `server.cfg` en `main`.
3. Ejecutar `iniciar_cod2rev.bat`.

El lanzador arranca el servidor en el puerto `28970`, para no chocar con el cliente del juego que usa el `28960` en el mismo PC, y carga el mapa `mp_toujane`. Usa `dedicated 1` (LAN o IP directa); con `dedicated 2` el servidor se anuncia en la lista pública.

### Linux

`run_bin.sh` es el script de pruebas del proyecto original: arranca `bin/cod2rev_lnxded` en modo desarrollo con trucos activados y bots de prueba. Sirve de ejemplo de parámetros, no como configuración para un servidor real.

## Estructura del repositorio

| Ruta | Contenido |
| --- | --- |
| `src/server` | Lógica del servidor: clientes, red, rcon, master servers. |
| `src/game`, `src/bgame` | Lógica de juego. |
| `src/script` | Máquina virtual de scripts GSC. |
| `src/qcommon`, `src/universal` | Código común: comandos, variables (dvars), sistema de archivos, red. |
| `src/xanim` | Modelos y animaciones. |
| `src/stringed` | Textos localizados. |
| `src/libcod` | Extensiones de script de libcod, con SQLite y las cabeceras de MySQL. |
| `src/win32`, `src/unix` | Código específico de cada plataforma. |
| `src/zlib` | Copia de zlib. |
| `test` | Material de la ingeniería inversa: base de datos de IDA, tablas de offsets y cargador de pruebas. |
| `scripts/windows` | Lanzador para Windows. |

## Licencia

[GNU Affero General Public License v3.0](LICENSE), la misma que el proyecto original.

## Créditos

- [voron00](https://github.com/voron00/CoD2rev_Server), autor del proyecto original
- id Software
- Activision / Infinity Ward
- [CoD4x Server](https://github.com/callofduty4x/CoD4x_Server)
- [OpenBO2](https://github.com/builtbyxeno/OpenBO2)
