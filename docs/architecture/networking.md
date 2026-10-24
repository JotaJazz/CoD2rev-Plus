# Networking

## Path of a packet

```
Com_EventLoop                    src/qcommon/common.cpp (sockets: src/unix/unix_net.cpp, src/win32/win_net.cpp)
└─ SV_PacketEvent                src/server/sv_main_mp.cpp
   ├─ SV_ConnectionlessPacket    getinfo, getstatus, getchallenge, connect, rcon, ipAuthorize…
   │  ├─ SV_DirectConnect        src/server/sv_client_mp.cpp
   │  └─ SVC_RemoteCommand       src/server/sv_main_pc_mp.cpp
   └─ Netchan_Process            sequence, fragments
      └─ SV_ExecuteClientMessage src/server/sv_client_mp.cpp: commands, usercmds, downloads
```

Outgoing: `SV_SendClientMessages` → `SV_BuildClientSnapshot`
(`src/server/sv_snapshot_mp.cpp`) → `Netchan_Transmit` /
`Netchan_TransmitNextFragment`. Bit and byte encoding, delta compression and
Huffman: `src/qcommon/msg_mp.cpp`.

## Protocols

| Protocol | Client | Build that accepts it |
|---|---|---|
| 115 | 1.0 | 1.0 build |
| 116, 117 | — | 1.0 build with libcod (range check only, untested) |
| 118 | 1.3, CoD2x 1.4 | 1.3 build; 1.0 build with libcod |
| 119 | Game Pass | 1.0 build with libcod (untested here) |

- The build's own version is `PROTOCOL_VERSION`; it is what the server
  announces to the master servers.
- The 1.0 build with libcod keeps the client's protocol in
  `netchan.protocol` and switches formats per client: fragment offsets are a
  long for 118 and a short otherwise (`Netchan_Process`,
  `Netchan_TransmitNextFragment`), and 119 has one stat less in the
  playerstate (`src/qcommon/msg_mp.cpp`).
- `MAX_MSGLEN` is `0x20000` in the 1.3 build and `0x4000` in the 1.0 build;
  packets are at most `MAX_PACKETLEN` (1400) bytes, fragments `FRAGMENT_SIZE`
  (1300).

## Hardening rules

The fixes are listed in [../CHANGES.md](../CHANGES.md#security-and-stability).
New code that reads from the network follows them:

- Every length read from a message is checked against the destination buffer
  before it is used (compressed sizes, fragment offsets and lengths, strings,
  voice packets).
- Connectionless commands that answer with data (`getinfo`, `getstatus`,
  `getchallenge`, `rcon`) go through `SVC_RateLimitAddress` (per address) and
  `SVC_RateLimit` (global); `connect`, `ipAuthorize` and `disconnect` do not.
  rcon also counts wrong passwords per address (`sv_rconMaxAttempts`,
  `sv_rconLockTime`, `sv_rconWhitelist`). A new connectionless command that
  answers gets the same rate limits.
- Text from players never becomes a format string; `-Wformat` and the
  printf attributes catch it at build time.
- The `ip` userinfo key (real address behind a local proxy) is trusted only
  from `127.0.0.1`.
- Downloads refuse the stock `iw_*.iwd` and server-only `_svr_` files.

## Known gaps

- `sv_pure` only proves a client has the server's iwds: clients report just
  the iwds they have read from, and the pure checksums can be computed from
  the stock files, so it cannot detect extra iwds. Protocol 119 skips the
  check. See [../roadmap/debt.md](../roadmap/debt.md).
- The protocol 119 fragment offset format is not verified against a real
  client.
