#pragma once
#ifndef _Puddle_H
#define _Puddle_H

#include "Projectile.h"
#include "Timer.h"

#include "Game.h"
#include "AnimationManager.h"

extern Game *game;

class Puddle : public Projectile
{
	private:
		Vec3d mOffset;

		double mLifeTime;

	public:
		Puddle(Vec3d &position, entity_types type);
		~Puddle();

		virtual void think(const double elapsedTime);
		virtual void draw();
		virtual void collide(Entity* other);

};

#endif