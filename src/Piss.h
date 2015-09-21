#pragma once
#ifndef _PISS_H
#define _PISS_H

#include "Projectile.h"
#include "Timer.h"

#include "Game.h"
#include "AnimationManager.h"

extern Game *game;

class Piss : public Projectile
{
	private:
		std::string mLastPiss;
		Vec3d mLastPosition;
		Vec3d mOffset;
		GLfloat mBlastRadius;

		double mLifeTime;

	public:
		Piss(std::string lastPiss, Vec3d position, Vec3d velocity, entity_types type);
		~Piss();

		virtual void think(const double elapsedTime);
		virtual void draw();
		virtual void collide(Entity* other);

};

#endif
