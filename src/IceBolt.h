#pragma once
#ifndef _ICE_BOLT_H
#define _ICE_BOLT_H

#include "Projectile.h"

#include "Game.h"

extern Game *game;

class IceBolt : public Projectile
{
	private:

	public:
		IceBolt(Vec3d position, Vec3d velocity, Animation *animation, Vec2d damageRange, entity_types type);
		~IceBolt();

		virtual void think(const double elapsedTime);
		virtual void draw();
		virtual void collide(Entity* other);

};

#endif
