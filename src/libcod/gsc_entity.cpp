#include "gsc_entity.hpp"

#if LIBCOD_COMPILE_ENTITY == 1

void gsc_entity_setalive(scr_entref_t id)
{
	int isAlive;

	if ( ! stackGetParams("i", &isAlive))
	{
		stackError("gsc_entity_setalive() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	gentity_t *entity = &g_entities[id.entnum];

	entity->takedamage = isAlive;
	stackPushBool(qtrue);
}

void gsc_entity_setbounds(scr_entref_t id)
{
	float width, height;

	if ( ! stackGetParams("ff", &width, &height))
	{
		stackError("gsc_entity_setbounds() one or more arguments is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	gentity_t *entity = &g_entities[id.entnum];

	vec3_t mins = {-height, -width, -width};
	vec3_t maxs = {width, width, height};

	VectorCopy(mins, entity->r.mins);
	VectorCopy(maxs, entity->r.maxs);

	stackPushBool(qtrue);
}

// Methods below are named and behave as in zk_libcod

// spawnGrenade(weapon, origin[, direction[, velocity[, fuse seconds]]]): a
// grenade thrown by this entity
void gsc_entity_spawngrenade(scr_entref_t id)
{
	int args = Scr_GetNumParam();
	const char *name;
	vec3_t origin;
	vec3_t dir = { 0, 0, 0 };
	vec3_t velocity = { 0, 0, 0 };

	if (args < 2 || args > 5 || !stackGetParamString(0, &name) || !stackGetParamVector(1, origin)
	        || (args > 2 && !stackGetParamVector(2, dir)) || (args > 3 && !stackGetParamVector(3, velocity)))
	{
		stackError("gsc_entity_spawngrenade() one or more arguments is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	int weaponIndex = BG_FindWeaponIndexForName(name);
	if (!weaponIndex || BG_GetWeaponDef(weaponIndex)->weaponType != WEAPTYPE_GRENADE)
	{
		stackError("gsc_entity_spawngrenade() %s is not a loaded grenade", name);
		stackPushUndefined();
		return;
	}

	int fuseTime = BG_GetWeaponDef(weaponIndex)->fuseTime;
	if (args > 4)
	{
		float seconds;
		if (!stackGetParamFloat(4, &seconds))
		{
			stackError("gsc_entity_spawngrenade() fuse time has a wrong type");
			stackPushUndefined();
			return;
		}
		fuseTime = (int)(seconds * 1000);
	}

	gentity_t *owner = &g_entities[id.entnum];

	// fire_grenade takes over a grenade the owner is cooking; this one is
	// not that grenade
	int cooking = owner->client ? owner->client->ps.grenadeTimeLeft : 0;
	if (owner->client)
		owner->client->ps.grenadeTimeLeft = 0;

	gentity_t *grenade = fire_grenade(owner, origin, dir, weaponIndex, fuseTime);

	if (owner->client)
		owner->client->ps.grenadeTimeLeft = cooking;

	Vec3Normalize(dir);
	VectorMA(grenade->s.pos.trDelta, DotProduct(velocity, dir), dir, grenade->s.pos.trDelta);

	stackPushEntity(grenade);
}

static bool gsc_entity_isgrenade(scr_entref_t id, const char *function)
{
	if (g_entities[id.entnum].handler != ENT_HANDLER_GRENADE)
	{
		stackError("%s() entity %i is not a grenade", function, id.entnum);
		stackPushUndefined();
		return false;
	}
	return true;
}

// Seconds left before a thrown grenade explodes
void gsc_entity_getgrenadefusetime(scr_entref_t id)
{
	if (!gsc_entity_isgrenade(id, "gsc_entity_getgrenadefusetime"))
		return;

	int left = g_entities[id.entnum].nextthink - level.time;
	stackPushFloat(left < 0 ? 0 : left / 1000.0f);
}

// Adds (or with a negative value removes) seconds of fuse
void gsc_entity_addgrenadefusetime(scr_entref_t id)
{
	float seconds;

	if (!stackGetParams("f", &seconds))
	{
		stackError("gsc_entity_addgrenadefusetime() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	if (!gsc_entity_isgrenade(id, "gsc_entity_addgrenadefusetime"))
		return;

	gentity_t *ent = &g_entities[id.entnum];
	int ms = (int)(seconds * 1000);

	ent->nextthink += ms - ms % FRAMETIME;
	if (ent->nextthink < level.time)
		ent->nextthink = level.time;

	stackPushBool(qtrue);
}

void gsc_entity_isturret(scr_entref_t id)
{
	stackPushBool(g_entities[id.entnum].pTurretInfo != NULL);
}

// The player on the turret, or undefined
void gsc_entity_getturretowner(scr_entref_t id)
{
	gentity_t *ent = &g_entities[id.entnum];

	if (!ent->pTurretInfo)
	{
		stackError("gsc_entity_getturretowner() entity %i is not a turret", id.entnum);
		stackPushUndefined();
		return;
	}

	if (ent->r.ownerNum == ENTITYNUM_NONE)
		stackPushUndefined();
	else
		stackPushEntity(&g_entities[ent->r.ownerNum]);
}

static const gitem_t *gsc_entity_getitem(scr_entref_t id, const char *function, int type1, int type2)
{
	gentity_t *ent = &g_entities[id.entnum];

	if (ent->s.eType != ET_ITEM || ent->s.index < 1 || ent->s.index >= bg_numItems
	        || (bg_itemlist[ent->s.index].giType != type1 && bg_itemlist[ent->s.index].giType != type2))
	{
		stackError("%s() entity %i is not an item of the right type", function, id.entnum);
		stackPushUndefined();
		return NULL;
	}

	return &bg_itemlist[ent->s.index];
}

// Ammo of a weapon lying on the ground (-1: none)
void gsc_entity_getweaponitemammo(scr_entref_t id)
{
	if (gsc_entity_getitem(id, "gsc_entity_getweaponitemammo", IT_WEAPON, IT_WEAPON))
		stackPushInt(g_entities[id.entnum].count);
}

void gsc_entity_setweaponitemammo(scr_entref_t id)
{
	int ammo;

	if (!stackGetParams("i", &ammo))
	{
		stackError("gsc_entity_setweaponitemammo() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	if (gsc_entity_getitem(id, "gsc_entity_setweaponitemammo", IT_WEAPON, IT_WEAPON))
	{
		g_entities[id.entnum].count = ammo;
		stackPushBool(qtrue);
	}
}

void gsc_entity_getweaponitemclipammo(scr_entref_t id)
{
	if (gsc_entity_getitem(id, "gsc_entity_getweaponitemclipammo", IT_WEAPON, IT_WEAPON))
		stackPushInt(g_entities[id.entnum].item.clipAmmoCount);
}

void gsc_entity_setweaponitemclipammo(scr_entref_t id)
{
	int ammo;

	if (!stackGetParams("i", &ammo))
	{
		stackError("gsc_entity_setweaponitemclipammo() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	if (gsc_entity_getitem(id, "gsc_entity_setweaponitemclipammo", IT_WEAPON, IT_WEAPON))
	{
		g_entities[id.entnum].item.clipAmmoCount = ammo;
		stackPushBool(qtrue);
	}
}

// How much an ammo or health item gives. This is per item type: it changes
// every item of that type.
void gsc_entity_getitemquantity(scr_entref_t id)
{
	const gitem_t *item = gsc_entity_getitem(id, "gsc_entity_getitemquantity", IT_AMMO, IT_HEALTH);
	if (item)
		stackPushInt(item->quantity);
}

void gsc_entity_setitemquantity(scr_entref_t id)
{
	int quantity;

	if (!stackGetParams("i", &quantity))
	{
		stackError("gsc_entity_setitemquantity() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	const gitem_t *item = gsc_entity_getitem(id, "gsc_entity_setitemquantity", IT_AMMO, IT_HEALTH);
	if (item)
	{
		bg_itemlist[item - bg_itemlist].quantity = quantity;
		stackPushBool(qtrue);
	}
}

#endif
