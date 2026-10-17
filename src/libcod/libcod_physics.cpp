#include "gsc.hpp"

#ifdef LIBCOD

// Gravity for script models (enableGravity), on the trajectories the client
// already knows: TR_GRAVITY while falling, TR_LINEAR at the maximum speed and
// TR_STATIONARY at rest. The trajectory is only rewritten when the motion
// changes (a hit, a bounce, the speed limit), so the client draws the same
// path the server follows.

#define GRAVITY_MASK ((MASK_SHOT & ~CONTENTS_WATER) | CONTENTS_PLAYERCLIP)

// Surfaces flatter than this are ground; steeper ones are walls
#define GROUND_NORMAL 0.7f

// A bounce off the ground weaker than what gravity adds in this many frames
// would bounce again every frame: the entity comes to rest instead
#define BOUNCE_REST_FRAMES 1.5f

static void Libcod_StartMotion(gentity_t *ent, const vec3_t velocity)
{
	libcodEntity_t *le = &libcodEntities[ent->s.number];

	ent->s.pos.trType = TR_GRAVITY;
	ent->s.pos.trTime = level.time;
	ent->s.pos.trDuration = 0;
	VectorCopy(ent->r.currentOrigin, ent->s.pos.trBase);
	VectorCopy(velocity, ent->s.pos.trDelta);
	ent->s.groundEntityNum = ENTITYNUM_NONE;
	le->resting = false;

	// at the speed limit already: no acceleration
	if (VectorLength(velocity) > le->maxVelocity)
	{
		ent->s.pos.trType = TR_LINEAR;
		VectorScale(velocity, le->maxVelocity / VectorLength(velocity), ent->s.pos.trDelta);
	}
}

// The velocity now: what the trajectory gives, not the last frame's move
void Libcod_GetEntityVelocity(gentity_t *ent, vec3_t velocity)
{
	if (libcodEntities[ent->s.number].resting || ent->s.pos.trType == TR_STATIONARY)
		VectorClear(velocity);
	else
		BG_EvaluateTrajectoryDelta(&ent->s.pos, level.time, velocity);
}

void Libcod_SetEntityVelocity(gentity_t *ent, const vec3_t velocity)
{
	Libcod_StartMotion(ent, velocity);
}

void Libcod_EnableGravity(gentity_t *ent, bool bounce, bool locational, bool alignToGround)
{
	libcodEntity_t *le = &libcodEntities[ent->s.number];
	vec3_t velocity;

	if (!le->gravity)
	{
		le->maxVelocity = 8192;
		le->parallelBounce = 0.5f;
		le->perpendicularBounce = 0.25f;
		VectorClear(velocity);
	}
	else
	{
		Libcod_GetEntityVelocity(ent, velocity);
	}

	le->gravity = bounce ? LIBCOD_GRAVITY_BOUNCE : LIBCOD_GRAVITY_FALL;
	le->locational = locational;
	le->alignToGround = alignToGround;
	ent->clipmask = GRAVITY_MASK;
	G_SetAngle(ent, ent->r.currentAngles);
	Libcod_StartMotion(ent, velocity);
}

// Script motion (moveTo, rotateTo, linkTo...) takes over from gravity
void Libcod_DisableGravity(gentity_t *ent)
{
	libcodEntity_t *le = &libcodEntities[ent->s.number];

	if (!le->gravity)
		return;

	le->gravity = LIBCOD_GRAVITY_NONE;
	le->resting = false;
	G_SetOrigin(ent, ent->r.currentOrigin);
}

// The origin was set by script: fall again from there
void Libcod_EntityOriginSet(gentity_t *ent)
{
	vec3_t velocity;

	if (!libcodEntities[ent->s.number].gravity)
		return;

	VectorClear(velocity);
	Libcod_StartMotion(ent, velocity);
}

static void Libcod_NotifyGround(gentity_t *ent, unsigned short name, const trace_t *trace)
{
	if (!Scr_IsSystemActive())
		return;

	if (trace->entityNum < ENTITYNUM_WORLD)
		Scr_AddEntity(&g_entities[trace->entityNum]);
	else
		Scr_AddUndefined();
	Scr_AddString(Com_SurfaceTypeToName(SURF_TYPEINDEX(trace->surfaceFlags)));
	Scr_Notify(ent, name, 2);
}

