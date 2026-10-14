# CoD2rev_Server (Fork von JotaJazz)

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

- **Windows-Build mit MinGW** (32 Bit), auch als Cross-Kompilierung unter Linux.
- **Passwörter erscheinen nicht im Log:** `rcon_password` und `sv_privatePassword` werden im Konsolenprotokoll als `********` angezeigt.
- **Rcon-Sperre nach Fehlversuchen:** Nach mehreren falschen Passwörtern wird die IP-Adresse von rcon eine Zeit lang ignoriert. Ein richtiges Passwort setzt den Zähler zurück, und Adressen aus `127.x.x.x` werden nie gesperrt.
- **Master-Server:**
  - `sv_master2` ist mit `master.cod2x.me` vorbelegt, dem Master der CoD2x-Community.
  - Das Format `Host:Port` in `sv_master1`–`sv_master5` funktioniert jetzt.
  - Ein DNS-Fehler wird erneut versucht, statt den Master zu löschen.
- **Spielversion beim Kompilieren wählbar:** Der Server lässt sich als 1.0 oder als 1.3 bauen.
- **Windows-Starter:** `scripts/windows/iniciar_cod2rev.bat`.

### Neue Variablen

| Variable | Standard | Bereich | Beschreibung |
| --- | --- | --- | --- |
| `sv_rconMaxAttempts` | `5` | 0–100 | Erlaubte falsche rcon-Passwörter pro IP vor der Sperre. `0` schaltet die Sperre ab. |
| `sv_rconLockTime` | `600` | 0–86400 | Dauer der Sperre in Sekunden. Zugleich das Zeitfenster, in dem die Versuche gezählt werden. |

## Spielversionen

- **Die Version wird beim Kompilieren gewählt.** Standardmäßig wird der Server als 1.0 gebaut (Protokoll 115); mit `PROTOCOL=118` wird er als 1.3 gebaut. Der Server meldet diese Version an die Master-Server und erscheint daher in der Serverliste von Clients derselben Version.
- **Der 1.0-Build akzeptiert per direkter IP auch andere Versionen.** Mit libcod, das standardmäßig aktiviert ist, nimmt er Verbindungen mit den Protokollen 115 bis 119 an, also auch 1.3 (Protokoll 118) und die Game-Pass-Version (Protokoll 119). Die Verbindung per direkter IP zu diesem Build wurde getestet, auch mit einem CoD2x-1.4.x-Client.
- **Der 1.3-Build akzeptiert nur Clients mit Protokoll 118.** Er kompiliert für Linux und Windows, wurde aber noch nicht im Spiel getestet. Auch [CoD2x](https://github.com/callofduty2x/CoD2x)-1.4-Clients verbinden sich mit Protokoll 118, und ihre Serverliste zeigt nur Server an, die Protokoll 118 oder 120 melden; dies ist also der Build, den sie sehen können.

## Kompilieren

Das Ergebnis landet in `bin/`, die Zwischenobjekte in `obj/`.

### Windows (Cross-Kompilierung unter Linux mit MinGW)

Benötigt den 32-Bit-Compiler von MinGW-w64 (`i686-w64-mingw32`).

```bash
mkdir -p bin obj
make OS=Windows_NT CC=i686-w64-mingw32-gcc-posix WINDRES=i686-w64-mingw32-windres bin/cod2rev_win32.exe
```

Erzeugt `bin/cod2rev_win32.exe`.

### Linux

Der Server ist eine 32-Bit-x86-Binärdatei. Auf einem 64-Bit-System wird die 32-Bit-Unterstützung von GCC benötigt (unter Debian und Ubuntu die Pakete `gcc-multilib` und `g++-multilib`):

```bash
make CC="gcc -m32"
```

Erzeugt `bin/cod2rev_lnxded`.

### Spielversion

Hängen Sie `PROTOCOL=118` an den `make`-Befehl an, um den Server als 1.3 zu bauen; ohne diese Angabe oder mit `PROTOCOL=115` wird er als 1.0 gebaut. Führen Sie `make clean` aus, bevor Sie von einer Version zur anderen wechseln. Die automatischen Builds veröffentlichen beide Versionen für jede Plattform.

### Automatische Builds

Jeder Push auf `master` baut beide Versionen in GitHub Actions ([`.github/workflows/build.yml`](.github/workflows/build.yml)). Die fertigen Binärdateien lassen sich als Artefakte im Tab [Actions](https://github.com/JotaJazz/CoD2rev_Server/actions) des Repositorys herunterladen.

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
2. Eine `server.cfg` in `main` bereithalten.
3. `iniciar_cod2rev.bat` ausführen.

Der Starter startet den Server auf Port `28970`, damit er nicht mit dem Spielclient kollidiert, der auf demselben PC `28960` benutzt, und lädt die Karte `mp_toujane`. Er verwendet `dedicated 1` (LAN oder direkte IP); mit `dedicated 2` wird der Server in der öffentlichen Liste angekündigt.

### Schlüsselautorisierung

Der Server fragt bei `cod2master.activision.com` an, um den Schlüssel jedes Clients zu prüfen, der sich nicht im lokalen Netz befindet. Mit `sv_cracked 1` schickt er die Anfrage weiterhin, akzeptiert den Client aber unabhängig von der Antwort. Der Windows-Starter schaltet das ein; entfernen Sie es aus der Befehlszeile, wenn ein gültiger Schlüssel verlangt werden soll.

### Linux

`run_bin.sh` ist das Testskript des Originalprojekts: Es startet `bin/cod2rev_lnxded` im Entwicklermodus mit aktivierten Cheats und Test-Bots. Es dient als Beispiel für Parameter, nicht als Konfiguration für einen echten Server.

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
| `scripts/windows` | Windows-Starter. |

## Lizenz

[GNU Affero General Public License v3.0](LICENSE), dieselbe wie beim Originalprojekt.

## Danksagungen

- [voron00](https://github.com/voron00/CoD2rev_Server), Autor des Originalprojekts
- id Software
- Activision / Infinity Ward
- [CoD4x Server](https://github.com/callofduty4x/CoD4x_Server)
- [OpenBO2](https://github.com/builtbyxeno/OpenBO2)
