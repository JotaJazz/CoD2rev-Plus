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

	// weapons; the scales start at 1
	float weaponSpreadScale;
	float turretSpreadScale;
	float meleeRangeScale;
	float meleeWidthScale;
	float meleeHeightScale;
	float fireRangeScale;
	bool overrideBulletMask;
	int bulletMask;
	bool fireThroughWalls;
	bool noBulletImpacts;
} libcodClient_t;

extern libcodClient_t libcodClients[];

void Libcod_ResetClient(int clientNum);

#endif
