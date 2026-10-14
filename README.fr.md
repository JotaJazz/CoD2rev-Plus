# CoD2rev_Server (fork de JotaJazz)

[Español](README.md) · [English](README.en.md) · [Português](README.pt.md) · **Français** · [Deutsch](README.de.md) · [Русский](README.ru.md)

Code source, obtenu par rétro-ingénierie, du serveur dédié de **Call of Duty 2** (version 1.0, protocole 115).
Il corrige plusieurs bogues des binaires d'origine et permet d'étendre les fonctionnalités du serveur.

Ce dépôt est un fork de [voron00/CoD2rev_Server](https://github.com/voron00/CoD2rev_Server), où tout le travail de rétro-ingénierie a été réalisé. Ce fork n'ajoute que les modifications décrites dans [Modifications de ce fork](#modifications-de-ce-fork).

## Transparence

- **Qui maintient ce dépôt :** je suis JotaJazz. J'aime les vieux jeux, et c'est la raison d'être de ce fork : je veux pouvoir continuer à faire tourner un serveur CoD2 sur mes propres machines.
- **Comment le travail est fait :** je travaille avec [Claude Code](https://claude.com/claude-code), l'assistant de programmation d'Anthropic. Les modifications de ce fork, y compris ce README et ses traductions, sont écrites avec son aide. Les commits réalisés ainsi portent la ligne `Co-Authored-By: Claude` dans leur message, ce qui permet de les identifier dans l'historique avec `git log`.
- **Relation avec l'original :** ce fork récupère les modifications du dépôt d'origine quand il y en a, mais ne lui envoie pas les siennes. Il s'en éloignera probablement avec le temps.
- **Ce que ce n'est pas :** ce n'est pas un projet officiel et il n'est affilié ni à Activision, ni à Infinity Ward, ni à l'auteur du dépôt d'origine. Il ne contient aucun fichier du jeu.

## Modifications de ce fork

- **Compilation pour Windows avec MinGW** (32 bits), y compris en compilation croisée depuis Linux.
- **Mots de passe absents du journal :** `rcon_password` et `sv_privatePassword` apparaissent sous la forme `********` dans le journal de la console.
- **Blocage de rcon après des tentatives échouées :** après plusieurs mots de passe erronés, l'adresse IP est ignorée par rcon pendant un certain temps. Un mot de passe correct remet le compteur à zéro et les adresses `127.x.x.x` ne sont jamais bloquées.
- **Serveurs maîtres :**
  - `sv_master2` est préconfiguré avec `master.cod2x.me`, le serveur maître de la communauté CoD2x.
  - Le format `hôte:port` dans `sv_master1`–`sv_master5` fonctionne désormais.
  - Un échec DNS donne lieu à une nouvelle tentative au lieu d'effacer le serveur maître.
- **Lanceur pour Windows :** `scripts/windows/iniciar_cod2rev.bat`.

### Nouvelles variables

| Variable | Valeur par défaut | Plage | Description |
| --- | --- | --- | --- |
| `sv_rconMaxAttempts` | `5` | 0–100 | Nombre de mots de passe rcon erronés autorisés par IP avant le blocage. `0` désactive le blocage. |
| `sv_rconLockTime` | `600` | 0–86400 | Durée du blocage en secondes. C'est aussi la fenêtre pendant laquelle les tentatives sont comptées. |

## Versions du jeu

- **Le serveur s'identifie comme 1.0** (protocole 115). C'est ce qu'il annonce aux serveurs maîtres et dans ses réponses d'information.
- **Il accepte les clients d'autres versions.** Avec libcod, activé par défaut, il accepte les connexions utilisant les protocoles 115 à 119. Cela inclut la 1.3 (protocole 118) et la version Game Pass (protocole 119), avec des ajustements par client dans le code réseau.
- **Non testé avec de vrais clients dans ce fork.** Ce qui précède vient de la lecture du code. Comme le serveur s'annonce avec le protocole 115, il est possible qu'un client d'une autre version ne le voie pas dans sa liste de serveurs et doive se connecter par IP.

## Compilation

Le résultat est placé dans `bin/` et les objets intermédiaires dans `obj/`.

### Windows (compilation croisée depuis Linux avec MinGW)

Nécessite le compilateur MinGW-w64 32 bits (`i686-w64-mingw32`).

```bash
mkdir -p bin obj
make OS=Windows_NT CC=i686-w64-mingw32-gcc-posix WINDRES=i686-w64-mingw32-windres bin/cod2rev_win32.exe
```

Produit `bin/cod2rev_win32.exe`.

### Linux

Le serveur est un binaire x86 32 bits. Sur un système 64 bits, il faut la prise en charge 32 bits de GCC (sous Debian et Ubuntu, les paquets `gcc-multilib` et `g++-multilib`) :

```bash
make CC="gcc -m32"
```

Produit `bin/cod2rev_lnxded`.

### Compilation automatique

Chaque push sur `master` compile les deux versions dans GitHub Actions ([`.github/workflows/build.yml`](.github/workflows/build.yml)). Les binaires obtenus peuvent être téléchargés comme artefacts depuis l'onglet [Actions](https://github.com/JotaJazz/CoD2rev_Server/actions) du dépôt.

### Options du Makefile

| Option | Valeur par défaut | Description |
| --- | --- | --- |
| `WITH_LIBCOD` | `true` | Inclut les extensions de script de libcod (bots, joueurs, entités, armes, utilitaires, etc.). |
| `WITH_SQLITE` | `true` | Fonctions de script pour SQLite. |
| `WITH_MYSQL` | `false` | Fonctions de script pour MySQL. Nécessite la bibliothèque cliente MySQL. |

`make clean` supprime le binaire et les objets.

## Exécution

Le serveur a besoin des fichiers du jeu d'origine (le dossier `main` avec ses fichiers `.iwd`), qui ne sont pas dans ce dépôt. Il faut posséder sa propre copie de Call of Duty 2.

### Windows

1. Copier `cod2rev_win32.exe` et `scripts/windows/iniciar_cod2rev.bat` dans le dossier du jeu, à côté de `main`.
2. Avoir un `server.cfg` dans `main`.
3. Lancer `iniciar_cod2rev.bat`.

Le lanceur démarre le serveur sur le port `28970`, pour ne pas entrer en conflit avec le client du jeu qui utilise le `28960` sur le même PC, et charge la carte `mp_toujane`. Il utilise `dedicated 1` (réseau local ou IP directe) ; avec `dedicated 2`, le serveur est annoncé dans la liste publique.

### Autorisation des clés

Le serveur interroge `cod2master.activision.com` pour valider la clé de chaque client qui n'est pas sur le réseau local. Avec `sv_cracked 1`, il envoie toujours la requête, mais accepte le client quelle que soit la réponse. Le lanceur Windows l'active ; retirez-le de la ligne de commande si vous voulez qu'une clé valide soit exigée.

### Linux

`run_bin.sh` est le script de test du projet d'origine : il démarre `bin/cod2rev_lnxded` en mode développeur avec les triches activées et des bots de test. Il sert d'exemple de paramètres, pas de configuration pour un vrai serveur.

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
| `scripts/windows` | Lanceur pour Windows. |

## Licence

[GNU Affero General Public License v3.0](LICENSE), la même que celle du projet d'origine.

## Crédits

- [voron00](https://github.com/voron00/CoD2rev_Server), auteur du projet d'origine
- id Software
- Activision / Infinity Ward
- [CoD4x Server](https://github.com/callofduty4x/CoD4x_Server)
- [OpenBO2](https://github.com/builtbyxeno/OpenBO2)
