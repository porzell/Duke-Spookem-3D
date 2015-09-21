#pragma once
#ifndef _HealthPotion_H
#define _HealthPotion_H

#include "Entity.h"
#include "Timer.h"

#include "Game.h"
#include "AnimationManager.h"

extern Game *game;

class HealthPotion : public Entity
{
	protected:
		Vec2d mHealthRange;

	public:
		HealthPotion(Vec3d &position, Vec2d healthRange = Vec2d(10.0f, 20.f));
		~HealthPotion();

		virtual void think(const double elapsedTime);
		virtual void draw();
		virtual void collide(Entity* other);

};

#endif
