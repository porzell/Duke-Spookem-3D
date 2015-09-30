#ifndef _EXPLOSION_H
#define _EXPLOSION_H

#include "Entity.h"
#include "Timer.h"

#include "Game.h"
#include "AnimationManager.h"

extern Game *game;

class Explosion : public Entity
{
	private:
        Timer mLifeTimer;
		Vec2d mDamageRange;
		float mDamage;

	public:
		Explosion(Vec3d &position);
		Explosion(Vec3d &position, GLfloat scale);
		Explosion(Vec3d &position, GLfloat scale, Vec2d damageRange);
		~Explosion();

		virtual void think(const double elapsedTime);
		virtual void draw();
		virtual void collide(Entity* other);

};

#endif
