#pragma once
#ifndef _MONSTER_H
#define _MONSTER_H

#include "Entity.h"

#include "Timer.h"

#include "SoundEngine.h"

enum MONSTER_TYPE
{
	NO_MONSTER, MINOTAUR_MONSTER, SWAMP_MONSTER, ZOMBIE_MONSTER, NAZI_MONSTER, HITLER_BOSS, CYBORG_MONSTER, MAX_MONSTER
};

class Monster : public Entity
{
	//static Timer shotTimer;

	protected:
		bool mIsStunned;
		bool mIsAlerted;
		bool mIsFleeing;
		bool mIsDying;
		Timer mDeathTimer;
		float mRotation;
		Vec3d playerPos;

		Timer mFreezeTimer;

		MONSTER_TYPE mMonsterType;

		Vec3d mVelocity;

		Sound *mVoice;

	public:
		Monster(Vec3d pos);
		virtual ~Monster();
		
		//Check if the monster is dying.
		inline bool isDying() { return mIsDying; }

		virtual void collide(Entity* other);

		virtual void think(const double elapsedTime);
		virtual void draw();

		virtual void kill();

		virtual void setFrozen(bool frozen);

		virtual void spawnGib(float velocity);

		virtual void addAttachment(Entity *attachment);

		virtual void shutUp();

		virtual void stun(Vec3d velocity);

		virtual void makeMonsterNoise(bool priority = false);

		virtual bool isSpeaking() { if(mVoice) return !mVoice->isFinished(); else return false; }

		virtual void speak(std::string *soundFile, bool shouldInterrupt = false);
		virtual void speak(std::string soundFile, bool shouldInterrupt = false);

		virtual void removeAttachment(Entity *toBeRemoved);
		virtual void removeAttachment(std::string index);

		virtual void takeDamage(float hp = 0);

		virtual std::string getSoundFireScream();
		virtual std::string getAttackSound() { return "sound/minotaur/minotaur.ogg"; }
		
		virtual void attack();

		inline MONSTER_TYPE Monster::getMonsterType() { return mMonsterType; }
};

#endif