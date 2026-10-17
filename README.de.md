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

- **Windows-Build mit MinGW** (32 Bit), auch als Cross-Kompilierung unter Linux.
- **Passwörter erscheinen nicht im Log:** `rcon_password` und `sv_privatePassword` werden im Konsolenprotokoll als `********` angezeigt.
- **Rcon-Sperre nach Fehlversuchen:** Nach mehreren falschen Passwörtern wird die IP-Adresse von rcon eine Zeit lang ignoriert. Ein richtiges Passwort setzt den Zähler zurück, und Adressen aus `127.x.x.x` werden nie gesperrt.
- **Master-Server:**
  - `sv_master2` ist mit `master.cod2x.me` vorbelegt, dem Master der CoD2x-Community.
  - Das Format `Host:Port` in `sv_master1`–`sv_master5` funktioniert jetzt.
  - Ein DNS-Fehler wird erneut versucht, statt den Master zu löschen.
- **Spielversion beim Kompilieren wählbar:** Der Server lässt sich als 1.0 oder als 1.3 bauen.
- **Überlauf bei Game-Pass-Clients behoben:** Das Senden der Systeminformationen an einen Client mit Protokoll 119 konnte einen Puffer überlaufen lassen und den Server zum Absturz bringen.
- **Große Nachrichten von 1.3-Clients:** Fragmentierte Nachrichten von 1.3-Clients wurden falsch gelesen und verworfen; sie werden jetzt im 1.3-Format gelesen.
- **Begrenzte Zeichenketten in den libcod-Skriptfunktionen:** `printf` und `sprintf` von libcod, die asynchronen SQLite- und exec-Aufrufe und das Umbenennen von Spielern schreiben nicht mehr über ihre Puffer hinaus, wenn ein Skript lange Zeichenketten übergibt.
- **Einfacheres Bauen:** `make` erzeugt direkt die 32-Bit-Linux-Binärdatei, `make windows` cross-kompiliert für Windows, und GitHub Actions baut bei jedem Push alle Varianten.
- **Beispiel-`server.cfg`:** `scripts/server.cfg.example` listet alle Variablen auf, die der Server registriert; `scripts/gen_server_cfg.py` erzeugt sie neu aus dem Quellcode.
- **`dedicated` wird beachtet:** `+set dedicated 1` wurde ignoriert, und jeder Server meldete sich bei den Master-Servern an. Jetzt hält `1` den Server aus der öffentlichen Liste heraus, und `2`, der Standardwert, kündigt ihn an.
- **Spieldateien auf Windows-Laufwerken:** Der Linux-Build konnte die Spieldateien auf Dateisystemen mit großen Inode-Nummern nicht sehen, etwa auf Windows-Laufwerken unter WSL. Er wird jetzt mit 64-Bit-Dateioffsets gebaut.
- **Stabile Spieluhr unter Linux:** Der Server hat die Zeit mit der Systemuhr gemessen, sodass jede Zeitkorrektur (NTP, Zeitsynchronisation einer virtuellen Maschine) das Spiel kurz einfrieren ließ. Jetzt verwendet er die monotone Uhr.
- **Robustere Netzwerkeingaben:** Ein verbundener Spieler konnte den Server mit einer zu großen komprimierten Nachricht, einer fragmentierten Nachricht, einem Namen mit `%` (den die originalen Spielmodi ins Log schreiben), `callvote clientkick` mit einer negativen Nummer oder Sprachpaketen abstürzen lassen oder seinen Speicher überschreiben. Diese Pfade prüfen jetzt ihre Grenzen, und die Dateien abgebrochener HTTP-Downloads werden geschlossen.
- **Bots nach einem Kartenwechsel:** Nach einem Kartenwechsel blieben Bots verbunden, aber außerhalb des Spiels und belegten ihren Platz, sodass die Bot-Mod weitere hinzufügte, bis der Server voll war. Jetzt kehren sie ins Spiel zurück, sobald die neue Karte geladen ist.
- **libcod-Proxyadresse und Downloads:** Die echte Adresse, die ein lokaler Proxy im Userinfo-Schlüssel `ip` übergibt, wird nur von `127.0.0.1` angenommen, muss eine IPv4-Adresse sein und wird bei jeder Verbindung gelöscht. Ein `donedl` eines Spielers, der schon im Spiel ist, lässt den Server nicht mehr den ganzen Gamestate erneut senden.
- **Downloads und Passwörter:** Die Prüfung, die Clients am Herunterladen der originalen `iw_*.iwd` hindern sollte, griff nie, sodass jeder Client sie laden konnte (jeweils Hunderte MB); sie werden jetzt abgelehnt, ebenso die nur für den Server bestimmten `_svr_`-iwds, auch vor einer HTTP-Weiterleitung. Downloads bleiben nicht mehr hängen, wenn währenddessen die Karte wechselt, eine Client-Nachricht, die zum Dekomprimieren zu groß ist, führt zum Entfernen des Clients statt abgeschnitten zu werden, und rcon, `g_password` und `sv_privatePassword` werden in konstanter Zeit verglichen.
- **Banns, Spiel-Log und weitere in zk_libcod gefundene Fehler:** `addip` und `g_banIPs` führten eine Liste gebannter Adressen, die nie geprüft wurde; sie wird jetzt beim Verbinden angewendet, auch bei Spielern hinter einem lokalen Proxy. Die letzte Adresse in `g_banIPs` wird nicht mehr übergangen, und ein zu langes `addip` läuft nicht mehr über den Stack hinaus. Zeilenumbrüche in Chatnachrichten und Spielernamen konnten gefälschte Zeilen in `games_mp.log` schreiben und `getstatus`-Antworten zerteilen; sie werden jetzt entfernt. Ein falsches rcon-Passwort hält das richtige nicht mehr auf, ein `userinfo` verschluckt nicht mehr die nächste Chatnachricht, und das Halten der Granatentaste ohne Granaten verbirgt Schüsse und Schritte des Spielers nicht mehr vor den anderen. Die Effekt-Entitäten von Granatenexplosionen werden jetzt freigegeben, statt sich bis zum Kartenende anzuhäufen, ein Skript, das ein benutztes stationäres MG löscht, beendet nicht mehr die Karte, und ein zu langer Kartenname, der an eine Skriptfunktion übergeben wird, hält den Server nicht mehr an.
- **Absturzberichte:** Stürzt der Server ab, schreibt er das Signal bzw. die Ausnahme, die fehlerhafte Adresse und die Aufrufadressen in die Konsole und in `crash.log` in `fs_homepath`. `addr2line -f -e <Binärdatei> <Adresse>` macht daraus Stellen im Quellcode. Unter Linux endet der Prozess danach mit demselben Signal wie bisher, Core-Dumps funktionieren also weiterhin.
- **Log- und Admin-Einstellungen:** Das Konsolen-Log lässt sich umbenennen (`logfileName`), kann die Logs früherer Läufe behalten (`logfileRotate`) und jede Zeile mit Datum und Uhrzeit beginnen (`logTimestamps`). Angenommene rcon-Befehle können aus dem Log herausgehalten werden (`sv_logRcon`), der Autorisierungsserver für CD-Keys ist einstellbar (`sv_authorizeServer`, `sv_authorizePort`), der Server kann allen Clients eine Rate und Snapshot-Rate vorgeben (`g_forceRate`, `g_forceSnaps`), und die Meldungen über einen Kick oder ein Timeout lassen sich abschalten (`sv_kickMessages`, `sv_timeoutMessages`). Die Namen entsprechen zk_libcod, dessen Konfigurationen funktionieren also weiter.
- **Mehr libcod-Callbacks und Spielermethoden (aus zk_libcod, gleiche Namen):** `_callbacksetup.gsc` kann `CodeCallback_Error(critical, function, message)` (Skript-Laufzeitfehler, im nächsten Frame zugestellt), `CodeCallback_HitchWarning(msec)`, `CodeCallback_Pickup(type, weapon, name, quantity, clip, origin, angles)` (ersetzt das originale Aufheben), `CodeCallback_Suicide()` (ersetzt `suicide()`; `processSuicide()` tötet), `CodeCallback_WeaponChange(index, name)` und je einen Callback pro Tastendruck definieren: `Attack`, `Melee`, `Use`, `Reload`, `LeanLeft`, `LeanRight`, `Prone`, `Crouch`, `Stand`, `Ads`, `MeleeBreath`, `HoldBreath`, `Frag` und `Smoke` (`CodeCallback_<Name>Button()`). Neue Spielermethoden: `isFiring`, `isMeleeing`, `isReloading`, `isThrowingGrenade`, `isUsingBinoculars`, `isRechambering`, `isShellShocked`, `isChatting`, `holdBreathButtonPressed`, `get`/`setCurrentWeaponAmmo`, `get`/`setCurrentWeaponClipAmmo`, `getCurrentWeaponSlot`, `get`/`setSpeed`, `get`/`setGravity`, `getGroundEntity`, `getViewOrigin`, `getPlayerstateFlags`, `getInactivityTime`, `getEnterTime` und `processSuicide`. Anders als in zk_libcod laufen die Callbacks für Waffenwechsel und Fehler nie mitten in der Spielerbewegung oder in der Skript-VM, und ein `suicide()` in `CodeCallback_Suicide` erzeugt keine Endlosschleife.
- **Wegpunkt-Graphen mit A\* und weitere Skriptfunktionen (aus zk_libcod):** `graphCreate`, `graphAddNode`, `graphAddEdge`, `graphFindPath`, `graphFindClosestNode`, `graphAutodiscover`, `graphSave`/`graphLoad` und die übrigen `graph*`-Funktionen von zk_libcod lassen Bot-Skripte durch eine Karte navigieren (`src/libcod/gsc_graph.cpp`, unter seiner MIT-Lizenz übernommen). Anders als in zk_libcod werden Graphen auch bei einem schnellen `map_restart` (jede SD-Runde) gelöscht, eine ungültige ID oder Datei liefert undefined, ohne den Skript-Thread zu beenden, `graphLoad` lehnt Dateien über 64 MB ab und `graphSave` lehnt Namen mit `.iwd`, `.cfg` und `.gsc` ab. Ebenfalls neu: `abs`, `floor`, `ceil`, `pow`, `atan2`, `roundTo`, `getMilliseconds`, `getMicroseconds`, `toHex`, `fromHex`, `ord`, `chr`, `getEntityCount`, `logPrintConsole` und die Methoden `getUserinfo`/`setUserinfo`; `bulletTrace` und `bulletTracePassed` nehmen eine optionale Inhaltsmaske, und `bulletTrace` liefert zusätzlich `flags`, `contents` und `material`; mit `sv_botUseTriggerUse 1` bekommt ein Bot, der in einem Benutzungs-Trigger steht (etwa einer SD-Bombenzone), `self waittill("bot_trigger", trigger)`.
- **Funktionstest:** `scripts/smoke_test.py` startet den Linux-Server mit einem echten Spielordner und prüft, ob er eine Karte lädt, Anfragen beantwortet und einen minimalen Client (`scripts/cod2client.py`) verbinden lässt, der den Gamestate empfängt, ins Spiel kommt und Befehle sendet, auch in fragmentierten Nachrichten.
- **Einfache Bots:** `mods/bots` ist ein kleiner Skript-Mod, dessen Bots jede Karte über einen Wegpunkt-Graphen ablaufen, der mit den `graph*`-Funktionen von libcod gebaut wird: Beim ersten Spielen einer Karte wird der Graph aus ihren Spawnpunkten erzeugt und als `waypoints/<Karte>_v1.graph` im Mod-Ordner gespeichert, spätere Ladevorgänge lesen ihn. Ist niemand in Sicht, folgen sie dem, was sie zuletzt gesehen oder schießen gehört haben, patrouillieren zur gegnerischen Seite und gehen erst nach 20 s ohne Kontakt auf den nächsten Gegner zu. Auf Distanz zielen sie über Kimme und Korn, auf mittlere Entfernung werfen sie Granaten, aus nächster Nähe setzen sie das Messer ein, und zwischen Gefechten laden sie nach. In SD und CTF zieht jeder Bot in jedem Leben eine Rolle: Angreifer legen die Bombe oder jagen, Verteidiger bewachen eine Bombenzone oder jagen, und sobald die Bombe liegt, gehen die Verteidiger sie entschärfen, während die Leger sie bewachen; in CTF stehlen die Bots die gegnerische Flagge und bringen sie zur eigenen Basis oder verteidigen die eigene, verfolgen ihren Träger und bringen sie zurück, wenn sie fällt (SD im dualen Bombenmodus wird ohne Ziele gespielt). `scripts/install_bots.py` installiert ihn; `bots_count`, `bots_skill` und `bots_debug` steuern sie.
- **Mehr Bot-Tasten in libcod:** Die Skriptmethoden `useButton`, `throwSmoke` und `holdBreath` lassen einen Bot Benutzen drücken (um die Bombe zu legen oder zu entschärfen), eine Rauchgranate werfen oder den Atem anhalten, wie die vorhandenen `fireWeapon` und `throwNade`. Sie nehmen `1` zum Drücken und `0` zum Loslassen.
- **Weniger Hänger und ein saubererer Start:** Der Name des Master-Servers wird nur einmal aufgelöst statt bei jedem Zeitüberlauf der Autorisierung, und der Server warnt nicht mehr bei jedem `+set` der Befehlszeile.
- **Startskripte:** `scripts/windows/iniciar_cod2rev.bat` und `scripts/linux/iniciar_cod2rev.sh`, mit einem LAN- und einem Internet-Modus.

