#pragma once
#ifndef _ShellCasing_H
#define _ShellCasing_H

#include "Entity.h"

#include "Game.h"

#include "Timer.h"

extern Game *game;

class ShellCasing : public Entity
{
	protected:
		double mTimeToLive;
		//Timer mLifeTimer;

		Vec3d mVelocity;

		float rotator;

	public:
		ShellCasing(Vec3d position, Vec3d velocity);
		ShellCasing(Animation *anim, Vec3d position, Vec3d velocity);
		virtual ~ShellCasing();
		
		virtual void think(const double elapsedTime);
		virtual void draw();
		virtual void collide(Entity* other) {}

		void playShellCasingSound();

};

#endif
