# CoD2rev_Server (fork de JotaJazz)

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

- **Compilation pour Windows avec MinGW** (32 bits), y compris en compilation croisée depuis Linux.
- **Mots de passe absents du journal :** `rcon_password` et `sv_privatePassword` apparaissent sous la forme `********` dans le journal de la console.
- **Blocage de rcon après des tentatives échouées :** après plusieurs mots de passe erronés, l'adresse IP est ignorée par rcon pendant un certain temps. Un mot de passe correct remet le compteur à zéro et les adresses `127.x.x.x` ne sont jamais bloquées.
- **Serveurs maîtres :**
  - `sv_master2` est préconfiguré avec `master.cod2x.me`, le serveur maître de la communauté CoD2x.
  - Le format `hôte:port` dans `sv_master1`–`sv_master5` fonctionne désormais.
  - Un échec DNS donne lieu à une nouvelle tentative au lieu d'effacer le serveur maître.
- **Version du jeu sélectionnable à la compilation :** le serveur peut être compilé en 1.0 ou en 1.3.
- **Correction d'un débordement avec les clients Game Pass :** l'envoi des informations système à un client utilisant le protocole 119 pouvait faire déborder un tampon et planter le serveur.
- **Messages volumineux des clients 1.3 :** les messages fragmentés envoyés par les clients 1.3 étaient mal lus et rejetés ; ils sont désormais lus au format de la 1.3.
- **Chaînes bornées dans les fonctions de script de libcod :** `printf` et `sprintf` de libcod, les appels asynchrones SQLite et exec et le renommage des joueurs n'écrivent plus au-delà de leurs tampons quand un script passe de longues chaînes.
- **Compilation simplifiée :** `make` produit directement le binaire Linux 32 bits, `make windows` fait la compilation croisée pour Windows et GitHub Actions compile toutes les variantes à chaque push.
- **`server.cfg` d'exemple :** `scripts/server.cfg.example` liste toutes les variables enregistrées par le serveur ; `scripts/gen_server_cfg.py` le régénère à partir du code.
- **Lanceurs :** `scripts/windows/iniciar_cod2rev.bat` et `scripts/linux/iniciar_cod2rev.sh`, avec un mode réseau local et un mode internet.

### Nouvelles variables

| Variable | Valeur par défaut | Plage | Description |
| --- | --- | --- | --- |
| `sv_rconMaxAttempts` | `5` | 0–100 | Nombre de mots de passe rcon erronés autorisés par IP avant le blocage. `0` désactive le blocage. |
| `sv_rconLockTime` | `600` | 0–86400 | Durée du blocage en secondes. C'est aussi la fenêtre pendant laquelle les tentatives sont comptées. |

## Versions du jeu

- **La version se choisit à la compilation.** Par défaut, le serveur est compilé en 1.0 (protocole 115) ; avec `PROTOCOL=118`, il est compilé en 1.3. Le serveur annonce cette version aux serveurs maîtres, il apparaît donc dans la liste de serveurs des clients de cette même version.
- **La compilation 1.0 accepte aussi d'autres versions par IP directe.** Avec libcod, activé par défaut, elle accepte les connexions utilisant les protocoles 115 à 119, ce qui inclut la 1.3 (protocole 118) et la version Game Pass (protocole 119). La connexion par IP directe à cette compilation a été testée, y compris avec un client CoD2x 1.4.x.
- **La compilation 1.3 n'accepte que les clients au protocole 118.** Elle compile pour Linux et Windows, mais n'a pas encore été testée en jeu. Les clients [CoD2x](https://github.com/callofduty2x/CoD2x) 1.4 se connectent eux aussi avec le protocole 118, et leur liste de serveurs n'affiche que ceux qui annoncent le protocole 118 ou 120 ; c'est donc cette compilation qu'ils peuvent voir.

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

Chaque push sur `master` qui modifie le code compile les deux versions dans GitHub Actions ([`.github/workflows/build.yml`](.github/workflows/build.yml)). Les binaires obtenus peuvent être téléchargés comme artefacts depuis l'onglet [Actions](https://github.com/JotaJazz/CoD2rev_Server/actions) du dépôt.

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
2. Avoir un `server.cfg` dans `main`. [`scripts/server.cfg.example`](scripts/server.cfg.example) règle les options habituelles et liste toutes les autres variables enregistrées par le serveur avec leur valeur par défaut (commentaires en anglais).
3. Lancer `iniciar_cod2rev.bat`.

Le lanceur démarre le serveur sur le port `28970`, pour ne pas entrer en conflit avec le client du jeu qui utilise le `28960` sur le même PC, et charge la carte `mp_toujane`. Par défaut, il démarre en mode réseau local (`dedicated 1`, réseau local ou IP directe) ; lancez `iniciar_cod2rev.bat internet` pour annoncer le serveur dans la liste publique (`dedicated 2`).

### Autorisation des clés

Le serveur interroge `cod2master.activision.com` pour valider la clé de chaque client qui n'est pas sur le réseau local. Avec `sv_cracked 1`, il envoie toujours la requête, mais accepte le client quelle que soit la réponse. Les lanceurs l'activent ; retirez-le de la ligne de commande si vous voulez qu'une clé valide soit exigée.

### Linux

Copiez `cod2rev_lnxded` et `scripts/linux/iniciar_cod2rev.sh` dans le dossier du jeu, à côté de `main`, avec un `server.cfg` dans `main`. Lancez `./iniciar_cod2rev.sh` pour le mode réseau local ou `./iniciar_cod2rev.sh internet` pour annoncer le serveur dans la liste publique. Il utilise le port `28960` et charge la carte `mp_toujane`.

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
| `scripts` | Lanceurs pour Windows et Linux, le `server.cfg` d'exemple et son générateur. |

## Licence

[GNU Affero General Public License v3.0](LICENSE), la même que celle du projet d'origine.

## Crédits

- [voron00](https://github.com/voron00/CoD2rev_Server), auteur du projet d'origine
- id Software
- Activision / Infinity Ward
- [CoD4x Server](https://github.com/callofduty4x/CoD4x_Server)
- [OpenBO2](https://github.com/builtbyxeno/OpenBO2)
