#include "Nazi.h"

#include "Game.h"

#include "Map.h"

#include "Player.h"

#include "SoundEngine.h"

#include "AnimationManager.h"

#include "EntityManager.h"

#include "NaziBullet.h"

#include "ResourceManager.h"

#include "Utils.h"

#include "Gib.h"

#include "HealthPotion.h"

#include "Grenade.h"

extern Game *game;

const float MONSTER_DAMPING = 0.5f;

Nazi::Nazi(Vec3d pos) : Monster(pos)
{
	mPosition = pos;
	playerPos = mPosition;
	mRotation = 0.0f;
	mIsStunned = false;
	mIsAlerted = false;
	mIsDying = false;

	mIsOfficer = (rand() % 5 == 0);

	mType = ENTITY_ENEMY;
	mMonsterType = NAZI_MONSTER;
	
	if(mIsOfficer)
		mpAnim = New Animation(*game->getAnimationManager()->get("offWalk"));
	else
		mpAnim = New Animation(*game->getAnimationManager()->get("guardWalk"));
	
	mFireTime = 0.0;
	mpWeaponSound = NULL;
	
	fireballAnim = game->getAnimationManager()->get("shot");
}

void Nazi::think(const double elapsedTime)
{	
	/*if(mIsStunned)
	{
		if(!mpWeaponSound)
		{

		}
		mpWeaponSound()
	}*/
	if(mHealth <= 0)
		kill();

	if(mpWeaponSound)
	{
		Entity::think(elapsedTime);
		mpWeaponSound->setPosition(mPosition);
		
		if(mShotTimer.getElapsedTime() > 106)
		{
			fireWeapon();
			mShotTimer.start();
		}
		
		if(mWeaponTimer.getElapsedTime() > mFireTime)
		{
			endAttack();
			//mpAnim->setPaused(false);
		}
	}
	else
	{	
		Monster::think(elapsedTime);
		
		if(!mIsDying && !mIsFrozen && !mIsStunned)
		{
			if(rand() % 250 == 0)
			{
				attack();
			}
		}
	}
	
	if(rand() % 2000 == 0)
	{
			makeMonsterNoise();
	}
}

Nazi::~Nazi()
{
	/*if(oldAnim)
	{
		delete oldAnim;
	}*/
}

void Nazi::collide(Entity* other)
{
	if(other->getType() == ENTITY_ATTACK_PLAYER){
		//mShouldDelete = true;
	}
}

void Nazi::stun(Vec3d startPos)
{
	endAttack();

	speak(getDeathSound(), true);

	Monster::stun(startPos);
}

void Nazi::kill()
{
	Monster::kill();

	endAttack();
	
	//mHealth = 0;

	ResourceManager *resources = game->getResourceManager();

	if(!mIsDying)
	{
		//if(mVoice)
			//mVoice->stop();

		speak(getDeathSound(), true);

		if(rand() % 2 == 0)
		{
			Player *player = game->getPlayer();

			if(rand() % 2 == 0)
				player->speak(player->getQuipKillNazi());
			else if(rand() % 2 == 0)
			{
				if(mAttachments.size() > 0)
					player->speak(player->getQuipBurn());
			}
			else
				player->speak(player->getQuipKill());
		}

		if(mIsFrozen)
			//Made this a bit quieter since it was pretty overpowering.
			game->getSoundEngine()->play2DSound(game->getResourceManager()->get("shatter" + std::to_string(rand() % 10 + 1)), 0.25f);

		//Remove all attachments left.
		for(auto it = mAttachments.begin(); it != mAttachments.end();  ++it)
		{
			it->second->setShouldDelete(true);
		}

		mAttachments.clear();

		mIsDying = true;
		mDeathTimer.start();

		delete mpAnim;

		mpAnim = New Animation(*game->getAnimationManager()->get("vaporize"));

		if(rand() % 5 == 0)
				game->getEntityManager()->add(New HealthPotion(mPosition));
	}
}

void Nazi::setFrozen(bool frozen)
{
	Monster::setFrozen(frozen);

	if(mIsFrozen)
	{
		speak(getQuoteFreeze());
		
		endAttack();
	}
}

//Get the filename of a nazi freeze quote.
std::string Nazi::getQuoteFreeze()
{
	static std::vector<std::string*> toBeSaid;

	ResourceManager *manager = game->getResourceManager();

	if(toBeSaid.size() < 1)
	{
		unsigned i = 1;

		std::string *path;

		path = manager->get("nazi_freeze" + std::to_string(i));

		while(path)
		{
			toBeSaid.push_back(path);
			i++;
			path = manager->get("nazi_freeze" + std::to_string(i));
		}
	}

	unsigned index = rand() % toBeSaid.size();

	//Get a pointer to a string to return.
	std::string *out = toBeSaid[index];

	//Remove the quip we just said.
	toBeSaid.erase(toBeSaid.begin() + index);

	//Return the string path to a quip.
	return *out;
}

