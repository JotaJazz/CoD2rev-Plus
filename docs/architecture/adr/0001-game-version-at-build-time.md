# 0001. Game version chosen at build time

- Status: Accepted
- Date: 2026-10-13

## Context

CoD2 1.0 (protocol 115) and 1.3 (protocol 118) differ in network formats,
message sizes and some game code. Upstream built only 1.0. Master servers
list a server only for clients of the version it announces, and CoD2x 1.4
clients list only 118 or 120.

## Decision

`PROTOCOL=115|118` selects the version at build time (`PROTOCOL_VERSION`).
One binary is one announced version. The 1.0 build with libcod also accepts
protocols 115–119 by direct connection, switching formats per client.

## Consequences

- Every change under `#if PROTOCOL_VERSION` is built and tested for both.
- Objects are kept per platform and version (`obj/<binary>_<protocol>`), but
  both versions write the same name in `bin/`; `BIN_DIR` separates them.
- One port announces one version; `net_altPort` gives the 1.0 build a second
  port for the 1.3 lists ([../networking.md](../networking.md)).
