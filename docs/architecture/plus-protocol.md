# CoD2 Plus protocol

The contract between the `plus` branches of CoD2rev-Plus and OpenCoD2-Plus.
This file is the source of truth; OpenCoD2-Plus follows it. The decision is
[ADR 0011](adr/0011-plus-protocol.md).

| Field | Value |
|---|---|
| Name | CoD2 Plus |
| Plus version | 121 |
| Base | The 1.3 build (`make PROTOCOL=118`) |
| State | Draft: nothing is implemented; milestone 0 is measured |

## Goals

- A newer client gets a better network: fewer bytes first, and later what the
  measurements justify.
- The 1.3 clients keep playing on a Plus server for as long as possible, and
  are always told to update.
- The Plus client keeps joining stock 1.3 servers.

## Non-goals

- A second master server, or a new listing.
- 1.0 clients on a Plus server: they stay on `master`, and the client's 1.0
  core speaks protocol 115 only.
- Refusing 1.3 clients. It waits for a feature that cannot work without it,
  and would be a new version of this document.

## Anti-goals

These would make the work a failure even if it ran:

- A 1.3 client on a Plus server, in a match with Plus clients, at a
  disadvantage it cannot fix by updating.
- A Plus client that cannot join a stock server.
- A server that trusts anything because a client claims to be Plus.

## Numbers

Stock uses 115 (1.0) and 118 (1.3), Game Pass 119, CoD2x 120. CoD2x clients
drop info replies whose `protocol` is not 118 or 120, so a server announcing
121 would vanish from their browser. The number is therefore never announced:
`protocol` stays 118, and 121 only appears in the handshake.

## Handshake

The server still announces protocol 118 and version 1.3, so it stays in every
list, and no client needs to look at a server before connecting.

1. The Plus client always sends a `plus` key in the `connect` userinfo, with
   its Plus version, next to the usual `protocol\118`. Stock servers do not
   read it. The key is set for that packet only, not as a user variable.
2. The server compares it with its own Plus version.
   - Equal: it stores it in the client slot and answers `connectResponse 121`.
   - Anything else, or no key: the client is a 1.3 client, and the server
     answers a bare `connectResponse`.
3. The client uses the Plus format only if the answer's argument equals its
   own version. A bare answer, from a stock server or from a Plus server of
   another version, means 1.3.

There is no error path: a mismatch in either direction falls back to 1.3, so
both sides can be released at their own pace. Every incompatible change raises
the Plus version.

A client that lies gets a format it cannot read, and hurts nobody else. The
server's Plus decoder treats everything a client sends as hostile, like the
rest of the network code, and the value only ever selects a format.

## Update notice

A client that ends up on 1.3 on a Plus server gets one line the first time it
enters the game on each connection, not again on a `map_restart`, as a
`SV_CMD_RELIABLE` game message (command 101, as the kick notices in
`src/server/sv_client_mp.cpp`). That covers 1.3 clients and Plus clients of
another version, so the default text asks for the current client rather than
a newer one, and links to the OpenCoD2-Plus releases. Stock clients can show
it, so it needs nothing from them.

The notice is always on. `sv_plusNotice` holds the text, for admins who need
another language or link, and an empty value restores the default.

## Fairness

While a match can hold both kinds of client, a Plus client gets the same
information as a 1.3 client, in fewer bytes, under the same caps (`sv_fps`,
`snaps`, `rate`).

One difference is accepted: the server spaces snapshots by the bytes of the
last one (`SV_RateMsec`), so a Plus client can get more snapshots than a 1.3
client on the same `rate`, up to those caps. With `rate` 5000, 20 snapshots a
second only fit messages of about 200 bytes. Updating fixes it, and an admin
who wants them level raises every client with `g_forceRate`: at 25000 the limit
is about 1,200 bytes.

Anything else that would give it more (extra entities, snapshots beyond the
caps, different movement) waits for a server option that is off by default and
refuses 1.3 clients while on, which needs a new version of this document.

## Where the format changes

- Server to client: `SV_SendMessageToClient` in
  `src/server/sv_snapshot_mp.cpp` compresses with `MSG_WriteBitsCompress` in
  one place, per client.
- Client to server: `SV_ExecuteClientMessage` in
  `src/server/sv_client_mp.cpp` decompresses with `MSG_ReadBitsCompress`.
- The per-client switch exists only in the 1.0 build
  (`PROTOCOL_VERSION == 115` with `LIBCOD`). The Plus branch of the 1.3 build
  gets its own, on the stored Plus version in the client slot.