void Nazi::spawnGib(float velocity)
{
	if(!game->getAllowGibs())
		return;

	//if(mHealth <= 0)
		//return;

	Animation *arm, *leg, *torso, *head;

	if(mIsOfficer)
	{
		arm = game->getAnimationManager()->get("off_arm");
		leg = game->getAnimationManager()->get("off_leg");
		torso = game->getAnimationManager()->get("off_torso");
		head = game->getAnimationManager()->get("off_head");
	}
	else
	{
		arm = game->getAnimationManager()->get("guard_arm");
		leg = game->getAnimationManager()->get("guard_leg");
		torso = game->getAnimationManager()->get("guard_torso");
		head = game->getAnimationManager()->get("guard_head");
	}
	
	for(int i = 0; i < 2; i++)
		game->getEntityManager()->add(New Gib(arm, mPosition, Vec3d(randomFloat(0.0f,0.075f),randomFloat(0.0f,0.075f),randomFloat(0.0f,0.075f)), mIsFrozen, GIB_ARM));
	for(int i = 0; i < 2; i++)
		game->getEntityManager()->add(New Gib(leg, mPosition, Vec3d(randomFloat(0.0f,0.075f),randomFloat(0.0f,0.075f),randomFloat(0.0f,0.075f)), mIsFrozen, GIB_LEG));
		
	game->getEntityManager()->add(New Gib(head, mPosition, Vec3d(randomFloat(0.0f,0.075f),randomFloat(0.0f,0.075f),randomFloat(0.0f,0.075f)), mIsFrozen, GIB_HEAD));
	game->getEntityManager()->add(New Gib(torso, mPosition, Vec3d(randomFloat(0.0f,0.075f),randomFloat(0.0f,0.075f),randomFloat(0.0f,0.075f)), mIsFrozen, GIB_TORSO));
	
}

void Nazi::makeMonsterNoise(bool priority)
{
	static std::vector<std::string*> toBeSaid;

	ResourceManager *manager = game->getResourceManager();

	if(toBeSaid.size() < 1)
	{
		unsigned i = 1;

		std::string *path;

		path = manager->get("nazi_chat" + std::to_string(i));

		while(path)
		{
			toBeSaid.push_back(path);
			i++;
			path = manager->get("nazi_chat" + std::to_string(i));
		}
	}

	unsigned index = rand() % toBeSaid.size();

	//Get a pointer to a string to return.
	std::string *out = toBeSaid[index];

	//Remove the quip we just said.
	toBeSaid.erase(toBeSaid.begin() + index);

	//Return the string path to a quip.
	//return *out;
	speak(out, priority);
}

void Nazi::speak(std::string *soundFile, bool shouldInterrupt)
{
	if(mVoice == NULL)
	{
		mVoice = game->getSoundEngine()->play3DSoundKeep(soundFile, mPosition, 1.0f, randomFloat(0.95f, 1.05f));
	}
	else if(mVoice->isFinished() || shouldInterrupt)
	{
		mVoice->stop();
		mVoice->drop();

		mVoice = game->getSoundEngine()->play3DSoundKeep(soundFile, mPosition,  1.0f, randomFloat(0.95f, 1.05f));
		
		mVoice->setMinDistance(1.5);
	}
}

void Nazi::speak(std::string &soundFile, bool shouldInterrupt)
{
	if(mVoice == NULL)
	{
		mVoice = game->getSoundEngine()->play3DSoundKeep(soundFile, mPosition, 1.0f, randomFloat(0.95f, 1.05f));
	}
	else if(mVoice->isFinished() || shouldInterrupt)
	{
		mVoice->stop();
		mVoice->drop();

		mVoice = game->getSoundEngine()->play3DSoundKeep(soundFile, mPosition, 1.0f, randomFloat(0.95f, 1.05f));
		
		mVoice->setMinDistance(1.5);
	}
}

void Nazi::removeAttachment(Entity *toBeRemoved)
{
	Monster::removeAttachment(toBeRemoved);

	if(mAttachments.size() < 1)
	{
		if(mVoice)
		{
			mVoice->stop();
			mVoice->drop();
			mVoice = NULL;
		}
	}
}

void Nazi::removeAttachment(std::string key)
{
	Monster::removeAttachment(key);

	if(mAttachments.size() < 1)
	{
		if(mVoice)
		{
			mVoice->stop();
			mVoice->drop();
			mVoice = NULL;
		}
	}
}

void Nazi::takeDamage(float hp)
{
	Monster::takeDamage(hp);
	//makeMonsterNoise();
}

