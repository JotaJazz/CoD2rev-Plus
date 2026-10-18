# CoD2rev-Plus

<p align="center"><img src="docs/banner.svg" alt="CoD2rev-Plus" width="100%"></p>

[![Build](https://github.com/JotaJazz/CoD2rev-Plus/actions/workflows/build.yml/badge.svg)](https://github.com/JotaJazz/CoD2rev-Plus/actions/workflows/build.yml)
[![License: AGPL-3.0](https://img.shields.io/badge/license-AGPL--3.0-blue)](LICENSE)
![Platforms: Linux | Windows](https://img.shields.io/badge/platform-Linux%20%7C%20Windows-lightgrey)
![CoD2 1.0 | 1.3](https://img.shields.io/badge/CoD2-1.0%20%7C%201.3-c9b37e)
[![Fork of voron00/CoD2rev_Server](https://img.shields.io/badge/fork%20of-voron00%2FCoD2rev__Server-555)](https://github.com/voron00/CoD2rev_Server)
[![Last commit](https://img.shields.io/github/last-commit/JotaJazz/CoD2rev-Plus)](https://github.com/JotaJazz/CoD2rev-Plus/commits/master)

[Español](README.es.md) · [English](README.md) · [Português](README.pt.md) · **Français** · [Deutsch](README.de.md) · [Русский](README.ru.md)

Code source, obtenu par rétro-ingénierie, du serveur dédié de **Call of Duty 2** (versions 1.0 et 1.3).
Il corrige plusieurs bogues des binaires d'origine et permet d'étendre les fonctionnalités du serveur.

Ce dépôt est un fork de [voron00/CoD2rev_Server](https://github.com/voron00/CoD2rev_Server), où tout le travail de rétro-ingénierie a été réalisé. Ce fork n'ajoute que les modifications décrites dans [Modifications de ce fork](#modifications-de-ce-fork).

## Transparence

- **Qui maintient ce dépôt :** je suis JotaJazz. J'aime les vieux jeux, et c'est la raison d'être de ce fork : je veux pouvoir continuer à faire tourner un serveur CoD2 sur mes propres machines.
- **Comment le travail est fait :** je travaille avec [Claude Code](https://claude.com/claude-code), l'assistant de programmation d'Anthropic. Les modifications de ce fork, y compris ce README et ses traductions, sont écrites avec son aide. Les commits réalisés ainsi portent la ligne `Co-Authored-By: Claude` dans leur message, ce qui permet de les identifier dans l'historique avec `git log`.
- **Relation avec l'original :** ce fork récupère les modifications du dépôt d'origine quand il y en a, mais ne lui envoie pas les siennes. Il s'en éloignera probablement avec le temps.
- **Ce que ce n'est pas :** ce n'est pas un projet officiel et il n'est affilié ni à Activision, ni à Infinity Ward, ni à l'auteur du dépôt d'origine. Il ne contient aucun fichier du jeu.

## Modifications de ce fork

- **Compilation pour Windows et Linux :** compilation MinGW pour Windows, `make` pour Linux 32 bits, version du jeu (1.0 ou 1.3) choisie à la compilation, compilations automatiques à chaque push, lanceurs et un `server.cfg` d'exemple.
- **Sécurité et stabilité :** les entrées réseau qui faisaient planter le serveur ou écrasaient sa mémoire sont maintenant vérifiées ; rcon bloque les IP après des mots de passe erronés ; les téléchargements ne livrent plus les fichiers d'origine du jeu ; les bannissements d'IP sont appliqués ; et un plantage laisse un rapport dans `crash.log`.
- **Compatibilité :** grands messages des clients 1.3, clients Game Pass, serveurs maîtres en `hôte:port`, `dedicated 1`, fichiers du jeu sur des lecteurs Windows sous WSL, une horloge de jeu stable et des bots qui reviennent après un changement de carte.
- **Administration :** mots de passe absents du log, nom, rotation et horodatage du log, rate et snapshots imposés, serveur d'autorisation de clé CD configurable et messages de kick et de timeout optionnels.
- **Scripts (libcod) :** la plupart des fonctions et callbacks de script de [zk_libcod](https://github.com/ibuddieat/zk_libcod), sous les mêmes noms : erreurs de script avec fichier et ligne, appuis sur les boutons, commandes rcon, contrôle des joueurs et des armes, outils d'administration, graphes de waypoints avec A\*, ce que chaque joueur voit, entend et ce qu'il percute, la physique, JSON, des clients HTTP et WebSocket, des sons par le chat vocal et plus, en corrigeant plusieurs de ses bugs au passage.
- **Mod de bots :** `mods/bots`, des bots qui se repèrent avec des graphes de waypoints, utilisent la visée, les grenades et le couteau, et jouent les objectifs de SD et de CTF.
- **Tests :** un smoke test avec un client minimal qui lit les snapshots du serveur, et un test de bots qui jouent de vraies cartes.

Chaque changement en détail, avec ses fonctions, variables et corrections, se trouve dans [docs/CHANGES.md](docs/CHANGES.md) (en anglais).

## Versions du jeu

- **La version se choisit à la compilation.** Par défaut, le serveur est compilé en 1.0 (protocole 115) ; avec `PROTOCOL=118`, il est compilé en 1.3. Le serveur annonce cette version aux serveurs maîtres, il apparaît donc dans la liste de serveurs des clients de cette même version.
- **La compilation 1.0 accepte aussi d'autres versions par IP directe.** Avec libcod, activé par défaut, elle accepte les connexions utilisant les protocoles 115 à 119, ce qui inclut la 1.3 (protocole 118) et la version Game Pass (protocole 119). La connexion par IP directe à cette compilation a été testée, y compris avec un client CoD2x 1.4.x.
- **La compilation 1.3 n'accepte que les clients au protocole 118.** Elle a été démarrée sous Linux et sous Windows, où elle charge une carte et répond en 1.3, et la compilation Linux a été jouée avec un client CoD2x 1.4. Les clients [CoD2x](https://github.com/callofduty2x/CoD2x) 1.4 se connectent eux aussi avec le protocole 118, et leur liste de serveurs n'affiche que ceux qui annoncent le protocole 118 ou 120 ; c'est donc cette compilation qu'ils peuvent voir.

## Compilation

Le résultat est placé dans `bin/` et les objets intermédiaires dans `obj/`.

### Windows (compilation croisée depuis Linux avec MinGW)

Nécessite le compilateur MinGW-w64 32 bits (`i686-w64-mingw32`).

```bash
make windows
```

Produit `bin/cod2rev_win32.exe`.

### Linux

Le serveur est un binaire x86 32 bits. Sur un système 64 bits, il faut la prise en charge 32 bits de GCC (sous Debian et Ubuntu, les paquets `gcc-multilib` et `g++-multilib`) :

```bash
make
```

Produit `bin/cod2rev_lnxded`.

### Version du jeu

Ajoutez `PROTOCOL=118` à la commande `make` pour compiler le serveur en 1.3 ; sans cela, ou avec `PROTOCOL=115`, il est compilé en 1.0. Chaque plateforme et chaque version garde ses propres objets dans `obj/`, vous pouvez donc passer de l'une à l'autre sans `make clean`. La compilation automatique publie les deux versions pour chaque plateforme.

### Compilation automatique

Chaque push sur `master` qui modifie le code compile les deux versions dans GitHub Actions ([`.github/workflows/build.yml`](.github/workflows/build.yml)). Les binaires obtenus peuvent être téléchargés comme artefacts depuis l'onglet [Actions](https://github.com/JotaJazz/CoD2rev-Plus/actions) du dépôt.

### Options du Makefile

| Option | Valeur par défaut | Description |
| --- | --- | --- |
| `WITH_LIBCOD` | `true` | Inclut les extensions de script de libcod (bots, joueurs, entités, armes, utilitaires, etc.). |
| `WITH_SQLITE` | `true` | Fonctions de script pour SQLite. |
| `WITH_MYSQL` | `false` | Fonctions de script pour MySQL. Nécessite la bibliothèque cliente MySQL. |
| `WITH_SPEEX` | `true` | `loadSoundFile`, `playSoundFile` et les autres fonctions de son, avec Speex (`src/libcod/speex`). |
| `WITH_HTTP` | `true` | `httpFetch` et `webSocket*`, avec Mbed TLS (`src/libcod/mbedtls`) pour `https://` et `wss://`. |

`make clean` supprime le binaire et les objets.

## Exécution

Le serveur a besoin des fichiers du jeu d'origine (le dossier `main` avec ses fichiers `.iwd`), qui ne sont pas dans ce dépôt. Il faut posséder sa propre copie de Call of Duty 2.

### Windows

1. Copier `cod2rev_win32.exe` et `scripts/windows/iniciar_cod2rev.bat` dans le dossier du jeu, à côté de `main`.
2. Avoir un `server.cfg` dans `main`. [`scripts/server.cfg.example`](scripts/server.cfg.example) règle les options habituelles et liste toutes les autres variables enregistrées par le serveur avec leur valeur par défaut (commentaires en anglais).
3. Lancer `iniciar_cod2rev.bat`.

Le lanceur démarre le serveur sur le port `28970`, pour ne pas entrer en conflit avec le client du jeu qui utilise le `28960` sur le même PC, et charge la carte `mp_toujane`. Par défaut, il démarre en mode réseau local (`dedicated 1`, réseau local ou IP directe) ; lancez `iniciar_cod2rev.bat internet` pour annoncer le serveur dans la liste publique (`dedicated 2`).

### Autorisation des clés

Le serveur interroge `cod2master.activision.com` pour valider la clé de chaque client qui n'est pas sur le réseau local. Avec `sv_cracked 1`, il envoie toujours la requête, mais accepte le client quelle que soit la réponse. Les lanceurs l'activent ; retirez-le de la ligne de commande si vous voulez qu'une clé valide soit exigée.

### Linux

Copiez `cod2rev_lnxded` et `scripts/linux/iniciar_cod2rev.sh` dans le dossier du jeu, à côté de `main`, avec un `server.cfg` dans `main`. Lancez `./iniciar_cod2rev.sh` pour le mode réseau local ou `./iniciar_cod2rev.sh internet` pour annoncer le serveur dans la liste publique. Il utilise le port `28960` et charge la carte `mp_toujane`.

`run_bin.sh` est le script de test du projet d'origine : il démarre `bin/cod2rev_lnxded` en mode développeur avec les triches activées et des bots de test. Il sert d'exemple de paramètres, pas de configuration pour un vrai serveur.

## Tests

`scripts/smoke_test.py` a besoin d'un serveur compilé et d'un dossier du jeu :

```bash
python3 scripts/smoke_test.py /chemin/vers/CoD2
make PROTOCOL=118 && python3 scripts/smoke_test.py /chemin/vers/CoD2 --protocol 118
```

Il démarre le serveur en mode LAN avec un dossier utilisateur temporaire, le dossier du jeu n'est donc que lu. Il vérifie que le serveur charge une carte et répond à `getinfo`, `getstatus` et rcon, et que `scripts/cod2client.py`, un client minimal, peut se connecter, recevoir le gamestate, entrer dans la partie et envoyer des commandes, y compris dans un message découpé en fragments. Utilisez un dossier du jeu avec les `.iwd` d'origine : des mods dans `main` qui remplacent les scripts des modes de jeu peuvent empêcher la carte de se charger.

`scripts/bots_test.py` installe le mod de bots dans un dossier temporaire, joue une carte avec six bots et vérifie que le graphe de waypoints est généré, enregistré puis relu, que les bots suivent des chemins et utilisent la mire, les grenades et le couteau, et qu'aucune erreur de script ne se produit :

```bash
python3 scripts/bots_test.py /chemin/vers/CoD2 --map mp_carentan
```

Avec `--gametype sd` ou `--gametype ctf`, il vérifie plutôt que les bots posent la bombe ou ramassent le drapeau ennemi. `--bombmode 1` joue en SD avec une seule zone de bombe. `--corrupt-cache` part d'un fichier de waypoints endommagé, que les bots doivent remplacer par un nouveau graphe.

Pour chercher des erreurs mémoire, compilez le serveur avec AddressSanitizer dans un dossier séparé et passez ce binaire avec `--binary` :

```bash
make OBJ_ROOT=/tmp/asan/obj BIN_DIR=/tmp/asan/bin \
  CFLAGS="-fno-pie -O1 -g -fno-omit-frame-pointer -mfpmath=387 -DNDEBUG -DPROTOCOL_VERSION=115 -MMD -MP -m32 -D_FILE_OFFSET_BITS=64 -fsanitize=address" \
  LFLAGS="-no-pie -m32 -fsanitize=address"
```

`CFLAGS` donné en ligne de commande remplace celui du Makefile, il faut donc répéter `-m32` et `-D_FILE_OFFSET_BITS=64`. Laisser tourner cette compilation un moment avec le mod de bots et un `sv_mapRotation` court couvre aussi les changements de carte et le jeu.

## Structure du dépôt

| Chemin | Contenu |
| --- | --- |
| `src/server` | Logique du serveur : clients, réseau, rcon, serveurs maîtres. |
| `src/game`, `src/bgame` | Logique de jeu. |
| `src/script` | Machine virtuelle des scripts GSC. |
| `src/qcommon`, `src/universal` | Code commun : commandes, variables (dvars), système de fichiers, réseau. |
| `src/xanim` | Modèles et animations. |
| `src/stringed` | Textes localisés. |
| `src/libcod` | Extensions de script de libcod, avec SQLite et les en-têtes MySQL. |
| `src/win32`, `src/unix` | Code propre à chaque plateforme. |
| `src/zlib` | Copie de zlib. |
| `test` | Matériel de rétro-ingénierie : base de données IDA, tables d'offsets et chargeur de test. |
| `scripts` | Lanceurs pour Windows et Linux, le `server.cfg` d'exemple et son générateur, l'installateur des bots, le test de bon fonctionnement avec son client minimal, et le test des bots. |
| `mods/bots` | Le mod de scripts des bots simples. |

## Licence

[GNU Affero General Public License v3.0](LICENSE), la même que celle du projet d'origine.

## Crédits

- [voron00](https://github.com/voron00/CoD2rev_Server), auteur du projet d'origine
- id Software
- Activision / Infinity Ward
- [CoD4x Server](https://github.com/callofduty4x/CoD4x_Server)
- [OpenBO2](https://github.com/builtbyxeno/OpenBO2)
- [zk_libcod](https://github.com/ibuddieat/zk_libcod) (ibuddieat), où plusieurs des correctifs de ce fork ont été trouvés
