# CoD2rev-Plus

<p align="center"><img src="docs/banner.svg" alt="CoD2rev-Plus" width="100%"></p>

[![Build](https://github.com/JotaJazz/CoD2rev-Plus/actions/workflows/build.yml/badge.svg)](https://github.com/JotaJazz/CoD2rev-Plus/actions/workflows/build.yml)
[![License: AGPL-3.0](https://img.shields.io/badge/license-AGPL--3.0-blue)](LICENSE)
![Platforms: Linux | Windows](https://img.shields.io/badge/platform-Linux%20%7C%20Windows-lightgrey)
![CoD2 1.0 | 1.3](https://img.shields.io/badge/CoD2-1.0%20%7C%201.3-c9b37e)
[![Fork of voron00/CoD2rev_Server](https://img.shields.io/badge/fork%20of-voron00%2FCoD2rev__Server-555)](https://github.com/voron00/CoD2rev_Server)
[![Last commit](https://img.shields.io/github/last-commit/JotaJazz/CoD2rev-Plus)](https://github.com/JotaJazz/CoD2rev-Plus/commits/master)

[Español](README.es.md) · [English](README.md) · [Português](README.pt.md) · [Français](README.fr.md) · **Deutsch** · [Русский](README.ru.md)

Durch Reverse Engineering gewonnener Quellcode des dedizierten Servers von **Call of Duty 2** (Versionen 1.0 und 1.3).
Er behebt mehrere Fehler der Originalbinärdateien und erlaubt es, die Funktionen des Servers zu erweitern.

Dieses Repository ist ein Fork von [voron00/CoD2rev_Server](https://github.com/voron00/CoD2rev_Server), wo die gesamte Reverse-Engineering-Arbeit geleistet wurde. Dieser Fork ergänzt nur die unter [Änderungen in diesem Fork](#änderungen-in-diesem-fork) beschriebenen Änderungen.

## Transparenz

- **Wer das hier pflegt:** Ich bin JotaJazz. Ich mag alte Spiele, und deshalb gibt es diesen Fork: Ich möchte auch weiterhin einen CoD2-Server auf meinen eigenen Rechnern betreiben können.
- **Wie hier gearbeitet wird:** Ich arbeite mit [Claude Code](https://claude.com/claude-code), dem Programmierassistenten von Anthropic. Die Änderungen in diesem Fork, einschließlich dieses README und seiner Übersetzungen, sind mit seiner Hilfe geschrieben. So entstandene Commits tragen die Zeile `Co-Authored-By: Claude` in der Commit-Nachricht und lassen sich daher mit `git log` im Verlauf erkennen.
- **Verhältnis zum Original:** Dieser Fork übernimmt Änderungen aus dem Original-Repository, wenn es welche gibt, schickt seine eigenen aber nicht zurück. Er wird sich mit der Zeit wahrscheinlich davon entfernen.
- **Was es nicht ist:** Es ist kein offizielles Projekt und steht in keiner Verbindung zu Activision, Infinity Ward oder dem Autor des Original-Repositorys. Es enthält keine Spieldateien.

## Änderungen in diesem Fork

- **Builds für Windows und Linux:** ein MinGW-Build für Windows, `make` für 32-Bit-Linux, die Spielversion (1.0 oder 1.3) beim Bauen wählbar, automatische Builds bei jedem Push, Startskripte und eine Beispiel-`server.cfg`.
- **Sicherheit und Stabilität:** Netzwerkeingaben, die den Server abstürzen ließen oder seinen Speicher überschrieben, werden jetzt geprüft; rcon sperrt IPs nach falschen Passwörtern; Downloads liefern die Originaldateien des Spiels nicht mehr aus; IP-Sperren werden durchgesetzt; und ein Absturz hinterlässt einen Bericht in `crash.log`.
- **Kompatibilität:** große Nachrichten von 1.3-Clients, Game-Pass-Clients, Masterserver als `host:port`, `dedicated 1`, Spieldateien auf Windows-Laufwerken unter WSL, eine gleichmäßige Spieluhr und Bots, die nach einem Kartenwechsel zurückkommen.
- **Administration:** Passwörter nicht im Log, Name, Rotation und Zeitstempel des Logs, erzwungene Rate und Snapshot-Rate, ein einstellbarer CD-Key-Autorisierungsserver und abschaltbare Kick- und Timeout-Meldungen.
- **Skripting (libcod):** die meisten Skriptfunktionen und Callbacks von [zk_libcod](https://github.com/ibuddieat/zk_libcod) mit denselben Namen: Skriptfehler mit Datei und Zeile, Tastendrücke, Rcon-Befehle, Spieler- und Waffensteuerung, Admin-Werkzeuge, Wegpunkt-Graphen mit A\* und mehr, wobei mehrere ihrer Fehler unterwegs behoben wurden.
- **Bots-Mod:** `mods/bots`, Bots, die sich über Wegpunkt-Graphen orientieren, Visier, Granaten und Messer benutzen und die Ziele von SD und CTF spielen.
- **Tests:** ein Smoke-Test mit einem minimalen Client und ein Bot-Test auf echten Karten.

Jede Änderung im Detail, mit ihren Funktionen, Variablen und Korrekturen, steht in [docs/CHANGES.md](docs/CHANGES.md) (auf Englisch).

## Spielversionen

- **Die Version wird beim Kompilieren gewählt.** Standardmäßig wird der Server als 1.0 gebaut (Protokoll 115); mit `PROTOCOL=118` wird er als 1.3 gebaut. Der Server meldet diese Version an die Master-Server und erscheint daher in der Serverliste von Clients derselben Version.
- **Der 1.0-Build akzeptiert per direkter IP auch andere Versionen.** Mit libcod, das standardmäßig aktiviert ist, nimmt er Verbindungen mit den Protokollen 115 bis 119 an, also auch 1.3 (Protokoll 118) und die Game-Pass-Version (Protokoll 119). Die Verbindung per direkter IP zu diesem Build wurde getestet, auch mit einem CoD2x-1.4.x-Client.
- **Der 1.3-Build akzeptiert nur Clients mit Protokoll 118.** Er wurde unter Linux und Windows gestartet, lädt dort eine Karte und antwortet als 1.3, und auf dem Linux-Build wurde mit einem CoD2x-1.4-Client gespielt. Auch [CoD2x](https://github.com/callofduty2x/CoD2x)-1.4-Clients verbinden sich mit Protokoll 118, und ihre Serverliste zeigt nur Server an, die Protokoll 118 oder 120 melden; dies ist also der Build, den sie sehen können.

## Kompilieren

Das Ergebnis landet in `bin/`, die Zwischenobjekte in `obj/`.

### Windows (Cross-Kompilierung unter Linux mit MinGW)

Benötigt den 32-Bit-Compiler von MinGW-w64 (`i686-w64-mingw32`).

```bash
make windows
```

Erzeugt `bin/cod2rev_win32.exe`.

### Linux

Der Server ist eine 32-Bit-x86-Binärdatei. Auf einem 64-Bit-System wird die 32-Bit-Unterstützung von GCC benötigt (unter Debian und Ubuntu die Pakete `gcc-multilib` und `g++-multilib`):

```bash
make
```

Erzeugt `bin/cod2rev_lnxded`.

### Spielversion

Hängen Sie `PROTOCOL=118` an den `make`-Befehl an, um den Server als 1.3 zu bauen; ohne diese Angabe oder mit `PROTOCOL=115` wird er als 1.0 gebaut. Jede Plattform und Version behält ihre eigenen Objekte unter `obj/`, Sie können also ohne `make clean` zwischen ihnen wechseln. Die automatischen Builds veröffentlichen beide Versionen für jede Plattform.

### Automatische Builds

Jeder Push auf `master`, der den Code ändert, baut beide Versionen in GitHub Actions ([`.github/workflows/build.yml`](.github/workflows/build.yml)). Die fertigen Binärdateien lassen sich als Artefakte im Tab [Actions](https://github.com/JotaJazz/CoD2rev-Plus/actions) des Repositorys herunterladen.

### Makefile-Optionen

| Option | Standard | Beschreibung |
| --- | --- | --- |
| `WITH_LIBCOD` | `true` | Bindet die Skript-Erweiterungen von libcod ein (Bots, Spieler, Entitäten, Waffen, Hilfsfunktionen usw.). |
| `WITH_SQLITE` | `true` | Skriptfunktionen für SQLite. |
| `WITH_MYSQL` | `false` | Skriptfunktionen für MySQL. Benötigt die MySQL-Client-Bibliothek. |

`make clean` löscht die Binärdatei und die Objekte.

## Ausführen

Der Server braucht die Dateien des Originalspiels (den Ordner `main` mit seinen `.iwd`-Dateien), die nicht in diesem Repository liegen. Man braucht eine eigene Kopie von Call of Duty 2.

### Windows

1. `cod2rev_win32.exe` und `scripts/windows/iniciar_cod2rev.bat` in den Spielordner kopieren, neben `main`.
2. Eine `server.cfg` in `main` bereithalten. [`scripts/server.cfg.example`](scripts/server.cfg.example) setzt die üblichen Optionen und listet alle übrigen Variablen, die der Server registriert, mit ihrem Standardwert auf (Kommentare auf Englisch).
3. `iniciar_cod2rev.bat` ausführen.

Der Starter startet den Server auf Port `28970`, damit er nicht mit dem Spielclient kollidiert, der auf demselben PC `28960` benutzt, und lädt die Karte `mp_toujane`. Standardmäßig startet er im LAN-Modus (`dedicated 1`, LAN oder direkte IP); mit `iniciar_cod2rev.bat internet` wird der Server in der öffentlichen Liste angekündigt (`dedicated 2`).

### Schlüsselautorisierung

Der Server fragt bei `cod2master.activision.com` an, um den Schlüssel jedes Clients zu prüfen, der sich nicht im lokalen Netz befindet. Mit `sv_cracked 1` schickt er die Anfrage weiterhin, akzeptiert den Client aber unabhängig von der Antwort. Die Startskripte schalten das ein; entfernen Sie es aus der Befehlszeile, wenn ein gültiger Schlüssel verlangt werden soll.

### Linux

Kopieren Sie `cod2rev_lnxded` und `scripts/linux/iniciar_cod2rev.sh` in den Spielordner, neben `main`, mit einer `server.cfg` in `main`. Starten Sie `./iniciar_cod2rev.sh` für den LAN-Modus oder `./iniciar_cod2rev.sh internet`, um den Server in der öffentlichen Liste anzukündigen. Das Skript verwendet Port `28960` und lädt die Karte `mp_toujane`.

`run_bin.sh` ist das Testskript des Originalprojekts: Es startet `bin/cod2rev_lnxded` im Entwicklermodus mit aktivierten Cheats und Test-Bots. Es dient als Beispiel für Parameter, nicht als Konfiguration für einen echten Server.

## Tests

`scripts/smoke_test.py` braucht einen gebauten Server und einen Spielordner:

```bash
python3 scripts/smoke_test.py /pfad/zu/CoD2
make PROTOCOL=118 && python3 scripts/smoke_test.py /pfad/zu/CoD2 --protocol 118
```

Es startet den Server im LAN-Modus mit einem temporären Benutzerordner, der Spielordner wird also nur gelesen. Es prüft, dass der Server eine Karte lädt und auf `getinfo`, `getstatus` und rcon antwortet, und dass `scripts/cod2client.py`, ein minimaler Client, sich verbinden, den Gamestate empfangen, ins Spiel kommen und Befehle senden kann, auch in einer in Fragmente aufgeteilten Nachricht. Verwende einen Spielordner mit den originalen `.iwd`-Dateien: Mods in `main`, die die Skripte der Spielmodi ersetzen, können das Laden der Karte verhindern.

`scripts/bots_test.py` installiert den Bots-Mod in einem temporären Ordner, spielt eine Karte mit sechs Bots und prüft, dass der Wegpunkt-Graph erzeugt, gespeichert und wieder gelesen wird, dass die Bots Wegen folgen und Visier, Granaten und Messer benutzen und dass keine Skriptfehler auftreten:

```bash
python3 scripts/bots_test.py /pfad/zu/CoD2 --map mp_carentan
```

Mit `--gametype sd` oder `--gametype ctf` prüft er stattdessen, dass die Bots die Bombe legen oder die gegnerische Flagge aufnehmen. `--bombmode 1` spielt SD im Modus mit nur einer Bombenzone. `--corrupt-cache` startet mit einer beschädigten Wegpunktdatei, die die Bots durch einen neuen Graphen ersetzen müssen.

Um Speicherfehler zu finden, baue den Server mit AddressSanitizer in einem eigenen Ordner und übergib dieses Binary mit `--binary`:

```bash
make OBJ_ROOT=/tmp/asan/obj BIN_DIR=/tmp/asan/bin \
  CFLAGS="-fno-pie -O1 -g -fno-omit-frame-pointer -mfpmath=387 -DNDEBUG -DPROTOCOL_VERSION=115 -MMD -MP -m32 -D_FILE_OFFSET_BITS=64 -fsanitize=address" \
  LFLAGS="-no-pie -m32 -fsanitize=address"
```

Ein auf der Kommandozeile angegebenes `CFLAGS` ersetzt das des Makefiles, daher müssen `-m32` und `-D_FILE_OFFSET_BITS=64` wiederholt werden. Lässt man diesen Build eine Weile mit der Bot-Mod und einer kurzen `sv_mapRotation` laufen, sind auch Kartenwechsel und das Spiel abgedeckt.

## Aufbau des Repositorys

| Pfad | Inhalt |
| --- | --- |
| `src/server` | Serverlogik: Clients, Netzwerk, rcon, Master-Server. |
| `src/game`, `src/bgame` | Spiellogik. |
| `src/script` | Virtuelle Maschine für GSC-Skripte. |
| `src/qcommon`, `src/universal` | Gemeinsamer Code: Befehle, Variablen (dvars), Dateisystem, Netzwerk. |
| `src/xanim` | Modelle und Animationen. |
| `src/stringed` | Lokalisierte Texte. |
| `src/libcod` | Skript-Erweiterungen von libcod, mit SQLite und den MySQL-Headern. |
| `src/win32`, `src/unix` | Plattformspezifischer Code. |
| `src/zlib` | Kopie von zlib. |
| `test` | Material aus dem Reverse Engineering: IDA-Datenbank, Offset-Tabellen und Test-Loader. |
| `scripts` | Startskripte für Windows und Linux, die Beispiel-`server.cfg` und ihr Generator, das Installationsskript der Bots der Funktionstest mit seinem minimalen Client sowie der Bots-Test. |
| `mods/bots` | Die Skript-Mod der einfachen Bots. |

## Lizenz

[GNU Affero General Public License v3.0](LICENSE), dieselbe wie beim Originalprojekt.

## Danksagungen

- [voron00](https://github.com/voron00/CoD2rev_Server), Autor des Originalprojekts
- id Software
- Activision / Infinity Ward
- [CoD4x Server](https://github.com/callofduty4x/CoD4x_Server)
- [OpenBO2](https://github.com/builtbyxeno/OpenBO2)
- [zk_libcod](https://github.com/ibuddieat/zk_libcod) (ibuddieat), wo mehrere der Korrekturen dieses Forks gefunden wurden
