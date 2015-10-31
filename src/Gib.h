#ifndef _Gib_H
#define _Gib_H

#include "Entity.h"

#include "Game.h"

#include "Monster.h"

#include "Timer.h"

extern Game *game;

enum GIB_TYPE {GIB_DEFAULT, GIB_HEAD, GIB_TORSO, GIB_ARM, GIB_LEG, GIB_CIRCUIT, GIB_METAL};

class Gib : public Entity
{
	protected:
		double mTimeToLive;
		Timer mLifeTimer;

		Vec3d mVelocity;

		float rotator;
		
		GIB_TYPE mGibType;

		bool mKicked;

		Vec3d mKickPosition;

	public:
		Gib(Animation *anim, Vec3d &position, Vec3d velocity, bool frozen = false, GIB_TYPE type = GIB_DEFAULT);
		virtual ~Gib();

		//Get the type of gib.
		inline virtual GIB_TYPE getGibType() { return mGibType; }
		
		virtual void think(const double elapsedTime);
		virtual void draw();
		virtual void collide(Entity* other);

		virtual void setFrozen(bool frozen);

		void playGibSound();

};

#endif