void Nazi::attack()
{
	if(!mpWeaponSound)
	{

		if(rand() % 5 == 0 && mPosition.getDistanceFrom(game->getPlayer()->getPosition()) >= 5)
		{
			throwGrenade();
		}
		//range.setLength(1.0);

		fireWeapon();

		speak(&getAttackSound());
		//game->getSoundEngine()->play3DSound(game->getResourceManager()->get("flame_attack"),mPosition);
	
		Sound *tmp = game->getSoundEngine()->play3DSoundKeep(game->getResourceManager()->get("mp40_beginfire"), mPosition, 1.0f, randomFloat(0.75f, 1.35f));

		tmp->setMinDistance(6);

		mpWeaponSound = game->getSoundEngine()->play3DSoundKeep(game->getResourceManager()->get("mp40_fireloop"), mPosition, 1.0f);
	
		mpWeaponSound->setMinDistance(6);

		mpWeaponSound->setIsLooped(true);
	
		mFireTime = randomFloat(10.f, 800.f);

		mWeaponTimer.start();
		mShotTimer.start();
		//mpAnim->setPaused(true);

		//oldAnim = mpAnim;

		delete mpAnim;

		if(mIsOfficer)
			mpAnim = New Animation(*game->getAnimationManager()->get("offShoot"));
		else
			mpAnim = New Animation(*game->getAnimationManager()->get("guardShoot"));
	}
}

std::string Nazi::getAttackSound()
{
	static std::vector<std::string*> toBeSaid;

	ResourceManager *manager = game->getResourceManager();

	if(toBeSaid.size() < 1)
	{
		unsigned i = 1;

		std::string *path;

		path = manager->get("nazi_attack" + std::to_string(i));

		while(path)
		{
			toBeSaid.push_back(path);
			i++;
			path = manager->get("nazi_attack" + std::to_string(i));
		}
	}

	unsigned index = rand() % toBeSaid.size();

	//Get a pointer to a string to return.
	std::string *out = toBeSaid[index];

	//Remove the quip we just said.
	toBeSaid.erase(toBeSaid.begin() + index);

	//Return the string path to a quip.
	return *out;
}

std::string Nazi::getDeathSound()
{
	static std::vector<std::string*> toBeSaid;

	ResourceManager *manager = game->getResourceManager();

	if(toBeSaid.size() < 1)
	{
		unsigned i = 1;

		std::string *path;

		path = manager->get("nazi_die" + std::to_string(i));

		while(path)
		{
			toBeSaid.push_back(path);
			i++;
			path = manager->get("nazi_die" + std::to_string(i));
		}
	}

	unsigned index = rand() % toBeSaid.size();

	//Get a pointer to a string to return.
	std::string *out = toBeSaid[index];

	//Remove the quip we just said.
	toBeSaid.erase(toBeSaid.begin() + index);

	//Return the string path to a quip.
	return *out;
}

void Nazi::fireWeapon()
{

	Vec3d playerPos = game->getPlayer()->getPosition() - mPosition;

		//Vec2d angle(atan2(playerPos.X-mPosition.X, playerPos.Z-mPosition.Z));

	playerPos.setLength(0.15);

	//playerPos /= 8;

	NaziBullet *b = New NaziBullet(mPosition,playerPos,fireballAnim);
	game->getEntityManager()->add(b);
}

void Nazi::throwGrenade()
{
	Vec3d playerPos = game->getPlayer()->getPosition() - mPosition;

	playerPos.Y *= 1.1;

		//Vec2d angle(atan2(playerPos.X-mPosition.X, playerPos.Z-mPosition.Z));

	playerPos.setLength(0.2);

	//playerPos /= 8;

	//Grenade *b = New Grnade(mPosition,playerPos,fireballAnim);
	game->getEntityManager()->add(New Grenade(mPosition, playerPos, game->getAnimationManager()->get("stick_grenade"), Vec2d(5,10), ENTITY_ATTACK_ENEMY, 30.0));
}

void Nazi::endAttack()
{
	if(mpWeaponSound)
	{
  		mpWeaponSound->stop();
		mpWeaponSound->drop();
		mpWeaponSound = NULL;
				
		mWeaponTimer.stop();
		mShotTimer.stop();
				
		Sound *tmp =game->getSoundEngine()->play3DSoundKeep(game->getResourceManager()->get("mp40_tail"), mPosition,  1.0f, randomFloat(0.95f, 1.05f));

		tmp->setMinDistance(15);
				
		delete mpAnim;

		if(mIsOfficer)
			mpAnim = New Animation(*game->getAnimationManager()->get("offWalk"));
		else
			mpAnim = New Animation(*game->getAnimationManager()->get("guardWalk"));
	}
}