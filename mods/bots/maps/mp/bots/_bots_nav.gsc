// Navigation for the simple bots: one waypoint graph per map, built with the
// libcod graph* functions and kept in waypoints/<map>_v1.graph, and A* paths
// over it. Without a graph the bots walk straight at their goal.

init()
{
	level.botsDebug = getCvarInt("bots_debug");
	level.botGraph = undefined;

	file = "waypoints/" + getCvar("mapname") + "_v1.graph";
	graph = graphLoad(file);

	if(isDefined(graph) && graphGetNodeCount(graph) > 0)
	{
		level.botGraph = graph;
		debugLog("BOTNAV graph loaded " + graphGetNodeCount(graph));
		return;
	}

	if(isDefined(graph))
		graphRemove(graph);

	graph = generateGraph();
	if(!isDefined(graph))
	{
		debugLog("BOTNAV graph none");
		return;
	}

	level.botGraph = graph;
	graphSave(graph, file);
	debugLog("BOTNAV graph generated " + graphGetNodeCount(graph));
}

// Every spawn point of every gametype seeds the flood fill, so floors and
// areas a single seed would miss are covered too.
generateGraph()
{
	graph = graphCreate();
	if(!isDefined(graph))
		return undefined;

	classes = spawnClasses();
	for(c = 0; c < classes.size; c++)
	{
		spawns = getentarray(classes[c], "classname");
		for(i = 0; i < spawns.size; i++)
			graphAutodiscoverEx(graph, spawns[i].origin, 64);
	}

	if(graphGetNodeCount(graph) == 0)
	{
		graphRemove(graph);
		return undefined;
	}

	return graph;
}

spawnClasses()
{
	classes = [];
	classes[0] = "mp_dm_spawn";
	classes[1] = "mp_tdm_spawn";
	classes[2] = "mp_sd_spawn_attacker";
	classes[3] = "mp_sd_spawn_defender";
	classes[4] = "mp_ctf_spawn_allied";
	classes[5] = "mp_ctf_spawn_axis";
	return classes;
}

debugLog(text)
{
	if(level.botsDebug)
		logprint(text + "\n");
}
