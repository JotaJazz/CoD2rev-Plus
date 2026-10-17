// Objectives for the simple bots: SD (plant, guard, defuse) and CTF (steal,
// capture, defend, return). Each life a bot draws a role; the role gives
// _bots_nav a goal and, on arrival, an action. Other gametypes are untouched.

init()
{
	level.botObjMode = undefined;
	level.botBombPrecomputed = false;
	gametype = getCvar("g_gametype");

	if(gametype == "sd" && isDefined(level.bombmode) && level.bombmode < 2)
	{
		level.botZones = getentarray("bombzone", "targetname");
		if(!level.botZones.size)
			return;
		level.botObjMode = "sd";
		for(i = 0; i < level.botZones.size; i++)
			precomputeTo(level.botZones[i].origin);
	}
	else if(gametype == "ctf")
	{
		level.botFlags = [];
		level.botFlags["allies"] = getent("allied_flag", "targetname");
		level.botFlags["axis"] = getent("axis_flag", "targetname");
		if(!isDefined(level.botFlags["allies"]) || !isDefined(level.botFlags["axis"]))
			return;
		if(!isDefined(level.botFlags["allies"].home_origin) || !isDefined(level.botFlags["axis"].home_origin))
			return;
		level.botObjMode = "ctf";
		precomputeTo(level.botFlags["allies"].home_origin);
		precomputeTo(level.botFlags["axis"].home_origin);
	}
}

// Paths to a fixed objective are computed once; graphFindPath then follows
// them without searching.
precomputeTo(pos)
{
	if(!isDefined(level.botGraph))
		return;

	node = graphFindClosestNode(level.botGraph, pos);
	if(isDefined(node))
		graphPrecomputePathsToNode(level.botGraph, node);
}

// A role for a new life. A bot only gets to be a hunter when a teammate
// already plays the objective, so small teams always play it.
resetObjective()
{
	self.botRole = "hunter";
	self.botZone = undefined;
	self.botGuardPos = undefined;
	self.botGuardCenter = undefined;
	self.botGuardUntil = 0;
	self.botGuardYaw = 0;
	self.botGuarding = false;
	self.botCrouched = false;
	self.botObjAction = undefined;
	self.botObjStart = 0;
	self.botHadFlag = false;
	self.botReturnPos = undefined;

	if(!isDefined(level.botObjMode))
		return;

	role = undefined;
	if(level.botObjMode == "sd")
	{
		if(self.pers["team"] == game["attackers"])
			role = "plant";
		else
			role = "guard";

		if(randomInt(3) == 0 && self teamHasRole(role))
			role = "hunter";
		else
			self.botZone = level.botZones[randomInt(level.botZones.size)];
	}
	else if(randomInt(2) == 0)
		role = "steal";
	else
		role = "defend";

	self setRole(role);
}

setRole(role)
{
	if(self.botRole == role && role != "hunter")
		return;

	self.botRole = role;
	maps\mp\bots\_bots_nav::debugLog("BOTOBJ " + self getEntityNumber() + " role " + role);
}

teamHasRole(role)
{
	players = getentarray("player", "classname");
	for(i = 0; i < players.size; i++)
	{
		other = players[i];
		if(other == self || !isAlive(other) || other.sessionstate != "playing")
			continue;
		if(other.pers["team"] == self.pers["team"] && isDefined(other.botRole) && other.botRole == role)
			return true;
	}
	return false;
}
