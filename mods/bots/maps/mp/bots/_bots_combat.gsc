// Fighting for the simple bots: aim, fire, aim down the sight at range, knife
// up close, reload, and grenades (tryGrenade).

resetCombat()
{
	self.botLastTarget = undefined;
	self.botReactAt = 0;
	self.botAdsAt = 0;
	self.botAds = false;
	self.botTick = 0;
	self.botStrafeDir = "left";
	self.botStrafeUntil = 0;
	self.botThrowing = false;
	self.botNadeAt = 0;
}

// One tick of fighting the target in sight.
fight(target, skill)
{
	now = getTime();
	num = self getEntityNumber();
	self.botTick++;

	// a new target is not shot at instantly, and the sight takes a moment
	if(!isDefined(self.botLastTarget) || self.botLastTarget != target)
	{
		self.botLastTarget = target;
		self.botReactAt = now + (10 - skill) * 60;
		self.botAdsAt = now + 200 + (10 - skill) * 80;
	}

	dist = distance(self.origin, target.origin);
	class = weaponClass(self getCurrentWeapon());
	eye = self getEye();

	if(dist < 60)
	{
		self adsAim(0);
		self holdBreath(0);
		self.botAds = false;
		self fireWeapon(0);
		self reloadWeapon(0);
		self maps\mp\bots\_bots::lookAt(vectorToAngles(target.origin + (0, 0, 40) - eye));

		if(self.botTick % 3 == 0)
		{
			self meleeWeapon(1);
			maps\mp\bots\_bots_nav::debugLog("BOTCOMBAT " + num + " melee");
		}
		else
			self meleeWeapon(0);

		self setWalkDir("none");
		self setWalkDir("forward");
		return;
	}

	self meleeWeapon(0);

	wantAds = false;
	if(class == "sniper" || class == "rifle" || class == "bolt")
		wantAds = dist > 400;
	else if(class == "mg")
		wantAds = dist > 800;

	if(wantAds && now >= self.botAdsAt)
	{
		if(!self.botAds)
			maps\mp\bots\_bots_nav::debugLog("BOTCOMBAT " + num + " ads");
		self adsAim(1);
		self.botAds = true;
	}
	else
	{
		self adsAim(0);
		self.botAds = false;
	}

	if(class == "sniper" && self.botAds)
		self holdBreath(1);
	else
		self holdBreath(0);

	spread = (10 - skill) * 3;
	if(self.botAds)
		spread = spread / 2;
	aimAt = target getEye() + (randomFloat(spread * 2) - spread, randomFloat(spread * 2) - spread, randomFloat(spread * 2) - spread - 14);
	self maps\mp\bots\_bots::lookAt(vectorToAngles(aimAt - eye));

	clip = self getCurrentWeaponClipAmmo();
	reloading = clip == 0 || self isReloading();

	if(reloading)
	{
		self fireWeapon(0);
		if(clip == 0 && !self isReloading())
			self reloadWeapon(1);
		else
			self reloadWeapon(0);
	}
	else
	{
		self reloadWeapon(0);
		// three ticks on, one off: bursts for automatics, repeated shots for rifles
		if(now >= self.botReactAt && (self.botTick % 4) != 0)
			self fireWeapon(1);
		else
			self fireWeapon(0);
	}

	// up close, long guns back off and submachine guns close in to knife range
	self setWalkDir("none");
	if(!reloading)
	{
		if(dist > 600)
			self setWalkDir("forward");
		else if(dist < 150 && (class == "sniper" || class == "bolt"))
			self setWalkDir("back");
		else if(dist < 150 && (class == "smg" || class == "pistol"))
			self setWalkDir("forward");
	}

	if(now >= self.botStrafeUntil)
	{
		if(randomInt(2) == 0)
			self.botStrafeDir = "left";
		else
			self.botStrafeDir = "right";
		self.botStrafeUntil = now + 600 + randomInt(900);
	}
	self setWalkDir(self.botStrafeDir);
}

// With nobody in sight: reload once the clip is below half.
idleReload()
{
	self fireWeapon(0);
	self adsAim(0);
	self holdBreath(0);
	self.botAds = false;

	weapon = self getCurrentWeapon();
	if(weapon == "none" || self isReloading())
	{
		self reloadWeapon(0);
		return;
	}

	size = getWeaponClipSize(weapon);
	if(isDefined(size) && self getCurrentWeaponClipAmmo() * 2 < size)
		self reloadWeapon(1);
	else
		self reloadWeapon(0);
}

weaponClass(name)
{
	switch(name)
	{
	case "thompson_mp":
	case "greasegun_mp":
	case "sten_mp":
	case "ppsh_mp":
	case "pps42_mp":
	case "PPS42_mp":
	case "mp40_mp":
	case "shotgun_mp":
		return "smg";
	case "m1carbine_mp":
	case "m1garand_mp":
	case "svt40_mp":
	case "g43_mp":
		return "rifle";
	case "enfield_mp":
	case "mosin_nagant_mp":
	case "kar98k_mp":
		return "bolt";
	case "springfield_mp":
	case "enfield_scope_mp":
	case "mosin_nagant_sniper_mp":
	case "kar98k_sniper_mp":
		return "sniper";
	case "bar_mp":
	case "bren_mp":
	case "mp44_mp":
		return "mg";
	case "colt_mp":
	case "webley_mp":
	case "tt30_mp":
	case "luger_mp":
		return "pistol";
	}

	return "rifle";
}
