# 0011. A negotiated Plus protocol on the plus branch

- Status: Superseded by 0012
- Date: 2026-10-31

## Context

`master` is a release candidate for stock clients, and its rule is that
network formats never change. OpenCoD2-Plus now has a client that plays online
and vendors this server, so a better network no longer needs the stock client,
but stock clients and servers are where the players are. CoD2x already holds
the names 1.4 and protocol 120, and its clients hide servers announcing
anything but 118 or 120.

## Decision

The `plus` branch adds CoD2 Plus, version 121, to the 1.3 build. The server
keeps announcing 118, and a client and server agree on Plus in the `connect`
handshake, falling back to 1.3 on any mismatch. 1.3 clients stay welcome and
are always told to update. On `plus`, the rule against changing formats and
needing a modified client applies to stock clients, not to the negotiated
format. [plus-protocol.md](../plus-protocol.md) is the contract.

Rejected:

- Plus without compatibility, on protocol 121 alone: no listing without a new
  master, an empty server until the client spreads, and nothing gained that
  the fallback prevents.
- Announcing 121: it would disappear from CoD2x browsers.
- The name 1.4: taken by CoD2x.
- One repository: the client's engine and history stay apart, and the existing
  one-way vendoring already works.

## Consequences

- The server has two formats to test per change, for as long as 1.3 clients
  are accepted.
- Mixed matches keep a limit: the same information under the same caps. A Plus
  client can get more snapshots on the same `rate`, which is accepted; other
  gains beyond fewer bytes need a mode that refuses 1.3 clients.
- OpenCoD2-Plus needs the same decision, in its own ADR: its docs also exclude
  protocol changes.
- A release that requires the client waits for that project's license.
