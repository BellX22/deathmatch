#pragma once

namespace gameconfig {
	// Bot
	const float BOT_BOUNDINGRADIUS = 0.2f;
	const float BOT_MAX_SPEED = 4.f;
	const float BOT_MASS = 1.f;
	const int   BOT_MAX_HEALTH = 100;
	const float BOT_CHANGEWEAPONTIMEOUT = 1.f;
	const float BOT_WEAPON_SELECTION_TIME = 0.5f;

	// Weapon
	const int   BLASTER_MAXROUNDS = 100;
	const float BLASTER_FIRERATE = 5.f; // per sec

	const int   SHOTGUN_MAXROUNDS = 30;
	const float SHOTGUN_FIRERATE = 1.f; // per sec

	const int   ROCKETLAUNCHER_MAXROUNDS = 100;
	const float ROCKETLAUNCHER_FIRERATE = 1.f; // per sec

	const int   RAILGUN_MAXROUNDS = 10;
	const float RAILGUN_FIRERATE = 0.5f; // per sec

	// Ammunition
	const float BOLT_LENGTH = 0.5f;
	const float BOLT_MAX_SPEED = 50.f;
	const float BOLT_DMG = 10.f;
	const float BOLT_RANGE = 100.f;

	const float ROCKET_BOUNDINGRADIUS = 0.3f;
	const float ROCKET_MAX_SPEED = 15.f;
	const float ROCKET_MAX_BLASTRADIUS = 3.f;
	const float ROCKET_BLASTRADIUS_SPEED = 3.f; // per sec
	const float ROCKET_BLASTRADIUS_PUSHBACK = 30.f; // per sec
	const float ROCKET_DMG = 50.f;
	const float ROCKET_RANGE = 100.f;

	const float PELLET_RANGE = 1000.f;
	const float PELLET_DMG = 20.f;
	const float PELLET_SECS_VISIBLE = 0.2f;

	const float SLUG_RANGE = 1000.f;
	const float SLUG_DMG = 70.f;
	const float SLUG_SECS_VISIBLE = 0.2f;
}
