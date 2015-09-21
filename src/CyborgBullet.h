#pragma once
#ifndef _CyborgBullet_H
#define _CyborgBullet_H

#include "Projectile.h"
#include "Timer.h"

#include "Game.h"
#include "AnimationManager.h"

#include "Color.h"

extern Game *game;

//typedef irrklang::ISound Sound;

class CyborgBullet : public Projectile
{
	private:
		Color mColor;
		//Sound *mpSound;
	
	public:
		CyborgBullet(Vec3d &position, Vec3d &velocity, Color &color);
		~CyborgBullet();

		Vec2d& getDamageRange() { return mDamageRange; }
		
		virtual void draw();
		virtual void think(const double elapsedTime);
		virtual void collide(Entity* other);
		
		//virtual void quiet();
		//virtual void unQuiet();

		static std::string getRicochetSound();

};

#endif