- New format code goes in new files under `src/qcommon`, with no engine types
  in their interface, so that OpenCoD2-Plus can vendor and compile them
  unchanged. Today each side has its own message code (`src/qcommon/msg_mp.cpp`
  here, the engine's own `msg_mp.c` there); this avoids writing a format twice.
- A compression library counts as a vendored library: it is asked for first,
  under `src/`, and carries no runtime dependency.
- Each message decodes on its own, with no state between packets: packets get
  lost, and a demo can start at any moment.
- Demos stay stock: a Plus client that records writes Huffman again, using
  its decoded message, so that any client can play them. This holds while
  only the compression stage changes; a change to the message's content has to
  revisit it.

## Milestones

| # | Work | Done when |
|---|---|---|
| 0 | Measure: server-to-client bytes per client in a match with real fights, split by message type, before and after Huffman; candidate compressors run offline on the same payloads; what a recoding of the entity and playerstate deltas could save, as an estimate | A table says what each candidate saves |
| 1 | Handshake and notice, no format change | The checks below pass |
| 2 | The chosen format | Same checks, with fewer bytes measured against milestone 0 |

Milestone 1 starts only if milestone 0 finds a candidate that takes at least
20% off the server-to-client bytes in fights, at no more than one extra point
of a core with 64 clients. Below that the maintainer decides between a delta
recoding, which needs engine types and a new version of this document, and
stopping: a handshake with no format behind it only adds an update notice.

Stability work that is not about the protocol goes to `master` when it is a
fix for stock clients too.

Milestone 1 is not done until a real 1.3 client, a real CoD2x client and the
Plus client have joined a Plus server, the first two with the notice shown and
all three in one match, and the Plus client has joined a stock 1.3 server.
Unit checks with `scripts/smoke_test.py`'s test client come first, and CI
covers what it can without game files.

## Milestone 0 results

Measured on 2026-10-31 with the 1.3 build: 20 protocol clients and 20 bots in
tdm, on `mp_toujane` and `mp_carentan`, about 90 s each with 65 to 113 kills.
Bytes are the server's output per message, without 28 bytes of IP and UDP. The
test clients only walk and all ran over loopback, so `rate` did not space the
snapshots.

Entities are 86–90% of a message and the playerstate 8–12%. Huffman takes off
only 4–7% of the raw bytes, because the data is already packed in bits.

The biggest saving needs no new format. `MSG_WriteDeltaField` in
`src/qcommon/msg_mp.cpp` compares the field pointers, not their values, so an
unchanged field is sent in full; 47–49% of the fields written are unchanged.
The engine's reconstruction in OpenCoD2-Plus compares values, and the decoder
already handles it. Written both ways on the same snapshots:

| Map | Messages | Today | Comparing values | Largest 10% of messages |
|---|---|---|---|---|
| `mp_toujane` | 41,049 | 499 bytes | 396 (−20.5%) | 978 to 772 |
| `mp_carentan` | 42,658 | 386 bytes | 309 (−19.8%) | 853 to 679 |

What a new format would add on top, against Huffman with values compared.
Tables were trained on one map and tested on the other; the time is one
compression, on one core:

| Candidate | Saves | Time | Table |
|---|---|---|---|
| New order-0 Huffman table | 4.7–5.4% | none extra | 256 entries |
| zlib 6 | 6.1–7.8% | 18 µs | none |
| zlib 6 with a raw dictionary | 7.2–8.7% | 56 µs | 32 KB |
| lz4 | −1.5 to −2.8% (larger) | 0.7 µs | none |
| zstd 3 with a trained dictionary | 8.1–9.8% | 3 µs | 16 KB |
| zstd 9 with a trained dictionary | 12.5–14.4% | 9.6 µs | 16 KB |
| Order-1 entropy (a limit, not a codec) | 11.3–11.9% | — | 64 KB |

A recoding of the entity deltas (XOR for floats, variable length for integers)
is estimated from a bit-cost model of the same field stream, without Huffman:
26–30% below today's bytes, about 12–14% below comparing values.

No candidate reaches the 20% of the gate above once values are compared, and
none was tried together. At 64 clients and 20 snapshots a second (1,280
messages), zstd 9 costs 1.2% of a core, above the gate's one point.

## Two repositories

| Step | Where |
|---|---|
| A fix starts on `master` and is merged to `plus` | CoD2rev-Plus |
| A protocol change starts on `plus`, with this file | CoD2rev-Plus |
| It is vendored into OpenCoD2-Plus `plus`, and `test/vendored_files.txt` records the revision | OpenCoD2-Plus |
| The client follows, and is tested against that server | OpenCoD2-Plus |
| A release is the same tag in both, the server first | Both |

- Code flows one way: from here to OpenCoD2-Plus, never back. Its `main`
  vendors `master` and its `plus` vendors `plus`.
- A fix found while working on the client is made here first
  ([its vendoring guide](https://github.com/JotaJazz/OpenCoD2-Plus/blob/main/docs/development/vendored.md)).
- `plus` tags are `plus-vX.Y.Z`, separate from the `master` releases.
- Merges go from `master` to `plus`, never the reverse. The 1.0 code stays on
  `plus`, built as today, so those merges stay clean.

## Before a public release

The Plus client cannot be required until OpenCoD2-Plus can be distributed: it
has no license yet, and its docs exclude distributing binaries. A release of
this branch has to wait for that, because the update notice would point at
something players cannot get.
