#pragma once
#ifndef _PROJECTILE_H
#define _PROJECTILE_H

#include "Entity.h"
#include "Timer.h"

#include "Game.h"
#include "AnimationManager.h"

extern Game *game;

class Projectile : public Entity
{
	protected:
		Entity *mpOwner;
		Timer mLifeTimer;
		Vec3d mVelocity;

		Vec2d mDamageRange;

	public:
		Projectile::Projectile(Vec3d &position, Vec3d &velocity, Animation *animation, Vec2d& damageRange, entity_types type = ENTITY_ATTACK_ENEMY);
		Projectile(Vec3d &position, Vec3d &velocity, Animation *animation, Vec2d& scale, Vec2d& damageRange, entity_types type = ENTITY_ATTACK_ENEMY);
		~Projectile();

		Vec2d& getDamageRange() { return mDamageRange; }
		
		virtual void think(const double elapsedTime);
		virtual void draw();
		virtual void collide(Entity* other);

		virtual Vec3d getVelocity() { return mVelocity; }
		virtual void setVelocity(Vec3d &vec) { mVelocity = vec; }

};

#endif