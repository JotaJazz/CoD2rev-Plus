#ifndef _LIBCOD_CLIENT_H_
#define _LIBCOD_CLIENT_H_

// Per-client state set by libcod script functions and read by the engine.
// It is cleared when a client slot is freed, so the next player in that slot
// starts with the defaults.
typedef struct
{
	bool hiddenFromScoreboard;
	bool hiddenFromServerStatus;
	bool notAllowingSpectators;
} libcodClient_t;

extern libcodClient_t libcodClients[];

void Libcod_ResetClient(int clientNum);

#endif
