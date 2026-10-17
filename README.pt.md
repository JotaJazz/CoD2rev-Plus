# CoD2rev-Plus

<p align="center"><img src="docs/banner.svg" alt="CoD2rev-Plus" width="100%"></p>

[![Build](https://github.com/JotaJazz/CoD2rev-Plus/actions/workflows/build.yml/badge.svg)](https://github.com/JotaJazz/CoD2rev-Plus/actions/workflows/build.yml)
[![License: AGPL-3.0](https://img.shields.io/badge/license-AGPL--3.0-blue)](LICENSE)
![Platforms: Linux | Windows](https://img.shields.io/badge/platform-Linux%20%7C%20Windows-lightgrey)
![CoD2 1.0 | 1.3](https://img.shields.io/badge/CoD2-1.0%20%7C%201.3-c9b37e)
[![Fork of voron00/CoD2rev_Server](https://img.shields.io/badge/fork%20of-voron00%2FCoD2rev__Server-555)](https://github.com/voron00/CoD2rev_Server)
[![Last commit](https://img.shields.io/github/last-commit/JotaJazz/CoD2rev-Plus)](https://github.com/JotaJazz/CoD2rev-Plus/commits/master)

[Español](README.es.md) · [English](README.md) · **Português** · [Français](README.fr.md) · [Deutsch](README.de.md) · [Русский](README.ru.md)

Código-fonte, obtido por engenharia reversa, do servidor dedicado de **Call of Duty 2** (versões 1.0 e 1.3).
Corrige vários erros dos binários originais e permite estender a funcionalidade do servidor.

Este repositório é um fork de [voron00/CoD2rev_Server](https://github.com/voron00/CoD2rev_Server), onde foi feito todo o trabalho de engenharia reversa. Aqui só são acrescentadas as alterações descritas em [Alterações deste fork](#alterações-deste-fork).

## Transparência

- **Quem mantém isto:** sou o JotaJazz. Gosto de jogos antigos e este fork existe por isso: quero continuar a poder ter um servidor de CoD2 nas minhas próprias máquinas.
- **Como se trabalha aqui:** trabalho com o [Claude Code](https://claude.com/claude-code), o assistente de programação da Anthropic. As alterações deste fork, incluindo este README e as suas traduções, são escritas com a sua ajuda. Os commits feitos assim têm a linha `Co-Authored-By: Claude` na mensagem, pelo que podem ser identificados no histórico com `git log`.
- **Relação com o original:** este fork traz alterações do repositório original quando existem, mas não lhe envia as suas. É provável que com o tempo se afaste dele.
- **O que não é:** não é um projeto oficial nem está associado à Activision, à Infinity Ward ou ao autor do repositório original. Não inclui nenhum ficheiro do jogo.

## Alterações deste fork

- **Compilação para Windows com MinGW** (32 bits), também cruzada a partir do Linux.
- **Palavras-passe fora do log:** `rcon_password` e `sv_privatePassword` aparecem como `********` no registo da consola.
- **Bloqueio de rcon por tentativas falhadas:** depois de várias palavras-passe erradas, o endereço IP é ignorado pelo rcon durante algum tempo. Uma palavra-passe correta reinicia o contador e os endereços `127.x.x.x` nunca são bloqueados.
- **Master servers:**
  - `sv_master2` vem configurado com `master.cod2x.me`, o master da comunidade CoD2x.
  - O formato `host:porta` em `sv_master1`–`sv_master5` agora funciona.
  - Uma falha de DNS é repetida em vez de apagar o master.
- **Versão do jogo selecionável ao compilar:** o servidor pode ser compilado como 1.0 ou como 1.3.
- **Correção de um overflow com clientes Game Pass:** enviar a informação do sistema a um cliente com o protocolo 119 podia exceder um buffer e fazer o servidor falhar.
- **Mensagens grandes de clientes 1.3:** as mensagens fragmentadas enviadas pelos clientes 1.3 eram mal lidas e descartadas; agora são lidas no formato da 1.3.
- **Strings limitadas nas funções de script do libcod:** `printf` e `sprintf` do libcod, as chamadas assíncronas de SQLite e exec e a mudança de nome de jogadores já não escrevem fora dos seus buffers quando um script passa strings longas.
- **Compilação mais simples:** `make` gera diretamente o binário Linux de 32 bits, `make windows` faz a compilação cruzada para Windows e o GitHub Actions compila todas as variantes em cada push.
- **`server.cfg` de exemplo:** `scripts/server.cfg.example` lista todas as variáveis que o servidor regista; `scripts/gen_server_cfg.py` volta a gerá-lo a partir do código.
- **`dedicated` é respeitado:** `+set dedicated 1` era ignorado e todos os servidores se anunciavam aos master servers. Agora `1` mantém o servidor fora da lista pública e `2`, o valor predefinido, anuncia-o.
- **Ficheiros do jogo em discos do Windows:** a compilação de Linux não via os ficheiros do jogo em sistemas de ficheiros com números de inode grandes, como os discos do Windows sob WSL. Agora é compilada com offsets de ficheiro de 64 bits.
- **Relógio de jogo estável em Linux:** o servidor media o tempo com o relógio do sistema, pelo que cada correção da hora (NTP, sincronização de uma máquina virtual) congelava o jogo por um instante. Agora usa o relógio monotónico.
- **Entrada de rede mais robusta:** um jogador ligado podia derrubar o servidor ou sobrescrever a sua memória com uma mensagem comprimida demasiado grande, uma mensagem fragmentada, um nome com `%` (os modos de jogo originais escrevem-no no log), `callvote clientkick` com um número negativo ou pacotes de voz. Agora esses caminhos verificam os limites, e os ficheiros das transferências HTTP interrompidas são fechados.
- **Bots depois de uma mudança de mapa:** ao mudar de mapa, os bots continuavam ligados mas fora da partida, a ocupar o seu slot, pelo que o mod de bots acrescentava outros até encher o servidor. Agora voltam à partida assim que o novo mapa carrega.
- **Endereço do proxy do libcod e transferências:** o endereço real que um proxy local passa na chave `ip` do userinfo só é aceite a partir de `127.0.0.1`, tem de ser um endereço IPv4 e é apagado em cada ligação. Um `donedl` de um jogador que já está na partida já não faz o servidor reenviar o gamestate completo.
- **Transferências e palavras-passe:** a verificação que devia impedir os clientes de transferir os `iw_*.iwd` originais nunca coincidia, pelo que qualquer cliente os podia obter (centenas de MB cada um); agora são recusados, tal como os `.iwd` `_svr_` que são só do servidor, também antes de um redirecionamento HTTP. As transferências já não ficam presas se o mapa mudar a meio, uma mensagem de cliente grande demais para descomprimir expulsa o cliente em vez de ser truncada, e rcon, `g_password` e `sv_privatePassword` são comparados em tempo constante.
- **Banimentos, log de jogo e outras correções encontradas no zk_libcod:** `addip` e `g_banIPs` guardavam uma lista de endereços banidos que nada consultava; agora é aplicada ao ligar, também aos jogadores que chegam por um proxy local. O último endereço de `g_banIPs` já não é ignorado, e um `addip` longo já não transborda a pilha. As quebras de linha em mensagens de chat e nomes de jogador podiam escrever linhas falsas em `games_mp.log` e partir as respostas de `getstatus`; agora são removidas. Uma palavra-passe de rcon errada já não atrasa a correta, um `userinfo` já não engole a mensagem de chat seguinte, e manter o botão de granada sem granadas já não esconde dos outros os disparos e passos do jogador. As entidades de efeito das explosões de granada passam a ser libertadas em vez de se acumularem até ao fim do mapa, um script que apaga uma metralhadora fixa em uso já não termina o mapa, e um nome de mapa longo passado a uma função de script já não para o servidor.
- **Relatórios de falha:** quando o servidor falha, escreve o sinal ou exceção, o endereço da falha e os endereços das chamadas na consola e em `crash.log` dentro de `fs_homepath`. `addr2line -f -e <binário> <endereço>` converte-os em posições no código. Em Linux o processo termina depois com o mesmo sinal de antes, pelo que os core dumps continuam a funcionar.
- **Opções de log e administração:** o log da consola pode mudar de nome (`logfileName`), guardar os logs de execuções anteriores (`logfileRotate`) e começar cada linha com a data e hora (`logTimestamps`). Os comandos rcon aceites podem ficar fora do log (`sv_logRcon`), o servidor de autorização de CD keys pode ser alterado (`sv_authorizeServer`, `sv_authorizePort`), o servidor pode impor uma taxa e uma frequência de snapshots a todos os clientes (`g_forceRate`, `g_forceSnaps`), e as mensagens que anunciam uma expulsão ou um timeout podem ser desligadas (`sv_kickMessages`, `sv_timeoutMessages`). Os nomes são os mesmos do zk_libcod, pelo que as suas configs continuam a servir.
- **Mais callbacks e métodos de jogador do libcod (do zk_libcod, mesmos nomes):** `_callbacksetup.gsc` pode definir `CodeCallback_Error(critical, function, message)` (erros de runtime de scripts, entregues no frame seguinte), `CodeCallback_HitchWarning(msec)`, `CodeCallback_Pickup(type, weapon, name, quantity, clip, origin, angles)` (substitui a apanha original), `CodeCallback_Suicide()` (substitui `suicide()`; `processSuicide()` mata), `CodeCallback_WeaponChange(index, name)` e um callback por cada botão premido: `Attack`, `Melee`, `Use`, `Reload`, `LeanLeft`, `LeanRight`, `Prone`, `Crouch`, `Stand`, `Ads`, `MeleeBreath`, `HoldBreath`, `Frag` e `Smoke` (`CodeCallback_<nome>Button()`). Novos métodos de jogador: `isFiring`, `isMeleeing`, `isReloading`, `isThrowingGrenade`, `isUsingBinoculars`, `isRechambering`, `isShellShocked`, `isChatting`, `holdBreathButtonPressed`, `get`/`setCurrentWeaponAmmo`, `get`/`setCurrentWeaponClipAmmo`, `getCurrentWeaponSlot`, `get`/`setSpeed`, `get`/`setGravity`, `getGroundEntity`, `getViewOrigin`, `getPlayerstateFlags`, `getInactivityTime`, `getEnterTime` e `processSuicide`. Ao contrário do zk_libcod, os callbacks de troca de arma e de erro nunca correm dentro do movimento do jogador nem da VM de scripts, e um `suicide()` dentro de `CodeCallback_Suicide` não entra em ciclo.
- **Grafos de waypoints com A\* e mais funções de script (do zk_libcod):** `graphCreate`, `graphAddNode`, `graphAddEdge`, `graphFindPath`, `graphFindClosestNode`, `graphAutodiscover`, `graphSave`/`graphLoad` e as restantes funções `graph*` do zk_libcod permitem que os scripts de bots naveguem num mapa (`src/libcod/gsc_graph.cpp`, portado sob a sua licença MIT). Ao contrário do zk_libcod, os grafos também são apagados num `map_restart` rápido (cada ronda de SD), um id ou ficheiro inválido devolve undefined sem terminar a thread do script, `graphLoad` recusa ficheiros com mais de 64 MB e `graphSave` recusa nomes `.iwd`, `.cfg` e `.gsc`. Também são novas: `abs`, `floor`, `ceil`, `pow`, `atan2`, `roundTo`, `getMilliseconds`, `getMicroseconds`, `toHex`, `fromHex`, `ord`, `chr`, `getEntityCount`, `logPrintConsole` e os métodos `getUserinfo`/`setUserinfo`; `bulletTrace` e `bulletTracePassed` aceitam uma máscara de conteúdos opcional, e `bulletTrace` também devolve `flags`, `contents` e `material`; com `sv_botUseTriggerUse 1`, um bot dentro de um trigger de uso (como uma zona de bomba de SD) recebe `self waittill("bot_trigger", trigger)`.
- **Teste de fumo:** `scripts/smoke_test.py` inicia o servidor de Linux com uma pasta real do jogo e verifica que carrega um mapa, responde às consultas e deixa entrar um cliente mínimo (`scripts/cod2client.py`) que recebe o gamestate, entra na partida e envia comandos, também em mensagens fragmentadas.
- **Bots simples:** `mods/bots` é um pequeno mod de scripts cujos bots percorrem cada mapa sobre um grafo de waypoints construído com as funções `graph*` do libcod: na primeira vez que um mapa é jogado o grafo é gerado a partir dos seus pontos de spawn e guardado como `waypoints/<mapa>_v1.graph` na pasta do mod, e os carregamentos seguintes leem-no. Sem ninguém à vista seguem o que viram ou ouviram disparar por último, patrulham em direção ao lado inimigo e só após 20 s sem contacto vão até ao inimigo mais próximo. Apontam com a mira à distância, lançam granadas a média distância, usam a faca de perto e recarregam entre combates. `scripts/install_bots.py` instala-o; `bots_count`, `bots_skill` e `bots_debug` controlam-nos.
- **Mais botões para os bots no libcod:** os métodos de script `useButton`, `throwSmoke` e `holdBreath` fazem um bot carregar em usar (para plantar ou desativar a bomba), lançar uma granada de fumo ou suster a respiração, tal como os já existentes `fireWeapon` e `throwNade`. Recebem `1` para carregar e `0` para soltar.
- **Menos bloqueios e um arranque mais limpo:** o nome do master server é resolvido uma única vez em vez de em cada espera de autorização esgotada, e o servidor já não avisa por cada `+set` da linha de comandos.
- **Lançadores:** `scripts/windows/iniciar_cod2rev.bat` e `scripts/linux/iniciar_cod2rev.sh`, com um modo LAN e um modo internet.

### Variáveis novas

| Variável | Predefinição | Intervalo | Descrição |
| --- | --- | --- | --- |
| `sv_rconMaxAttempts` | `5` | 0–100 | Palavras-passe de rcon erradas permitidas por IP antes do bloqueio. `0` desativa o bloqueio. |
| `sv_rconLockTime` | `600` | 0–86400 | Duração do bloqueio em segundos. É também a janela em que as tentativas são contadas. |
| `logfileName` | `""` | — | Ficheiro do log da consola, em `fs_homepath/<mod>`. Vazio usa `console_mp_server.log`; nomes com `..` ou um caminho completo são ignorados. |
| `logfileRotate` | `0` | 0–1000 | Logs de execuções anteriores guardados ao iniciar, como `<nome>.1` (o mais recente) a `<nome>.N`. |
| `logTimestamps` | `0` | 0–1 | `1` começa cada linha do log da consola com a data e hora. |
| `sv_logRcon` | `1` | 0–1 | `0` deixa fora do log os comandos rcon aceites. As palavras-passe erradas são sempre registadas. |
| `sv_authorizeServer` | `cod2master.activision.com` | — | Servidor de autorização de CD keys. Um valor novo é usado a partir da ligação seguinte. |
| `sv_authorizePort` | `20700` | 1–65535 | Porta do servidor de autorização. |
| `g_forceRate` | `0` | 0–25000 | Taxa dada a todos os clientes em vez da sua (mínimo 1000). `0` mantém a do cliente. |
| `g_forceSnaps` | `0` | 0–30 | Snapshots por segundo dados a todos os clientes. `0` mantém os do cliente. |
| `sv_kickMessages` | `1` | 0–1 | `0` esconde a mensagem que avisa todos de que um jogador foi expulso. |
| `sv_timeoutMessages` | `1` | 0–1 | `0` esconde a mensagem que avisa todos de que um jogador perdeu a ligação por timeout. |
| `sv_botUseTriggerUse` | `0` | 0–1 | `1` envia ao bot um notify `bot_trigger`, com o trigger, enquanto está dentro de um trigger de uso que poderia usar. |
| `g_debugGraph` | `0` | 0–1 | `1` imprime mensagens de depuração das funções de grafos. |
| `sv_graphAstarMaxIterations` | `0` | 0–100000 | Máximo de nós que um `graphFindPath` expande. `0` significa sem limite. |
| `sv_graphAstarWeight` | `1` | 1–2 | Peso da heurística do A\*. Acima de `1`, os caminhos são encontrados mais depressa mas podem ser mais longos. |
| `sv_graphMaxGraphs` | `8` | 1–64 | Máximo de grafos em simultâneo. |
| `sv_graphMaxNodes` | `8192` | 64–65535 | Máximo de nós num grafo. |

## Versões do jogo

- **A versão é escolhida ao compilar.** Por predefinição o servidor é compilado como 1.0 (protocolo 115); com `PROTOCOL=118` é compilado como 1.3. O servidor anuncia essa versão aos master servers, pelo que aparece na lista de servidores dos clientes dessa mesma versão.
- **A compilação 1.0 aceita também outras versões por IP direto.** Com o libcod, que vem ativado por predefinição, admite ligações com protocolos de 115 a 119, o que inclui a 1.3 (protocolo 118) e a versão do Game Pass (protocolo 119). A ligação por IP direto a esta compilação foi testada, também com um cliente CoD2x 1.4.x.
- **A compilação 1.3 só aceita clientes com o protocolo 118.** Foi iniciada em Linux e em Windows, onde carrega um mapa e responde como 1.3, e na compilação de Linux já se jogou com um cliente CoD2x 1.4. Os clientes [CoD2x](https://github.com/callofduty2x/CoD2x) 1.4 também se ligam com o protocolo 118, e a sua lista de servidores só mostra os que informam o protocolo 118 ou 120, pelo que esta é a compilação que conseguem ver.

## Compilar

O resultado fica em `bin/` e os objetos intermédios em `obj/`.

### Windows (cruzado a partir do Linux com MinGW)

Precisa do compilador MinGW-w64 de 32 bits (`i686-w64-mingw32`).

```bash
make windows
```

Gera `bin/cod2rev_win32.exe`.

### Linux

O servidor é um binário x86 de 32 bits. Num sistema de 64 bits é necessário o suporte de 32 bits do GCC (em Debian e Ubuntu, os pacotes `gcc-multilib` e `g++-multilib`):

```bash
make
```

Gera `bin/cod2rev_lnxded`.

### Versão do jogo

Acrescente `PROTOCOL=118` ao comando `make` para compilar o servidor como 1.3; sem ele, ou com `PROTOCOL=115`, é compilado como 1.0. Cada plataforma e versão guarda os seus próprios objetos em `obj/`, pelo que pode mudar de uma para a outra sem `make clean`. A compilação automática publica as duas versões para cada plataforma.

### Compilação automática

Cada push para `master` que altere o código compila as duas versões no GitHub Actions ([`.github/workflows/build.yml`](.github/workflows/build.yml)). Os binários resultantes podem ser descarregados como artefactos no separador [Actions](https://github.com/JotaJazz/CoD2rev-Plus/actions) do repositório.

### Opções do Makefile

| Opção | Predefinição | Descrição |
| --- | --- | --- |
| `WITH_LIBCOD` | `true` | Inclui as extensões de script do libcod (bots, jogadores, entidades, armas, utilitários, etc.). |
| `WITH_SQLITE` | `true` | Funções de script para SQLite. |
| `WITH_MYSQL` | `false` | Funções de script para MySQL. Requer a biblioteca cliente do MySQL. |

`make clean` apaga o binário e os objetos.

## Executar

O servidor precisa dos ficheiros do jogo original (a pasta `main` com os seus `.iwd`), que não estão neste repositório. É preciso ter uma cópia própria de Call of Duty 2.

### Windows

1. Copiar `cod2rev_win32.exe` e `scripts/windows/iniciar_cod2rev.bat` para a pasta do jogo, junto a `main`.
2. Ter um `server.cfg` em `main`. [`scripts/server.cfg.example`](scripts/server.cfg.example) configura as opções habituais e lista as restantes variáveis que o servidor regista com o seu valor predefinido (comentários em inglês).
3. Executar `iniciar_cod2rev.bat`.

O lançador inicia o servidor na porta `28970`, para não colidir com o cliente do jogo que usa a `28960` no mesmo PC, e carrega o mapa `mp_toujane`. Por predefinição inicia em modo LAN (`dedicated 1`, LAN ou IP direto); execute `iniciar_cod2rev.bat internet` para anunciar o servidor na lista pública (`dedicated 2`).

### Autorização de chaves

O servidor consulta `cod2master.activision.com` para validar a chave de cada cliente que não esteja na rede local. Com `sv_cracked 1` continua a fazer a consulta, mas aceita o cliente seja qual for a resposta. Os lançadores ativam-no; retire-o da linha de comandos se quiser que seja exigida uma chave válida.

### Linux

Copie `cod2rev_lnxded` e `scripts/linux/iniciar_cod2rev.sh` para a pasta do jogo, junto a `main`, com um `server.cfg` em `main`. Execute `./iniciar_cod2rev.sh` para o modo LAN ou `./iniciar_cod2rev.sh internet` para anunciar o servidor na lista pública. Usa a porta `28960` e carrega o mapa `mp_toujane`.

`run_bin.sh` é o script de testes do projeto original: inicia `bin/cod2rev_lnxded` em modo de desenvolvimento com cheats ativados e bots de teste. Serve de exemplo de parâmetros, não de configuração para um servidor real.

## Testes

`scripts/smoke_test.py` precisa de um servidor compilado e de uma pasta do jogo:

```bash
python3 scripts/smoke_test.py /caminho/para/CoD2
make PROTOCOL=118 && python3 scripts/smoke_test.py /caminho/para/CoD2 --protocol 118
```

Inicia o servidor em modo LAN com uma pasta de utilizador temporária, pelo que a pasta do jogo só é lida. Verifica que o servidor carrega um mapa e responde a `getinfo`, `getstatus` e rcon, e que `scripts/cod2client.py`, um cliente mínimo, consegue ligar-se, receber o gamestate, entrar na partida e enviar comandos, também numa mensagem dividida em fragmentos. Use uma pasta do jogo com os `.iwd` originais: mods em `main` que substituem os scripts dos modos de jogo podem impedir que o mapa carregue.

`scripts/bots_test.py` instala o mod de bots numa pasta temporária, joga um mapa com seis bots e verifica que o grafo de waypoints é gerado, guardado e lido de novo, que os bots seguem rotas e usam a mira, as granadas e a faca, e que não há erros de script:

```bash
python3 scripts/bots_test.py /caminho/para/CoD2 --map mp_carentan
```

Para procurar erros de memória, compile o servidor com AddressSanitizer numa pasta à parte e passe esse binário com `--binary`:

```bash
make OBJ_ROOT=/tmp/asan/obj BIN_DIR=/tmp/asan/bin \
  CFLAGS="-fno-pie -O1 -g -fno-omit-frame-pointer -mfpmath=387 -DNDEBUG -DPROTOCOL_VERSION=115 -MMD -MP -m32 -D_FILE_OFFSET_BITS=64 -fsanitize=address" \
  LFLAGS="-no-pie -m32 -fsanitize=address"
```

`CFLAGS` na linha de comandos substitui o do Makefile, pelo que é preciso repetir `-m32` e `-D_FILE_OFFSET_BITS=64`. Deixar essa compilação a correr algum tempo com o mod de bots e um `sv_mapRotation` curto cobre também as mudanças de mapa e a partida.

## Estrutura do repositório

| Caminho | Conteúdo |
| --- | --- |
| `src/server` | Lógica do servidor: clientes, rede, rcon, master servers. |
| `src/game`, `src/bgame` | Lógica de jogo. |
| `src/script` | Máquina virtual de scripts GSC. |
| `src/qcommon`, `src/universal` | Código comum: comandos, variáveis (dvars), sistema de ficheiros, rede. |
| `src/xanim` | Modelos e animações. |
| `src/stringed` | Textos localizados. |
| `src/libcod` | Extensões de script do libcod, com SQLite e os cabeçalhos do MySQL. |
| `src/win32`, `src/unix` | Código específico de cada plataforma. |
| `src/zlib` | Cópia do zlib. |
| `test` | Material da engenharia reversa: base de dados do IDA, tabelas de offsets e carregador de testes. |
| `scripts` | Lançadores para Windows e Linux, o `server.cfg` de exemplo e o seu gerador, o instalador dos bots, o teste de fumo com o seu cliente mínimo, e o teste dos bots. |
| `mods/bots` | O mod de scripts dos bots simples. |

## Licença

[GNU Affero General Public License v3.0](LICENSE), a mesma do projeto original.

## Créditos

- [voron00](https://github.com/voron00/CoD2rev_Server), autor do projeto original
- id Software
- Activision / Infinity Ward
- [CoD4x Server](https://github.com/callofduty4x/CoD4x_Server)
- [OpenBO2](https://github.com/builtbyxeno/OpenBO2)
- [zk_libcod](https://github.com/ibuddieat/zk_libcod) (ibuddieat), onde foram encontradas várias das correções deste fork
