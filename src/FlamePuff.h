#pragma once
#ifndef _FLAME_PUFF_H
#define _FLAME_PUFF_H

#include "Projectile.h"
#include "Timer.h"

#include "Game.h"
#include "AnimationManager.h"

extern Game *game;

class FlamePuff : public Projectile
{
	private:
		Vec3d mOffset;
		Entity *stuck;
		GLfloat mBlastRadius;

		double mLifeTime;

	public:
		FlamePuff(Vec3d &position, Vec3d &velocity, Animation *animation, Vec2d& damageRange, entity_types type);
		~FlamePuff();

		virtual void think(const double elapsedTime);
		virtual void draw();
		virtual void collide(Entity* other);

};

#endif