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

	// overrideContents
	bool overrideContents;
	int contents;

	bool noEarthquakes;
	bool silent;

	// setCollisionTeam: which players this one collides with
	int collisionTeam;

	// objective_player_*: a slot that is not empty replaces the shared
	// objective of the same index for this player
	objective_t objectives[MAX_OBJECTIVES];
} libcodClient_t;

extern libcodClient_t libcodClients[];

// Per-entity state, cleared when the entity is freed
typedef struct
{
	// the r.clientMask bits set by hideFromPlayer, undone when that player's
	// slot is freed
	int hiddenMask[2];

	// notSolidForPlayer: the players who walk through this brush model
	int notSolidMask[2];

	// playFxOnTagForPlayer: per event slot, the event sequence it was added
	// with and the player (number + 1) it is for
	struct
	{
		int sequence;
		int target;
	} fxOnTag[4];
} libcodEntity_t;

enum
{
	COLLISION_TEAM_AXIS_ALLIES,	// everyone (the default)
	COLLISION_TEAM_NONE,
	COLLISION_TEAM_AXIS,
	COLLISION_TEAM_ALLIES,
};

extern libcodEntity_t libcodEntities[];

// The client whose Pmove is running, or -1: per-player collision only
// applies to a player's own movement
extern int libcodPmoveClient;

void Libcod_ResetClient(int clientNum);
void Libcod_FreeEntity(int entityNum);
bool Libcod_SkipCollision(int clientNum, int otherNum);
bool Libcod_IsNotSolidFor(int entityNum, int clientNum);
bool Libcod_IsSilencedEvent(int entityNum, int event);

#endif