static void Libcod_Land(gentity_t *ent, const trace_t *trace, const vec3_t endpos)
{
	libcodEntity_t *le = &libcodEntities[ent->s.number];

	G_SetOrigin(ent, endpos);
	ent->s.groundEntityNum = trace->entityNum;
	le->resting = true;

	if (le->alignToGround)
	{
		// keep the yaw, tilt the model onto the surface
		vec3_t axis[3], angles, yaw = { 0, ent->r.currentAngles[1], 0 };

		VectorCopy(trace->normal, axis[2]);
		AngleVectors(yaw, axis[0], NULL, NULL);
		VectorMA(axis[0], -DotProduct(axis[0], axis[2]), axis[2], axis[0]);
		Vec3Normalize(axis[0]);
		Vec3Cross(axis[2], axis[0], axis[1]);
		AxisToAngles(axis, angles);
		G_SetAngle(ent, angles);
	}

	Libcod_NotifyGround(ent, scr_const.land, trace);
}

// Called for every entity each frame; true when the entity is under
// script gravity and was moved here
bool Libcod_RunEntityGravity(gentity_t *ent)
{
	libcodEntity_t *le = &libcodEntities[ent->s.number];
	trace_t trace;
	vec3_t target, velocity;

	if (!le->gravity)
		return false;

	if (le->resting)
	{
		// still standing on something?
		VectorCopy(ent->r.currentOrigin, target);
		target[2] -= 1.5f;
		SV_Trace(&trace, ent->r.currentOrigin, ent->r.mins, ent->r.maxs, target, ent->s.number, ent->clipmask, le->locational, NULL, 1);

		if (trace.fraction == 1.0f && !trace.startsolid)
		{
			VectorClear(velocity);
			Libcod_StartMotion(ent, velocity);
		}

		G_RunThink(ent);
		return true;
	}

	BG_EvaluateTrajectory(&ent->s.pos, level.time, target);
	SV_Trace(&trace, ent->r.currentOrigin, ent->r.mins, ent->r.maxs, target, ent->s.number, ent->clipmask, le->locational, NULL, 1);

	if (trace.startsolid)
	{
		// stuck in something: stop where it is
		trace.normal[0] = trace.normal[1] = 0;
		trace.normal[2] = 1;
		Libcod_Land(ent, &trace, ent->r.currentOrigin);
		SV_LinkEntity(ent);
		G_RunThink(ent);
		return true;
	}

	vec3_t endpos;
	Vec3Lerp(ent->r.currentOrigin, target, trace.fraction, endpos);
	VectorCopy(endpos, ent->r.currentOrigin);

	if (trace.fraction < 1.0f)
	{
		float into;

		BG_EvaluateTrajectoryDelta(&ent->s.pos, level.time, velocity);
		into = DotProduct(velocity, trace.normal);

		if (le->gravity == LIBCOD_GRAVITY_BOUNCE)
		{
			vec3_t along, across;

			// the part across the surface is reflected and damped, the part
			// along it only damped
			VectorScale(trace.normal, into, across);
			VectorSubtract(velocity, across, along);
			VectorScale(along, le->parallelBounce, velocity);
			VectorMA(velocity, -le->perpendicularBounce, across, velocity);

			if (trace.normal[2] > GROUND_NORMAL
			        && DotProduct(velocity, trace.normal) < DEFAULT_GRAVITY * level.frameTime * 0.001f * BOUNCE_REST_FRAMES)
			{
				Libcod_Land(ent, &trace, endpos);
			}
			else
			{
				Libcod_StartMotion(ent, velocity);
				Libcod_NotifyGround(ent, scr_const.bounce, &trace);
			}
		}
		else if (trace.normal[2] > GROUND_NORMAL)
		{
			Libcod_Land(ent, &trace, endpos);
		}
		else
		{
			// a wall or a ceiling: slide along it
			VectorMA(velocity, -into, trace.normal, velocity);
			Libcod_StartMotion(ent, velocity);
		}
	}
	else if (ent->s.pos.trType == TR_GRAVITY)
	{
		// the speed limit: from here on at a constant speed
		BG_EvaluateTrajectoryDelta(&ent->s.pos, level.time, velocity);
		if (VectorLength(velocity) > le->maxVelocity)
			Libcod_StartMotion(ent, velocity);
	}

	SV_LinkEntity(ent);
	G_RunThink(ent);
	return true;
}

#endif
