# Glossary

| Term | Meaning |
|---|---|
| 1.0 / 1.3 | CoD2 patch levels; network protocols 115 and 118 |
| ADR | Architecture decision record, in `docs/architecture/adr` |
| antilag | Server rewinds player positions to the shooter's view time when tracing bullets |
| ASan | AddressSanitizer build, used for memory error runs |
| BSP | Compiled map file (`maps/mp/*.d3dbsp` in the iwds) |
| callback | GSC function the engine calls (`CodeCallback_*`) |
| CoD2x | Community client and server extension for CoD2 1.3/1.4; its 1.4 client connects with protocol 118 |
| configstring | Indexed string the server sends to all clients (models, sounds, server info) |
| CRLF / LF | Line endings; this repo keeps each file's own |
| DObj | Runtime skeleton instance of a model, built from XModels and animations (`src/xanim/dobj.cpp`) |
| dvar | Engine variable (`set`, `seta`, `+set` on the command line) |
| entity | Game object: player, script model, trigger…; `gentity_t` in game, `entityState_t` on the wire |
| fragment | Part of a message larger than one packet, reassembled by the netchan |
| `fs_game`, `fs_homepath` | Mod folder and the folder the server writes to |
| Game Pass | Microsoft Store edition; network protocol 119 |
| gamestate | First message to a client: configstrings and entity baselines |
| gametype | dm (Deathmatch), tdm (Team Deathmatch), sd (Search and Destroy), ctf (Capture the Flag), hq (Headquarters); GSC scripts in `maps/mp/gametypes` |
| graph | Waypoint graph for bot navigation (`graph*` builtins, A\*) |
| GSC | Game script language; compiled and run by `src/script` |
| iwd | Zip archive with game files, in `main/` or a mod folder |
| libcod | Script extension layer, `src/libcod` |
| `LISTEN_SERVER` | Build define for running this server inside a client |
| master server | Lists public servers; receives heartbeats |
| netchan | Sequenced, fragmenting channel over UDP (`src/qcommon/net_chan_mp.cpp`) |
| PID | Process id; test servers are stopped by the PID they started with |
| playerstate | Full state of the client's own player in a snapshot |
| protocol | Network protocol number a client sends on connect (115–119) |
| pure (`sv_pure`) | Server check that clients only use the server's iwds |
| rcon | Remote console over UDP, password protected |
| reversed code | Code reconstructed from the original binaries; mirrors them |
| snapshot | Periodic message with the playerstate and visible entities, delta-compressed |
| stage | Entry of `docs/ai/PROJECT_MEMORY.md` |
| usercmd | Client input for one frame: buttons, angles, movement |
| XAnim | Animation data and blending (`src/xanim/xanim.cpp`) |
| XModel | Model with its bones and hit locations (`src/xanim/xmodel.cpp`) |
| zk_libcod | ibuddieat's libcod for the stock 1.3 binary (MIT), source of most script functions |
| zPAM | Competitive match mod for CoD2 |
