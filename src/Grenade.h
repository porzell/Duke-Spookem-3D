#pragma once
#ifndef _GRENADE_H
#define _GRENADE_H

#include "Projectile.h"
#include "Timer.h"

#include "Game.h"
#include "AnimationManager.h"

#include "SoundEngine.h"

extern Game *game;

class Grenade : public Projectile
{
	private:
		GLfloat mBlastRadius;
		int ttl;
		float rotator;
		//Sound *mVoice;

	public:
		Grenade(Vec3d &position, Vec3d &velocity, Animation *animation, Vec2d& damageRange, entity_types type, float radius);
		~Grenade();

		virtual void think(const double elapsedTime);
		virtual void draw();
		virtual void collide(Entity* other);

		virtual void quiet();
		virtual void unQuiet();
};

#endif
