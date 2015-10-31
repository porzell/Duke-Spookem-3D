#pragma once
#ifndef _NAZI_H
#define _NAZI_H

#include "Monster.h"

#include "Timer.h"

//typedef irrklang::ISound Sound;

class Nazi : public Monster
{
	private:
		/*static Animation *msHeadGib;
		static Animation *msTorsoGib;
		static Animation *msLegGib;
		static Animation *msArmGib;*/
		Timer mWeaponTimer;
		Timer mShotTimer;
		
		//Sound *mpWeaponSound;
		
		float mFireTime;

		Animation *fireballAnim;

		Animation *oldAnim;

		bool mIsOfficer;

		void fireWeapon();
		
	public:
		Nazi(Vec3d pos);
		virtual ~Nazi();

		virtual void collide(Entity* other);

		virtual void think(const double elapsedTime);
		//virtual void draw();

		virtual void kill();

		virtual void spawnGib(float velocity);

		//virtual void addAttachment(Entity *attachment);

		virtual void stun(Vec3d startPos);

		virtual void removeAttachment(Entity *entity);
		virtual void removeAttachment(std::string entityName);

		virtual void makeMonsterNoise(bool priority = false);

		virtual void takeDamage(float hp = 0);

		virtual void setFrozen(bool frozen);

		//virtual std::string getSoundFireScream();
		
		virtual void attack();

		virtual void endAttack();

		virtual void throwGrenade();

		virtual void speak(std::string *soundFile, bool shouldInterrupt = false);
		virtual void speak(std::string soundFile, bool shouldInterrupt = false);
		
		virtual std::string getQuoteFreeze();
		
		virtual std::string getAttackSound();
		virtual std::string getDeathSound();

		//virtual void quiet() { if(mpWeaponSound) mpWeaponSound->setIsPaused(true); if(mVoice) mVoice->setIsPaused(true); }
		//virtual void unQuiet() { if(mpWeaponSound) mpWeaponSound->setIsPaused(false); if(mVoice) mVoice->setIsPaused(false); }
};

#endif
