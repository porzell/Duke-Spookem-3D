#pragma once
#ifndef _PLAYER_H
#define _PLAYER_H

#include "GLincludes.h"

#include "Trackable.h"

#include "Game.h"

#include <queue>

//#include <irrKlang.h>

//using namespace irrklang;

//#include "EventSystem.h"
//#include "EventListener.h"

#include "Header.h"

#include "VectorMath.h"

#include "Entity.h"

#include "EventListener.h"

#include "Timer.h"

#include "Piss.h"

//typedef irrklang::ISound Sound;

extern Game *game;

const GLfloat PLAYER_RUN_SPEED = 0.05f;
const GLfloat PLAYER_WALK_SPEED = 0.025f;

//These are in degrees.
const GLfloat MAX_UPWARD_VIEWANGLE = 30.0f;
const GLfloat MIN_DOWNWARD_VIEWANGLE = -30.0f;


class Player : public Entity, public EventListener
{

	static float msHeight;

	private:

		float mVoiceVolume;

		Vec3d mVelocity;
		Vec3d mAngle;

		bool mForwardPressed;
		bool mBackwardPressed;
		bool mLeftPressed;
		bool mRightPressed;

		bool mWalkPressed;

		Timer mSnapchatTimer;

		bool mIsSnapchatting;
		bool mSnapchatted;
		bool mIsAttacking;
		bool mIsKicking;
		bool mIsPeeing;
		bool mIsFlamethrowing;

		//Sound *mpSpeaking;
		//Sound *mpPeeing;

		//FPS HUD stuff.
		Animation *mpHandAnim;
		Animation *mpLeftHandAnim;
		Animation *mpLegAnim;

		Vec2d mHandPosition;
		Vec2d mLeftHandPosition;
		Vec2d mLegPosition;

		//Called by input class.
		void handleKey(bool keyDown, int keycode);

		void handleMouse(const Vec2d &delta);

		float mp;

		virtual void collide(Entity* other);
		std::vector<std::pair<int, int>> getSections();

		//Queue for queued quips.
		std::queue<std::string> mQueuedQuips;

		float mViewBob;

		std::string mLastPiss;

	public:

		Player();
		~Player();

		void think();

		void draw();
		void drawHUD();

		//Play a player voice file.
		void speak(std::string soundFile, bool shouldInterrupt = false, bool shouldAddToQueue = false);

		inline bool isPeeing() { return mIsPeeing; }

		inline Vec3d getPosition() { return mPosition; };
		inline void setPosition(Vec3d &pos) { mPosition = pos; };

		inline Vec3d getVelocity() { return mVelocity; };
		inline void setVelocity(Vec3d &vel) { mVelocity = vel; };

		inline Vec3d getAngle() { return mAngle; };
		inline void setAngle(Vec3d &angle) { mAngle = angle; };

		void attack(int button);

		inline bool getSneaking() {return mWalkPressed || (!mForwardPressed && !mBackwardPressed && !mLeftPressed && !mRightPressed);}

		virtual void handleEvent( const Event& theEvent );

		//Heals the player by a given HP amount. (Also makes Spookum make heal quip.)
		void heal(float hpAmount);

		//Damages the player by a given HP amount. (Also makes Spookum make damage quips.)
		void takeHit(float hpAmount);

		std::string getQuipFreeze();
		std::string getQuipBurn();
		std::string getQuipKill();
		std::string getQuipKillCyborg();
		std::string getQuipKillMinotaur();
		std::string getQuipKillNazi();
		std::string getQuipKillSwampMonster();
		std::string getQuipKillZombie();
		std::string getQuipKick();
		std::string getQuipDecap();
		std::string getQuipLeaveBehind();
		std::string getQuipReturnLevel();
		std::string getQuipLevelCleared();
		std::string getQuipLevelAlreadyCleared();
		std::string getQuipPain();
		std::string getQuipPain25();
		std::string getQuipPain50();
		std::string getQuipExplosion();
		std::string getQuipNewLevel();
		std::string getQuipShatter();
		std::string getQuipHeal();
		std::string getQuipSelfie();
		std::string getQuipSetFire();
		std::string getQuipRespondScreams();

		std::string getQuipEnterNazi();
		std::string getQuipEnterTech();

		std::string getQuipHeadGolf();

		std::string getQuipUnzip();

		std::string getQuipEncounterHitler();
		std::string getQuipTauntHitler();
		std::string getQuipKillHitler();
		std::string getQuipThaw();

		inline float getVoiceVolume() { return mVoiceVolume; }
		inline void setVoiceVolume(float volume) { mVoiceVolume = volume; if(mpSpeaking) mpSpeaking->setVolume(mVoiceVolume); }

		inline bool isSpeaking() { return mpSpeaking; };

		friend class InputManager;

		void writeSave(FILE* outfile, int pos);
		void readSave(FILE* infile, int pos);
}; 

#endif
