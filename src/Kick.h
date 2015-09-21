#ifndef _KICK_H
#define _KICK_H

#include "Entity.h"
#include "Timer.h"

#include "Game.h"
#include "AnimationManager.h"

//typedef irrklang::ISound Sound;

extern Game *game;

class Kick : public Entity
{
	private:
		bool mHasHitHeadGib;

		//Timer mLifeTimer;
		//Sound *mVoice;

	public:
		Kick(Vec3d &position);
		Kick(Vec3d &position, GLfloat scale);
		~Kick();

		virtual void think(const double elapsedTime);
		virtual void draw();
		virtual void collide(Entity* other);

};

#endif
