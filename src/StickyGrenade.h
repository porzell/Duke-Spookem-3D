#pragma once
#ifndef _STICKY_GRENADE_H
#define _STICKY_GRENADE_H

#include "Projectile.h"
#include "Timer.h"

#include "Game.h"
#include "AnimationManager.h"

extern Game *game;

class StickyGrenade : public Projectile
{
	private:
		Vec3d mOffset;
		Entity *stuck;
		GLfloat mBlastRadius;

		double mLifeTime;

	public:
		StickyGrenade(Vec3d position, Vec3d velocity, Animation *animation, Vec2d damageRange, entity_types type, float radius);
		~StickyGrenade();

		virtual void think(const double elapsedTime);
		virtual void draw();
		virtual void collide(Entity* other);

};

#endif
