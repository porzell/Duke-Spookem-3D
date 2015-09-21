#pragma once
#ifndef _NaziBullet_H
#define _NaziBullet_H

#include "Projectile.h"
#include "Timer.h"

#include "Game.h"
#include "AnimationManager.h"

extern Game *game;

class NaziBullet : public Projectile
{
	public:
		NaziBullet(Vec3d position, Vec3d velocity, Animation *anim);
		~NaziBullet();

		Vec2d& getDamageRange() { return mDamageRange; }
		
		virtual void draw();
		virtual void think(const double elapsedTime);
		virtual void collide(Entity* other);
		
		static std::string getRicochetSound();

};

#endif
