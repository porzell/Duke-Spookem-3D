#pragma once
#ifndef _SWAMPMONSTER_H
#define _SWAMPMONSTER_H

#include "Monster.h"

#include "Timer.h"

#include "SoundEngine.h"

class SwampMonster : public Monster
{
	private:
		/*static Animation *msHeadGib;
		static Animation *msTorsoGib;
		static Animation *msLegGib;
		static Animation *msArmGib;*/
		
	public:
		SwampMonster(Vec3d pos);
		virtual ~SwampMonster() {};

		virtual void collide(Entity* other);
		
		virtual void think(const double elapsedTime);
		//virtual void draw();

		virtual void kill();

		virtual void spawnGib(float velocity);

		//virtual void addAttachment(Entity *attachment);

		//virtual void stun(Vec3d startPos);

		virtual void removeAttachment(Entity *entity);
		virtual void removeAttachment(std::string entityName);

		virtual void makeMonsterNoise(bool priority = false);

		virtual void takeDamage(float hp = 0);

		virtual void setFrozen(bool frozen);

		//virtual std::string getSoundFireScream();
		
		virtual std::string getAttackSound();
		
		virtual void attack();

		virtual void quiet() { if(mVoice) mVoice->setIsPaused(true); }
		virtual void unQuiet() { if(mVoice) mVoice->setIsPaused(false); }
};

#endif