# CoD2rev_Server (fork do JotaJazz)

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
- **Teste de fumo:** `scripts/smoke_test.py` inicia o servidor de Linux com uma pasta real do jogo e verifica que carrega um mapa e responde às consultas.
- **Bots simples:** `mods/bots` é um pequeno mod de scripts cujos bots caminham para o inimigo mais próximo e disparam sobre o que veem, usando as funções de bots do libcod. Não usam pontos de rota. `scripts/install_bots.py` instala-o; `bots_count` e `bots_skill` controlam-nos.
- **Menos bloqueios e um arranque mais limpo:** o nome do master server é resolvido uma única vez em vez de em cada espera de autorização esgotada, e o servidor já não avisa por cada `+set` da linha de comandos.
- **Lançadores:** `scripts/windows/iniciar_cod2rev.bat` e `scripts/linux/iniciar_cod2rev.sh`, com um modo LAN e um modo internet.

### Variáveis novas

| Variável | Predefinição | Intervalo | Descrição |
| --- | --- | --- | --- |
| `sv_rconMaxAttempts` | `5` | 0–100 | Palavras-passe de rcon erradas permitidas por IP antes do bloqueio. `0` desativa o bloqueio. |
| `sv_rconLockTime` | `600` | 0–86400 | Duração do bloqueio em segundos. É também a janela em que as tentativas são contadas. |

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

Cada push para `master` que altere o código compila as duas versões no GitHub Actions ([`.github/workflows/build.yml`](.github/workflows/build.yml)). Os binários resultantes podem ser descarregados como artefactos no separador [Actions](https://github.com/JotaJazz/CoD2rev_Server/actions) do repositório.

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
| `scripts` | Lançadores para Windows e Linux, o `server.cfg` de exemplo e o seu gerador. |

## Licença

[GNU Affero General Public License v3.0](LICENSE), a mesma do projeto original.

## Créditos

- [voron00](https://github.com/voron00/CoD2rev_Server), autor do projeto original
- id Software
- Activision / Infinity Ward
- [CoD4x Server](https://github.com/callofduty4x/CoD4x_Server)
- [OpenBO2](https://github.com/builtbyxeno/OpenBO2)
