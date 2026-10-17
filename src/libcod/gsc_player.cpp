#include "gsc_player.hpp"

#if LIBCOD_COMPILE_PLAYER == 1

void gsc_player_velocity_set(scr_entref_t id)
{
	vec3_t velocity;

	if ( ! stackGetParams("v", &velocity))
	{
		stackError("gsc_player_velocity_set() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	if (id.entnum >= MAX_CLIENTS)
	{
		stackError("gsc_player_velocity_set() entity %i is not a player", id.entnum);
		stackPushUndefined();
		return;
	}

	playerState_t *ps = SV_GameClientNum(id.entnum);
	VectorCopy(velocity, ps->velocity);
	stackPushBool(qtrue);
}

void gsc_player_velocity_add(scr_entref_t id)
{
	vec3_t velocity;

	if ( ! stackGetParams("v", &velocity))
	{
		stackError("gsc_player_velocity_add() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	if (id.entnum >= MAX_CLIENTS)
	{
		stackError("gsc_player_velocity_add() entity %i is not a player", id.entnum);
		stackPushUndefined();
		return;
	}

	playerState_t *ps = SV_GameClientNum(id.entnum);
	VectorAdd(ps->velocity, velocity, ps->velocity);
	stackPushBool(qtrue);
}

void gsc_player_velocity_get(scr_entref_t id)
{
	if (id.entnum >= MAX_CLIENTS)
	{
		stackError("gsc_player_velocity_get() entity %i is not a player", id.entnum);
		stackPushUndefined();
		return;
	}

	playerState_t *ps = SV_GameClientNum(id.entnum);
	stackPushVector(ps->velocity);
}

void gsc_player_clientuserinfochanged(scr_entref_t id)
{
	if (id.entnum >= MAX_CLIENTS)
	{
		stackError("gsc_player_clientuserinfochanged() entity %i is not a player", id.entnum);
		stackPushUndefined();
		return;
	}

	ClientUserinfoChanged(id.entnum);
	stackPushBool(qtrue);
}

void gsc_player_get_userinfo(scr_entref_t id)
{
	const char *key;

	if ( ! stackGetParams("s", &key))
	{
		stackError("gsc_player_get_userinfo() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	if (id.entnum >= MAX_CLIENTS)
	{
		stackError("gsc_player_get_userinfo() entity %i is not a player", id.entnum);
		stackPushUndefined();
		return;
	}

	client_t *client = &svs.clients[id.entnum];
	const char *val = Info_ValueForKey(client->userinfo, key);

	if (strlen(val))
		stackPushString(val);
	else
		stackPushUndefined();
}

void gsc_player_set_userinfo(scr_entref_t id)
{
	const char *key, *value;

	if ( ! stackGetParams("ss", &key, &value))
	{
		stackError("gsc_player_set_userinfo() one or more arguments is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	if (id.entnum >= MAX_CLIENTS)
	{
		stackError("gsc_player_set_userinfo() entity %i is not a player", id.entnum);
		stackPushUndefined();
		return;
	}

	client_t *client = &svs.clients[id.entnum];
	Info_SetValueForKey(client->userinfo, key, value);
	stackPushBool(qtrue);
}

void gsc_player_button_ads(scr_entref_t id)
{
	if (id.entnum >= MAX_CLIENTS)
	{
		stackError("gsc_player_button_ads() entity %i is not a player", id.entnum);
		stackPushUndefined();
		return;
	}

	client_t *client = &svs.clients[id.entnum];
	stackPushBool(client->lastUsercmd.buttons & BUTTON_ADS ? qtrue : qfalse);
}

void gsc_player_button_left(scr_entref_t id)
{
	if (id.entnum >= MAX_CLIENTS)
	{
		stackError("gsc_player_button_left() entity %i is not a player", id.entnum);
		stackPushUndefined();
		return;
	}

	client_t *client = &svs.clients[id.entnum];
	stackPushBool(client->lastUsercmd.rightmove == BUTTON_MOVELEFT ? qtrue : qfalse);
}

void gsc_player_button_right(scr_entref_t id)
{
	if (id.entnum >= MAX_CLIENTS)
	{
		stackError("gsc_player_button_right() entity %i is not a player", id.entnum);
		stackPushUndefined();
		return;
	}

	client_t *client = &svs.clients[id.entnum];
	stackPushBool(client->lastUsercmd.rightmove == BUTTON_MOVERIGHT ? qtrue : qfalse);
}

void gsc_player_button_forward(scr_entref_t id)
{
	if (id.entnum >= MAX_CLIENTS)
	{
		stackError("gsc_player_button_forward() entity %i is not a player", id.entnum);
		stackPushUndefined();
		return;
	}

	client_t *client = &svs.clients[id.entnum];
	stackPushBool(client->lastUsercmd.forwardmove == BUTTON_FORWARD ? qtrue : qfalse);
}

void gsc_player_button_back(scr_entref_t id)
{
	if (id.entnum >= MAX_CLIENTS)
	{
		stackError("gsc_player_button_back() entity %i is not a player", id.entnum);
		stackPushUndefined();
		return;
	}

	client_t *client = &svs.clients[id.entnum];
	stackPushBool(client->lastUsercmd.forwardmove == BUTTON_BACK ? qtrue : qfalse);
}

void gsc_player_button_leanleft(scr_entref_t id)
{
	if (id.entnum >= MAX_CLIENTS)
	{
		stackError("gsc_player_button_leanleft() entity %i is not a player", id.entnum);
		stackPushUndefined();
		return;
	}

	client_t *client = &svs.clients[id.entnum];
	stackPushBool(client->lastUsercmd.buttons & BUTTON_LEANLEFT ? qtrue : qfalse);
}

void gsc_player_button_leanright(scr_entref_t id)
{
	if (id.entnum >= MAX_CLIENTS)
	{
		stackError("gsc_player_button_leanright() entity %i is not a player", id.entnum);
		stackPushUndefined();
		return;
	}

	client_t *client = &svs.clients[id.entnum];
	stackPushBool(client->lastUsercmd.buttons & BUTTON_LEANRIGHT ? qtrue : qfalse);
}

void gsc_player_button_reload(scr_entref_t id)
{
	if (id.entnum >= MAX_CLIENTS)
	{
		stackError("gsc_player_button_reload() entity %i is not a player", id.entnum);
		stackPushUndefined();
		return;
	}

	client_t *client = &svs.clients[id.entnum];
	stackPushBool(client->lastUsercmd.buttons & BUTTON_RELOAD ? qtrue : qfalse);
}

void gsc_player_button_jump(scr_entref_t id)
{
	if (id.entnum >= MAX_CLIENTS)
	{
		stackError("gsc_player_button_jump() entity %i is not a player", id.entnum);
		stackPushUndefined();
		return;
	}

	client_t *client = &svs.clients[id.entnum];
	stackPushBool(client->lastUsercmd.buttons & BUTTON_JUMP ? qtrue : qfalse);
}

void gsc_player_button_frag(scr_entref_t id)
{
	if (id.entnum >= MAX_CLIENTS)
	{
		stackError("gsc_player_button_frag() entity %i is not a player", id.entnum);
		stackPushUndefined();
		return;
	}

	client_t *client = &svs.clients[id.entnum];
	stackPushBool(client->lastUsercmd.buttons & BUTTON_FRAG ? qtrue : qfalse);
}

void gsc_player_button_smoke(scr_entref_t id)
{
	if (id.entnum >= MAX_CLIENTS)
	{
		stackError("gsc_player_button_smoke() entity %i is not a player", id.entnum);
		stackPushUndefined();
		return;
	}

	client_t *client = &svs.clients[id.entnum];
	stackPushBool(client->lastUsercmd.buttons & BUTTON_SMOKE ? qtrue : qfalse);
}

void gsc_player_stance_get(scr_entref_t id)
{
	if (id.entnum >= MAX_CLIENTS)
	{
		stackError("gsc_player_stance_get() entity %i is not a player", id.entnum);
		stackPushUndefined();
		return;
	}

	playerState_t *ps = SV_GameClientNum(id.entnum);

	if (ps->pm_flags & PMF_DUCKED)
		stackPushString("duck");
	else if (ps->pm_flags & PMF_PRONE)
		stackPushString("lie");
	else
		stackPushString("stand");
}

void gsc_player_stance_set(scr_entref_t id)
{
	const char *stance;

	if ( ! stackGetParams("s", &stance))
	{
		stackError("gsc_player_stance_set() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	gentity_t *entity = &g_entities[id.entnum];

	if (entity->client == NULL)
	{
		stackError("gsc_player_stance_set() entity %i is not a player", id.entnum);
		stackPushUndefined();
		return;
	}

	int event;

	if (strcmp(stance, "stand") == 0)
		event = EV_STANCE_FORCE_STAND;
	else if (strcmp(stance, "crouch") == 0)
		event = EV_STANCE_FORCE_CROUCH;
	else if (strcmp(stance, "prone") == 0)
		event = EV_STANCE_FORCE_PRONE;
	else
	{
		stackError("gsc_player_stance_set() invalid argument '%s'. Valid arguments are: 'stand' 'crouch' 'prone'", stance);
		stackPushUndefined();
		return;
	}

	G_AddPredictableEvent(entity, event, 0);
	stackPushBool(qtrue);
}

void gsc_player_spectatorclient_get(scr_entref_t id)
{
	gentity_t *entity = &g_entities[id.entnum];

	if (entity->client == NULL)
	{
		stackError("gsc_player_spectatorclient_get() entity %i is not a player", id.entnum);
		stackPushUndefined();
		return;
	}

	if (entity->client->spectatorClient == -1)
		stackPushUndefined();
	else
		stackPushEntity(&g_entities[entity->client->spectatorClient]);
}

char proxy_realip[MAX_CLIENTS][16] = {0};

// The "ip" userinfo key is written by a proxy on the same machine. It is only
// trusted from clients that connect through 127.0.0.1 and must be an IPv4
// address, optionally followed by a port. The slot is cleared on every connect
// so a player never inherits the address of the previous one.
void proxy_setRealIP(int clientNum, const char *ip)
{
	if (clientNum < 0 || clientNum >= MAX_CLIENTS)
		return;

	proxy_realip[clientNum][0] = 0;

	const byte *adr = svs.clients[clientNum].netchan.remoteAddress.ip;
	if (adr[0] != 127 || adr[1] != 0 || adr[2] != 0 || adr[3] != 1)
		return;

	unsigned int a, b, c, d;
	char end;
	int n = sscanf(ip, "%u.%u.%u.%u%c", &a, &b, &c, &d, &end);
	if ((n != 4 && !(n == 5 && end == ':')) || a > 255 || b > 255 || c > 255 || d > 255)
		return;

	snprintf(proxy_realip[clientNum], sizeof(proxy_realip[clientNum]), "%u.%u.%u.%u", a, b, c, d);
	Com_DPrintf("realip = %s\n", proxy_realip[clientNum]);
}
void gsc_player_getip(scr_entref_t id)
{
	if (id.entnum >= MAX_CLIENTS)
	{
		stackError("gsc_player_getip() entity %i is not a player", id.entnum);
		stackPushUndefined();
		return;
	}

	client_t *client = &svs.clients[id.entnum];

	char tmp[16];
	snprintf(tmp, sizeof(tmp), "%d.%d.%d.%d", client->netchan.remoteAddress.ip[0], client->netchan.remoteAddress.ip[1], client->netchan.remoteAddress.ip[2], client->netchan.remoteAddress.ip[3]);

	char localip[16] = "127.0.0.1";

	if (strcmp(localip, tmp) == 0)
	{
		snprintf(tmp, sizeof(tmp), "%s", proxy_realip[id.entnum]);
	}

	stackPushString(tmp);
}

void gsc_player_getping(scr_entref_t id)
{
	if (id.entnum >= MAX_CLIENTS)
	{
		stackError("gsc_player_getping() entity %i is not a player", id.entnum);
		stackPushUndefined();
		return;
	}

	client_t *client = &svs.clients[id.entnum];
	stackPushInt(client->ping);
}

void gsc_player_clientcommand(scr_entref_t id)
{
	if (id.entnum >= MAX_CLIENTS)
	{
		stackError("gsc_player_clientcommand() entity %i is not a player", id.entnum);
		stackPushUndefined();
		return;
	}

	// the command arguments are only those of this client's command while
	// CodeCallback_PlayerCommand runs for it
	extern int playerCommandClient;
	if (id.entnum != playerCommandClient)
	{
		stackError("gsc_player_clientcommand() must be called on the player from CodeCallback_PlayerCommand, without delay, and at most once per player command");
		stackPushUndefined();
		return;
	}
	playerCommandClient = -1;

	ClientCommand(id.entnum);
	stackPushBool(qtrue);
}

void gsc_player_getlastconnecttime(scr_entref_t id)
{
	if (id.entnum >= MAX_CLIENTS)
	{
		stackError("gsc_player_getlastconnecttime() entity %i is not a player", id.entnum);
		stackPushUndefined();
		return;
	}

	client_t *client = &svs.clients[id.entnum];
	stackPushInt(client->lastConnectTime);
}

void gsc_player_getlastmsg(scr_entref_t id)
{
	if (id.entnum >= MAX_CLIENTS)
	{
		stackError("gsc_player_getlastmsg() entity %i is not a player", id.entnum);
		stackPushUndefined();
		return;
	}

	client_t *client = &svs.clients[id.entnum];
	stackPushInt(svs.time - client->lastPacketTime);
}

void gsc_player_getclientstate(scr_entref_t id)
{
	if (id.entnum >= MAX_CLIENTS)
	{
		stackError("gsc_player_getclientstate() entity %i is not a player", id.entnum);
		stackPushUndefined();
		return;
	}

	client_t *client = &svs.clients[id.entnum];
	stackPushInt(client->state);
}

void gsc_player_addresstype(scr_entref_t id)
{
	if (id.entnum >= MAX_CLIENTS)
	{
		stackError("gsc_player_addresstype() entity %i is not a player", id.entnum);
		stackPushUndefined();
		return;
	}

	client_t *client = &svs.clients[id.entnum];
	stackPushInt(client->netchan.remoteAddress.type);
}

void gsc_player_renameclient(scr_entref_t id)
{
	const char *name;

	if ( ! stackGetParams("s", &name))
	{
		stackError("gsc_player_renameclient() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	if (strlen(name) >= MAX_NAME_LENGTH)
	{
		stackError("gsc_player_renameclient() player name is longer than %i characters", MAX_NAME_LENGTH - 1);
		stackPushUndefined();
		return;
	}

	if (id.entnum >= MAX_CLIENTS)
	{
		stackError("gsc_player_renameclient() entity %i is not a player", id.entnum);
		stackPushUndefined();
		return;
	}

	client_t *client = &svs.clients[id.entnum];

	Info_SetValueForKey(client->userinfo, "name", name);
	I_strncpyz(client->name, name, sizeof(client->name));

	stackPushBool(qtrue);
}

void gsc_player_outofbandprint(scr_entref_t id)
{
	const char *cmd;

	if ( ! stackGetParams("s", &cmd))
	{
		stackError("gsc_player_outofbandprint() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	if (id.entnum >= MAX_CLIENTS)
	{
		stackError("gsc_player_outofbandprint() entity %i is not a player", id.entnum);
		stackPushUndefined();
		return;
	}

	client_t *client = &svs.clients[id.entnum];
	NET_OutOfBandPrint(NS_SERVER, client->netchan.remoteAddress, cmd);
	stackPushBool(qtrue);
}

void gsc_player_connectionlesspacket(scr_entref_t id)
{
	const char *cmd;

	if ( ! stackGetParams("s", &cmd))
	{
		stackError("gsc_player_connectionlesspacket() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	if (id.entnum >= MAX_CLIENTS)
	{
		stackError("gsc_player_connectionlesspacket() entity %i is not a player", id.entnum);
		stackPushUndefined();
		return;
	}

	client_t *client = &svs.clients[id.entnum];

	byte bufData[131072];
	msg_t msg;

	MSG_Init(&msg, bufData, sizeof(bufData));

	MSG_WriteByte(&msg, svc_nop);
	MSG_WriteShort(&msg, 0);
	MSG_WriteLong(&msg, -1);
	MSG_WriteString(&msg, cmd);

	SV_ConnectionlessPacket(client->netchan.remoteAddress, &msg);
	stackPushBool(qtrue);
}

void gsc_player_resetnextreliabletime(scr_entref_t id)
{
	if (id.entnum >= MAX_CLIENTS)
	{
		stackError("gsc_player_resetnextreliabletime() entity %i is not a player", id.entnum);
		stackPushUndefined();
		return;
	}

	client_t *client = &svs.clients[id.entnum];
	client->nextReliableTime = 0;
	stackPushBool(qtrue);
}

void gsc_player_ismantling(scr_entref_t id)
{
	if (id.entnum >= MAX_CLIENTS)
	{
		stackError("gsc_player_ismantling() entity %i is not a player", id.entnum);
		stackPushUndefined();
		return;
	}

	playerState_t *ps = SV_GameClientNum(id.entnum);
	stackPushBool(ps->pm_flags & PMF_MANTLE ? qtrue : qfalse);
}

void gsc_player_isonladder(scr_entref_t id)
{
	if (id.entnum >= MAX_CLIENTS)
	{
		stackError("gsc_player_isonladder() entity %i is not a player", id.entnum);
		stackPushUndefined();
		return;
	}

	playerState_t *ps = SV_GameClientNum(id.entnum);
	stackPushBool(ps->pm_flags & PMF_LADDER ? qtrue : qfalse);
}

void gsc_player_isusingturret(scr_entref_t id)
{
	gentity_t *entity = &g_entities[id.entnum];

	if (entity->client == NULL)
	{
		stackError("gsc_player_isusingturret() entity %i is not a player", id.entnum);
		stackPushUndefined();
		return;
	}

	stackPushBool(entity->s.eFlags & EF_TURRET_ACTIVE ? qtrue : qfalse);
}

void gsc_player_getjumpslowdowntimer(scr_entref_t id)
{
	if (id.entnum >= MAX_CLIENTS)
	{
		stackError("gsc_player_getjumpslowdowntimer() entity %i is not a player", id.entnum);
		stackPushUndefined();
		return;
	}

	playerState_t *ps = SV_GameClientNum(id.entnum);
	stackPushInt(ps->pm_time);
}

void gsc_player_clearjumpstate(scr_entref_t id)
{
	if (id.entnum >= MAX_CLIENTS)
	{
		stackError("gsc_player_clearjumpstate() entity %i is not a player", id.entnum);
		stackPushUndefined();
		return;
	}

	playerState_t *ps = SV_GameClientNum(id.entnum);

	ps->pm_flags &= ~( PMF_TIME_LAND | PMF_TIME_SLIDE );
	ps->pm_time = 0;
	ps->jumpTime = 0;
	ps->jumpOriginZ = 0;
}

int player_g_speed[MAX_CLIENTS] = {0};
void gsc_player_setg_speed(scr_entref_t id)
{
	int speed;

	if ( ! stackGetParams("i", &speed))
	{
		stackError("gsc_player_setg_speed() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	if (id.entnum >= MAX_CLIENTS)
	{
		stackError("gsc_player_setg_speed() entity %i is not a player", id.entnum);
		stackPushUndefined();
		return;
	}

	if (speed < 0)
	{
		stackError("gsc_player_setg_speed() param must be equal or above zero");
		stackPushUndefined();
		return;
	}

	player_g_speed[id.entnum] = speed;
	stackPushBool(qtrue);
}

int player_g_gravity[MAX_CLIENTS] = {0};
void gsc_player_setg_gravity(scr_entref_t id)
{
	int gravity;

	if ( ! stackGetParams("i", &gravity))
	{
		stackError("gsc_player_setg_gravity() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	if (id.entnum >= MAX_CLIENTS)
	{
		stackError("gsc_player_setg_gravity() entity %i is not a player", id.entnum);
		stackPushUndefined();
		return;
	}

	if (gravity < 0)
	{
		stackError("gsc_player_setg_gravity() param must be equal or above zero");
		stackPushUndefined();
		return;
	}

	player_g_gravity[id.entnum] = gravity;
	stackPushBool(qtrue);
}

void gsc_player_setweaponfiremeleedelay(scr_entref_t id)
{
	int delay;

	if ( ! stackGetParams("i", &delay))
	{
		stackError("gsc_player_setweaponfiremeleedelay() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	if (id.entnum >= MAX_CLIENTS)
	{
		stackError("gsc_player_setweaponfiremeleedelay() entity %i is not a player", id.entnum);
		stackPushUndefined();
		return;
	}

	if (delay < 0)
	{
		stackError("gsc_player_setweaponfiremeleedelay() param must be equal or above zero");
		stackPushUndefined();
		return;
	}

	playerState_t *ps = SV_GameClientNum(id.entnum);
	ps->weaponDelay = delay;
	stackPushBool(qtrue);
}

void gsc_player_set_anim(scr_entref_t id)
{
	char *animation;

	if ( ! stackGetParams("s", &animation))
	{
		stackError("gsc_player_set_anim() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	gentity_t *entity = &g_entities[id.entnum];

	if (entity->s.eType == ET_PLAYER_CORPSE)
	{
		int index = BG_AnimationIndexForString(animation);
		entity->s.legsAnim = index;
		stackPushBool(qtrue);
		return;
	}

	if (entity->client == NULL)
	{
		stackError("gsc_player_set_anim() entity %i is not a player", id.entnum);
		stackPushUndefined();
		return;
	}

	extern int custom_animation[MAX_CLIENTS];
	int animationIndex = 0;

	if (strcmp(animation, "none") != 0)
		animationIndex = BG_AnimationIndexForString(animation);

	custom_animation[id.entnum] = animationIndex;
	stackPushBool(qtrue);
}

void gsc_player_getcooktime(scr_entref_t id)
{
	if (id.entnum >= MAX_CLIENTS)
	{
		stackError("gsc_player_getcooktime() entity %i is not a player", id.entnum);
		stackPushUndefined();
		return;
	}

	playerState_t *ps = SV_GameClientNum(id.entnum);
	stackPushInt(ps->grenadeTimeLeft);
}

void gsc_kick_slot()
{
	int clientId;
	const char* msg;
	char tmp[128];

	if ( ! stackGetParams("is", &clientId, &msg))
	{
		stackError("gsc_kick_slot() one or more arguments is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	if (clientId >= MAX_CLIENTS)
	{
		stackError("gsc_kick_slot() entity %i is not a player", clientId);
		stackPushUndefined();
		return;
	}

	client_t *client = &svs.clients[clientId];

	if (client == NULL)
	{
		stackPushUndefined();
		return;
	}

	if (client->netchan.remoteAddress.type == NA_LOOPBACK)
	{
		stackPushUndefined();
		return;
	}

	if (!client->state)
	{
		stackPushUndefined();
		return;
	}

	strncpy(tmp, msg, sizeof(tmp));
	tmp[sizeof(tmp) - 1] = '\0';

	SV_DropClient(client, tmp);
	stackPushBool(qtrue);
}

void gsc_player_setguid(scr_entref_t id)
{
	int guid;

	if ( ! stackGetParams("i", &guid))
	{
		stackError("gsc_player_setguid() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	if (id.entnum >= MAX_CLIENTS)
	{
		stackError("gsc_player_setguid() entity %i is not a player", id.entnum);
		stackPushUndefined();
		return;
	}

	client_t *client = &svs.clients[id.entnum];
	client->guid = guid;
	stackPushBool(qtrue);
}

void gsc_player_clienthasclientmuted(scr_entref_t id)
{
	int id2;

	if ( ! stackGetParams("i", &id2))
	{
		stackError("gsc_player_clienthasclientmuted() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	if (id.entnum >= MAX_CLIENTS)
	{
		stackError("gsc_player_clienthasclientmuted() entity %i is not a player", id.entnum);
		stackPushUndefined();
		return;
	}

	stackPushInt(SV_ClientHasClientMuted(id.entnum, id2));
}

void gsc_player_getlastgamestatesize(scr_entref_t id)
{
	if (id.entnum >= MAX_CLIENTS)
	{
		stackError("gsc_player_getlastgamestatesize() entity %i is not a player", id.entnum);
		stackPushUndefined();
		return;
	}

	extern int gamestate_size[MAX_CLIENTS];
	stackPushInt(gamestate_size[id.entnum]);
}

void gsc_player_getfps(scr_entref_t id)
{
	if (id.entnum >= MAX_CLIENTS)
	{
		stackError("gsc_player_getfps() entity %i is not a player", id.entnum);
		stackPushUndefined();
		return;
	}

	extern int clientfps[MAX_CLIENTS];
	stackPushInt(clientfps[id.entnum]);
}

void gsc_player_isbot(scr_entref_t id)
{
	if (id.entnum >= MAX_CLIENTS)
	{
		stackError("gsc_player_isbot() entity %i is not a player", id.entnum);
		stackPushUndefined();
		return;
	}

	client_t *client = &svs.clients[id.entnum];
	stackPushBool(client->bIsTestClient);
}

int player_disableitempickup[MAX_CLIENTS] = {0};
void gsc_player_disableitempickup(scr_entref_t id)
{
	if (id.entnum >= MAX_CLIENTS)
	{
		stackError("gsc_player_disableitempickup() entity %i is not a player", id.entnum);
		stackPushUndefined();
		return;
	}

	player_disableitempickup[id.entnum] = 1;
	stackPushBool(qtrue);
}

void gsc_player_enableitempickup(scr_entref_t id)
{
	if (id.entnum >= MAX_CLIENTS)
	{
		stackError("gsc_player_enableitempickup() entity %i is not a player", id.entnum);
		stackPushUndefined();
		return;
	}

	player_disableitempickup[id.entnum] = 0;
	stackPushBool(qtrue);
}

void gsc_player_getcurrentoffhandslotammo(scr_entref_t id)
{
	if (id.entnum >= MAX_CLIENTS)
	{
		stackError("gsc_player_getcurrentoffhandslotammo() entity %i is not a player", id.entnum);
		stackPushUndefined();
		return;
	}

	playerState_t *ps = SV_GameClientNum(id.entnum);
	stackPushInt(ps->ammoclip[ps->offHandIndex - 1]);
}

void gsc_player_getprotocol(scr_entref_t id)
{
	if (id.entnum >= MAX_CLIENTS)
	{
		stackError("gsc_player_getprotocol() entity %i is not a player", id.entnum);
		stackPushUndefined();
		return;
	}

	client_t *client = &svs.clients[id.entnum];
	stackPushInt(client->netchan.protocol);
}

void gsc_player_islocalclient(scr_entref_t id)
{
	if (id.entnum >= MAX_CLIENTS)
	{
		stackError("gsc_player_islocalclient() entity %i is not a player", id.entnum);
		stackPushUndefined();
		return;
	}

	client_t *client = &svs.clients[id.entnum];
	stackPushInt(Sys_IsLANAddress(client->netchan.remoteAddress));
}

// The player entity for a method, or NULL after reporting the error
static gentity_t *gsc_player_entity(scr_entref_t id, const char *function)
{
	if (id.entnum >= MAX_CLIENTS || g_entities[id.entnum].client == NULL)
	{
		stackError("%s() entity %i is not a player", function, id.entnum);
		stackPushUndefined();
		return NULL;
	}

	return &g_entities[id.entnum];
}

static weapSlot_t gsc_player_currentslot(const playerState_t *ps)
{
	if (ps->weapon && ps->weapon == ps->weaponslots[SLOT_PRIMARY])
		return SLOT_PRIMARY;

	if (ps->weapon && ps->weapon == ps->weaponslots[SLOT_PRIMARYB])
		return SLOT_PRIMARYB;

	return SLOT_NONE;
}

void gsc_player_isfiring(scr_entref_t id)
{
	gentity_t *ent = gsc_player_entity(id, __func__);
	if (!ent)
		return;

	stackPushBool(ent->client->ps.weaponstate == WEAPON_FIRING);
}

void gsc_player_ismeleeing(scr_entref_t id)
{
	gentity_t *ent = gsc_player_entity(id, __func__);
	if (!ent)
		return;

	stackPushBool(ent->client->ps.weaponstate == WEAPON_MELEE_FIRE);
}

void gsc_player_isreloading(scr_entref_t id)
{
	gentity_t *ent = gsc_player_entity(id, __func__);
	if (!ent)
		return;

	int state = ent->client->ps.weaponstate;
	stackPushBool(state >= WEAPON_RELOADING && state <= WEAPON_RELOAD_END);
}

void gsc_player_isthrowinggrenade(scr_entref_t id)
{
	gentity_t *ent = gsc_player_entity(id, __func__);
	if (!ent)
		return;

	int state = ent->client->ps.weaponstate;
	stackPushBool(state >= WEAPON_OFFHAND_INIT && state <= WEAPON_OFFHAND_END);
}

void gsc_player_isusingbinoculars(scr_entref_t id)
{
	gentity_t *ent = gsc_player_entity(id, __func__);
	if (!ent)
		return;

	int state = ent->client->ps.weaponstate;
	stackPushBool(state >= WEAPON_BINOCULARS_INIT && state <= WEAPON_BINOCULARS_END);
}

// isRechambering([slot]): the current weapon, or the one in "primary"/"primaryb"
void gsc_player_isrechambering(scr_entref_t id)
{
	gentity_t *ent = gsc_player_entity(id, __func__);
	if (!ent)
		return;

	playerState_t *ps = &ent->client->ps;
	int slot = Scr_GetNumParam() ? BG_GetWeaponSlotForName(Scr_GetString(0)) : gsc_player_currentslot(ps);

	if (slot <= SLOT_NONE || slot >= SLOT_COUNT)
	{
		stackPushBool(qfalse);
		return;
	}

	stackPushBool(Com_BitCheck(ps->weaponrechamber, ps->weaponslots[slot]) ? qtrue : qfalse);
}

void gsc_player_isshellshocked(scr_entref_t id)
{
	gentity_t *ent = gsc_player_entity(id, __func__);
	if (!ent)
		return;

	playerState_t *ps = &ent->client->ps;
	stackPushBool(ps->shellshockIndex != 0 && level.time <= ps->shellshockTime + ps->shellshockDuration);
}

void gsc_player_ischatting(scr_entref_t id)
{
	gentity_t *ent = gsc_player_entity(id, __func__);
	if (!ent)
		return;

	stackPushBool(ent->client->buttons & BUTTON_TALK ? qtrue : qfalse);
}

void gsc_player_button_holdbreath(scr_entref_t id)
{
	if (!gsc_player_entity(id, __func__))
		return;

	stackPushBool(svs.clients[id.entnum].lastUsercmd.buttons & BUTTON_HOLDBREATH ? qtrue : qfalse);
}

// Reserve ammo of the current weapon, or its clip for clip-only weapons
void gsc_player_getcurrentweaponammo(scr_entref_t id)
{
	gentity_t *ent = gsc_player_entity(id, __func__);
	if (!ent)
		return;

	playerState_t *ps = &ent->client->ps;

	if (!G_IsPlaying(ent) || !ps->weapon)
		stackPushInt(0);
	else if (BG_WeaponIsClipOnly(ps->weapon))
		stackPushInt(ps->ammoclip[BG_ClipForWeapon(ps->weapon)]);
	else
		stackPushInt(ps->ammo[BG_AmmoForWeapon(ps->weapon)]);
}

void gsc_player_getcurrentweaponclipammo(scr_entref_t id)
{
	gentity_t *ent = gsc_player_entity(id, __func__);
	if (!ent)
		return;

	playerState_t *ps = &ent->client->ps;

	if (!G_IsPlaying(ent) || !ps->weapon)
		stackPushInt(0);
	else
		stackPushInt(ps->ammoclip[BG_ClipForWeapon(ps->weapon)]);
}

// Returns false when the player holds no weapon
void gsc_player_setcurrentweaponammo(scr_entref_t id)
{
	int ammo;

	if (!stackGetParams("i", &ammo))
	{
		stackError("gsc_player_setcurrentweaponammo() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	gentity_t *ent = gsc_player_entity(id, __func__);
	if (!ent)
		return;

	playerState_t *ps = &ent->client->ps;

	if (!G_IsPlaying(ent) || !ps->weapon)
	{
		stackPushBool(qfalse);
		return;
	}

	if (ammo < 0)
		ammo = 0;

	if (BG_WeaponIsClipOnly(ps->weapon))
	{
		int clip = BG_ClipForWeapon(ps->weapon);
		ps->ammoclip[clip] = ammo > BG_GetAmmoClipSize(clip) ? BG_GetAmmoClipSize(clip) : ammo;
	}
	else
	{
		int type = BG_AmmoForWeapon(ps->weapon);
		ps->ammo[type] = ammo > BG_GetAmmoTypeMax(type) ? BG_GetAmmoTypeMax(type) : ammo;
	}

	stackPushBool(qtrue);
}

void gsc_player_setcurrentweaponclipammo(scr_entref_t id)
{
	int ammo;

	if (!stackGetParams("i", &ammo))
	{
		stackError("gsc_player_setcurrentweaponclipammo() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	gentity_t *ent = gsc_player_entity(id, __func__);
	if (!ent)
		return;

	playerState_t *ps = &ent->client->ps;

	if (!G_IsPlaying(ent) || !ps->weapon)
	{
		stackPushBool(qfalse);
		return;
	}

	int clip = BG_ClipForWeapon(ps->weapon);

	if (ammo < 0)
		ammo = 0;

	ps->ammoclip[clip] = ammo > BG_GetAmmoClipSize(clip) ? BG_GetAmmoClipSize(clip) : ammo;
	stackPushBool(qtrue);
}

// "primary", "primaryb" or "none" (offhand, binoculars or no weapon)
void gsc_player_getcurrentweaponslot(scr_entref_t id)
{
	gentity_t *ent = gsc_player_entity(id, __func__);
	if (!ent)
		return;

	stackPushString(BG_GetWeaponSlotNameForIndex(gsc_player_currentslot(&ent->client->ps)));
}

void gsc_player_getspeed(scr_entref_t id)
{
	gentity_t *ent = gsc_player_entity(id, __func__);
	if (!ent)
		return;

	stackPushInt(ent->client->ps.speed);
}

void gsc_player_getgravity(scr_entref_t id)
{
	gentity_t *ent = gsc_player_entity(id, __func__);
	if (!ent)
		return;

	stackPushInt(ent->client->ps.gravity);
}

void gsc_player_getgroundentity(scr_entref_t id)
{
	gentity_t *ent = gsc_player_entity(id, __func__);
	if (!ent)
		return;

	int ground = ent->client->ps.groundEntityNum;

	// undefined in the air; standing on the map gives the world entity
	if (ground == ENTITYNUM_NONE)
		stackPushUndefined();
	else
		stackPushEntity(&g_entities[ground]);
}

void gsc_player_getvieworigin(scr_entref_t id)
{
	gentity_t *ent = gsc_player_entity(id, __func__);
	if (!ent)
		return;

	vec3_t origin;
	G_GetPlayerViewOrigin(ent, origin);
	stackPushVector(origin);
}

void gsc_player_getplayerstateflags(scr_entref_t id)
{
	gentity_t *ent = gsc_player_entity(id, __func__);
	if (!ent)
		return;

	stackPushInt(ent->client->ps.pm_flags);
}

void gsc_player_getinactivitytime(scr_entref_t id)
{
	gentity_t *ent = gsc_player_entity(id, __func__);
	if (!ent)
		return;

	stackPushInt(ent->client->inactivityTime);
}

void gsc_player_getentertime(scr_entref_t id)
{
	gentity_t *ent = gsc_player_entity(id, __func__);
	if (!ent)
		return;

	stackPushInt(ent->client->sess.enterTime);
}

// Kills the player as suicide() did before CodeCallback_Suicide replaced it
void gsc_player_processsuicide(scr_entref_t id)
{
	gentity_t *ent = gsc_player_entity(id, __func__);
	if (!ent)
		return;

	ent->flags &= ~(FL_GODMODE | FL_DEMI_GODMODE);
	ent->health = 0;
	ent->client->ps.stats[STAT_HEALTH] = 0;

	player_die(ent, ent, ent, 100000, MOD_SUICIDE, WP_NONE, NULL, HITLOC_NONE, 0);
}

// As get_userinfo, but "" for a missing key, like zk_libcod's getUserinfo
void gsc_player_getuserinfo(scr_entref_t id)
{
	const char *key;

	if (!stackGetParams("s", &key))
	{
		stackError("gsc_player_getuserinfo() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	if (id.entnum >= MAX_CLIENTS)
	{
		stackError("gsc_player_getuserinfo() entity %i is not a player", id.entnum);
		stackPushUndefined();
		return;
	}

	stackPushString(Info_ValueForKey(svs.clients[id.entnum].userinfo, key));
}

// Methods below are named and behave as in zk_libcod

static bool gsc_player_isclient(scr_entref_t id, const char *function)
{
	if (id.entnum >= MAX_CLIENTS)
	{
		stackError("%s() entity %i is not a player", function, id.entnum);
		stackPushUndefined();
		return false;
	}
	return true;
}

static void gsc_player_setmuted(scr_entref_t id, const char *function, bool muted)
{
	int other;

	if (!stackGetParams("i", &other))
	{
		stackError("%s() argument is undefined or has a wrong type", function);
		stackPushUndefined();
		return;
	}

	if (!gsc_player_isclient(id, function))
		return;

	if (other < 0 || other >= MAX_CLIENTS)
	{
		stackError("%s() client %i is not a player", function, other);
		stackPushUndefined();
		return;
	}

	// the same list the client's own muteclient command edits
	svs.clients[id.entnum].muteList[other] = muted;
	stackPushBool(qtrue);
}

void gsc_player_muteclient(scr_entref_t id)
{
	gsc_player_setmuted(id, "gsc_player_muteclient", true);
}

void gsc_player_unmuteclient(scr_entref_t id)
{
	gsc_player_setmuted(id, "gsc_player_unmuteclient", false);
}

void gsc_player_sethiddenfromscoreboard(scr_entref_t id)
{
	int hidden;

	if (!stackGetParams("i", &hidden))
	{
		stackError("gsc_player_sethiddenfromscoreboard() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	if (!gsc_player_isclient(id, "gsc_player_sethiddenfromscoreboard"))
		return;

	libcodClients[id.entnum].hiddenFromScoreboard = hidden != 0;
	stackPushBool(qtrue);
}

void gsc_player_ishiddenfromscoreboard(scr_entref_t id)
{
	if (!gsc_player_isclient(id, "gsc_player_ishiddenfromscoreboard"))
		return;

	stackPushBool(libcodClients[id.entnum].hiddenFromScoreboard);
}

void gsc_player_sethiddenfromserverstatus(scr_entref_t id)
{
	int hidden;

	if (!stackGetParams("i", &hidden))
	{
		stackError("gsc_player_sethiddenfromserverstatus() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	if (!gsc_player_isclient(id, "gsc_player_sethiddenfromserverstatus"))
		return;

	libcodClients[id.entnum].hiddenFromServerStatus = hidden != 0;
	stackPushBool(qtrue);
}

void gsc_player_ishiddenfromserverstatus(scr_entref_t id)
{
	if (!gsc_player_isclient(id, "gsc_player_ishiddenfromserverstatus"))
		return;

	stackPushBool(libcodClients[id.entnum].hiddenFromServerStatus);
}

void gsc_player_setallowspectators(scr_entref_t id)
{
	int allowed;

	if (!stackGetParams("i", &allowed))
	{
		stackError("gsc_player_setallowspectators() argument is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	if (!gsc_player_isclient(id, "gsc_player_setallowspectators"))
		return;

	libcodClients[id.entnum].notAllowingSpectators = !allowed;

	if (!allowed)
	{
		// whoever follows the player now goes back to free spectating
		for (int i = 0; i < level.maxclients; i++)
		{
			gentity_t *ent = &g_entities[i];

			if (i == id.entnum || !ent->client || ent->client->sess.connected == CON_DISCONNECTED)
				continue;

			if (ent->client->sess.sessionState == SESS_STATE_SPECTATOR && ent->client->spectatorClient == id.entnum)
				StopFollowing(ent);
		}
	}

	stackPushBool(qtrue);
}

void gsc_player_isallowingspectators(scr_entref_t id)
{
	if (!gsc_player_isclient(id, "gsc_player_isallowingspectators"))
		return;

	stackPushBool(!libcodClients[id.entnum].notAllowingSpectators);
}

// Sends one configstring to this player only. The next gamestate (map change,
// reconnect) brings back the value every player has.
void gsc_player_setconfigstringforplayer(scr_entref_t id)
{
	int index;
	const char *value;

	if (!stackGetParams("is", &index, &value))
	{
		stackError("gsc_player_setconfigstringforplayer() one or more arguments is undefined or has a wrong type");
		stackPushUndefined();
		return;
	}

	if (!gsc_player_isclient(id, "gsc_player_setconfigstringforplayer"))
		return;

	if (index < 0 || index >= MAX_CONFIGSTRINGS)
	{
		stackError("gsc_player_setconfigstringforplayer() bad index %i", index);
		stackPushUndefined();
		return;
	}

	client_t *cl = &svs.clients[id.entnum];
	if (cl->state < CS_PRIMED)
	{
		stackPushBool(qfalse);
		return;
	}

	SV_SendConfigstring(cl, index, value);
	stackPushBool(qtrue);
}

// Reliable commands sent to this player and not acknowledged yet; the
// player is dropped when it reaches MAX_RELIABLE_COMMANDS
void gsc_player_getservercommandqueuesize(scr_entref_t id)
{
	if (!gsc_player_isclient(id, "gsc_player_getservercommandqueuesize"))
		return;

	client_t *cl = &svs.clients[id.entnum];
	stackPushInt(cl->reliableSequence - cl->reliableAcknowledge);
}

void gsc_player_getcdkeyhash(scr_entref_t id)
{
	if (!gsc_player_isclient(id, "gsc_player_getcdkeyhash"))
		return;

	stackPushString(svs.clients[id.entnum].clientPBguid);
}

// Hud elements created only for this player (newClientHudElem)
void gsc_player_getclienthudelemcount(scr_entref_t id)
{
	if (!gsc_player_isclient(id, "gsc_player_getclienthudelemcount"))
		return;

	int count = 0;
	for (int i = 0; i < MAX_HUDELEMS_TOTAL; i++)
	{
		if (g_hudelems[i].elem.type != HE_TYPE_FREE && g_hudelems[i].clientNum == id.entnum)
			count++;
	}
	stackPushInt(count);
}

#endif