### Neue Variablen

| Variable | Standard | Bereich | Beschreibung |
| --- | --- | --- | --- |
| `sv_rconMaxAttempts` | `5` | 0–100 | Erlaubte falsche rcon-Passwörter pro IP vor der Sperre. `0` schaltet die Sperre ab. |
| `sv_rconLockTime` | `600` | 0–86400 | Dauer der Sperre in Sekunden. Zugleich das Zeitfenster, in dem die Versuche gezählt werden. |
| `logfileName` | `""` | — | Datei des Konsolen-Logs in `fs_homepath/<Mod>`. Leer bedeutet `console_mp_server.log`; Namen mit `..` oder einem vollständigen Pfad werden ignoriert. |
| `logfileRotate` | `0` | 0–1000 | Logs früherer Läufe, die beim Start behalten werden, als `<Name>.1` (neuestes) bis `<Name>.N`. |
| `logTimestamps` | `0` | 0–1 | `1` beginnt jede Zeile des Konsolen-Logs mit Datum und Uhrzeit. |
| `sv_logRcon` | `1` | 0–1 | `0` hält angenommene rcon-Befehle aus dem Log heraus. Falsche Passwörter werden immer protokolliert. |
| `sv_authorizeServer` | `cod2master.activision.com` | — | Autorisierungsserver für CD-Keys. Ein neuer Wert gilt ab der nächsten Verbindung. |
| `sv_authorizePort` | `20700` | 1–65535 | Port des Autorisierungsservers. |
| `g_forceRate` | `0` | 0–25000 | Rate, die jeder Client statt seiner eigenen bekommt (mindestens 1000). `0` behält die des Clients. |
| `g_forceSnaps` | `0` | 0–30 | Snapshots pro Sekunde für jeden Client. `0` behält die des Clients. |
| `sv_kickMessages` | `1` | 0–1 | `0` blendet die Meldung aus, dass ein Spieler gekickt wurde. |
| `sv_timeoutMessages` | `1` | 0–1 | `0` blendet die Meldung aus, dass ein Spieler wegen Timeout getrennt wurde. |
| `sv_botUseTriggerUse` | `0` | 0–1 | `1` schickt einem Bot ein `bot_trigger`-Notify mit dem Trigger, solange er in einem Benutzungs-Trigger steht, den er benutzen könnte. |
| `g_debugGraph` | `0` | 0–1 | `1` gibt Debug-Meldungen der Graph-Funktionen aus. |
| `sv_graphAstarMaxIterations` | `0` | 0–100000 | Höchstzahl der Knoten, die ein `graphFindPath` erweitert. `0` bedeutet kein Limit. |
| `sv_graphAstarWeight` | `1` | 1–2 | Gewicht der A\*-Heuristik. Über `1` werden Wege schneller gefunden, können aber länger sein. |
| `sv_graphMaxGraphs` | `8` | 1–64 | Höchstzahl gleichzeitiger Graphen. |
| `sv_graphMaxNodes` | `8192` | 64–65535 | Höchstzahl der Knoten in einem Graphen. |

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
