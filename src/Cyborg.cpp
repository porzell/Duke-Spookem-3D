#include "Cyborg.h"

#include "Game.h"

#include "Map.h"

#include "Player.h"

#include "SoundEngine.h"

#include "AnimationManager.h"

#include "EntityManager.h"

#include "CyborgBullet.h"

#include "ResourceManager.h"

#include "Utils.h"

#include "Gib.h"

#include "HealthPotion.h"

#include "Grenade.h"
#include <glm/glm.hpp>

extern Game *game;

const float MONSTER_DAMPING = 0.5f;

Cyborg::Cyborg(Vec3d pos) : Monster(pos)
{
	mPosition = pos;
	playerPos = mPosition;
	mRotation = 0.0f;
	mIsStunned = false;
	mIsAlerted = false;
	mIsDying = false;

	//mIsMech = (rand() % 5 == 0);
	mIsMech = false;

	mType = ENTITY_ENEMY;
	mMonsterType = CYBORG_MONSTER;
	
	if(mIsMech)
		mpAnim = New Animation(*game->getAnimationManager()->get("cyborg2Walk"));
	else
		mpAnim = New Animation(*game->getAnimationManager()->get("cyborg1Walk"));
	
	//mpWeaponSound = NULL;
	
	mShotColor.r = randomFloat(0.25,1);
	mShotColor.g = randomFloat(0.25,1);
	mShotColor.b = randomFloat(0.25,1);
}

void Cyborg::think(const double elapsedTime)
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

	//if(mpWeaponSound)
	//{
		//Entity::think(elapsedTime);
		//mpWeaponSound->setPosition(mPosition);
		
		//if(mpWeaponSound->isFinished())
		//{
			//endAttack();
		//}
	//}
	//else
    {	
        Monster::think(elapsedTime);
        
        if(!mIsFrozen && !mIsStunned)
        {
            if(rand() % 450 == 0)
            {
                attack();
            }
        }
    }
	
	if(rand() % 500 == 0)
	{
			makeMonsterNoise();
	}
}

Cyborg::~Cyborg()
{
	/*if(oldAnim)
	{
		delete oldAnim;
	}*/
}

void Cyborg::collide(Entity* other)
{
	if(other->getType() == ENTITY_ATTACK_PLAYER){
		//mShouldDelete = true;
	}
}

void Cyborg::stun(Vec3d startPos)
{
	endAttack();

	//speak(getDeathSound(), true);

	Monster::stun(startPos);
}

void Cyborg::kill()
{
	Monster::kill();

	endAttack();
	
	mHealth = 0;

	//ResourceManager *resources = game->getResourceManager();

	if(!mIsDying)
	{
		//if(mVoice)
			//mVoice->stop();

		//speak(getDeathSound(), true);

		if(rand() % 2 == 0)
		{
			Player *player = game->getPlayer();

			if(rand() % 2 == 0)
				player->speak(player->getQuipKillCyborg());
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

void Cyborg::setFrozen(bool frozen)
{
	Monster::setFrozen(frozen);

	if(mIsFrozen)
	{
        speak(getQuoteFreeze());
		
		endAttack();
	}
}

//Get the filename of a Cyborg freeze quote.
std::string Cyborg::getQuoteFreeze()
{
	static std::vector<std::string*> toBeSaid;

	ResourceManager *manager = game->getResourceManager();

	if(toBeSaid.size() < 1)
	{
		unsigned i = 1;

		std::string *path;

		path = manager->get("cyborg_freeze" + std::to_string(i));

		while(path)
		{
			toBeSaid.push_back(path);
			i++;
			path = manager->get("cyborg_freeze" + std::to_string(i));
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

void Cyborg::spawnGib(float velocity)
{
	if(!game->getAllowGibs())
		return;

	//if(mHealth <= 0)
		//return;

	Animation *gib, *head, *leg, *arm, *torso;

	if(mIsMech)
	{
		arm = game->getAnimationManager()->get("cyborg2_arm");
		leg = game->getAnimationManager()->get("cyborg2_leg");
		torso = game->getAnimationManager()->get("cyborg2_torso");
		head = game->getAnimationManager()->get("cyborg2_head");
	}
	else
	{
		arm = game->getAnimationManager()->get("cyborg1_arm");
		leg = game->getAnimationManager()->get("cyborg1_leg");
		torso = game->getAnimationManager()->get("cyborg1_torso");
		head = game->getAnimationManager()->get("cyborg1_head");
	}

	gib = game->getAnimationManager()->get("circuit_board");
	
	for(int i = 0; i < rand() % 5; i++)
		game->getEntityManager()->add(New Gib(gib, mPosition, Vec3d(randomFloat(0.0f,0.075f),randomFloat(0.0f,0.075f),randomFloat(0.0f,0.075f)), mIsFrozen, GIB_CIRCUIT));

	for(int i = 0; i < 2; i++)
		game->getEntityManager()->add(New Gib(arm, mPosition, Vec3d(randomFloat(0.0f,0.075f),randomFloat(0.0f,0.075f),randomFloat(0.0f,0.075f)), mIsFrozen, GIB_ARM));
	for(int i = 0; i < 2; i++)
		game->getEntityManager()->add(New Gib(leg, mPosition, Vec3d(randomFloat(0.0f,0.075f),randomFloat(0.0f,0.075f),randomFloat(0.0f,0.075f)), mIsFrozen, GIB_LEG));
		
	game->getEntityManager()->add(New Gib(head, mPosition, Vec3d(randomFloat(0.0f,0.075f),randomFloat(0.0f,0.075f),randomFloat(0.0f,0.075f)), mIsFrozen, GIB_HEAD));
	game->getEntityManager()->add(New Gib(torso, mPosition, Vec3d(randomFloat(0.0f,0.075f),randomFloat(0.0f,0.075f),randomFloat(0.0f,0.075f)), mIsFrozen, GIB_TORSO));
}

void Cyborg::makeMonsterNoise(bool priority)
{
	static std::vector<std::string*> toBeSaid;

	ResourceManager *manager = game->getResourceManager();

	if(toBeSaid.size() < 1)
	{
		unsigned i = 1;

		std::string *path;

		path = manager->get("cyborg_chat" + std::to_string(i));

		while(path)
		{
			toBeSaid.push_back(path);
			i++;
			path = manager->get("cyborg_chat" + std::to_string(i));
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

void Cyborg::speak(std::string *soundFile, bool shouldInterrupt)
{
	//if(mVoice == NULL)
	//{
		//mVoice = game->getSoundEngine()->play3DSoundKeep(soundFile, mPosition, 1.0f, randomFloat(0.95f, 1.05f));
	//}
	//else if(mVoice->isFinished() || shouldInterrupt)
	//{
		//mVoice->stop();
		//mVoice->drop();

		//mVoice = game->getSoundEngine()->play3DSoundKeep(soundFile, mPosition,  1.0f, randomFloat(0.95f, 1.05f));
		
		//mVoice->setMinDistance(3);
	//}
}

void Cyborg::speak(std::string soundFile, bool shouldInterrupt)
{
	//if(mVoice == NULL)
	//{
		//mVoice = game->getSoundEngine()->play3DSoundKeep(soundFile, mPosition, 1.0f, randomFloat(0.95f, 1.05f));
	//}
	//else if(mVoice->isFinished() || shouldInterrupt)
	//{
		//mVoice->stop();
		//mVoice->drop();

		//mVoice = game->getSoundEngine()->play3DSoundKeep(soundFile, mPosition, 1.0f, randomFloat(0.95f, 1.05f));
		
		//mVoice->setMinDistance(1.5);
	//}
}

void Cyborg::removeAttachment(Entity *toBeRemoved)
{
	Monster::removeAttachment(toBeRemoved);

	if(mAttachments.size() < 1)
	{
		//if(mVoice)
		//{
			//mVoice->stop();
			//mVoice->drop();
			//mVoice = NULL;
		//}
	}
}

void Cyborg::removeAttachment(std::string key)
{
	Monster::removeAttachment(key);

	if(mAttachments.size() < 1)
	{
		//if(mVoice)
		//{
			//mVoice->stop();
			//mVoice->drop();
			//mVoice = NULL;
		//}
	}
}

void Cyborg::takeDamage(float hp)
{
	Monster::takeDamage(hp);
	//makeMonsterNoise();
}

void Cyborg::attack()
{
	//if(!mpWeaponSound)
	//{

		//[>if(rand() % 5 == 0 && mPosition.getDistanceFrom(game->getPlayer()->getPosition()) >= 5)
		//{
			//throwGrenade();
		//}*/
		////range.setLength(1.0);

		//fireWeapon();

		//if(rand() % 3 == 0)
			//speak(&getAttackSound());
		////game->getSoundEngine()->play3DSound(game->getResourceManager()->get("flame_attack"),mPosition);

		//mpWeaponSound = game->getSoundEngine()->play3DSoundKeep(game->getResourceManager()->get("plasma_fire1"), mPosition, 1.0f);
	
		//mpWeaponSound->setMinDistance(6);

		//delete mpAnim;

		//if(mIsMech)
			//mpAnim = New Animation(*game->getAnimationManager()->get("cyborg2Shoot"));
		//else
			//mpAnim = New Animation(*game->getAnimationManager()->get("cyborg1Shoot"));
	//}
}

std::string Cyborg::getAttackSound()
{
	static std::vector<std::string*> toBeSaid;

	ResourceManager *manager = game->getResourceManager();

	if(toBeSaid.size() < 1)
	{
		unsigned i = 1;

		std::string *path;

		path = manager->get("cyborg_attack" + std::to_string(i));

		while(path)
		{
			toBeSaid.push_back(path);
			i++;
			path = manager->get("cyborg_attack" + std::to_string(i));
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

std::string Cyborg::getDeathSound()
{
	static std::vector<std::string*> toBeSaid;

	ResourceManager *manager = game->getResourceManager();

	if(toBeSaid.size() < 1)
	{
		unsigned i = 1;

		std::string *path;

		path = manager->get("cyborg_die" + std::to_string(i));

		while(path)
		{
			toBeSaid.push_back(path);
			i++;
			path = manager->get("cyborg_die" + std::to_string(i));
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

void Cyborg::fireWeapon()
{

	Vec3d playerPos = game->getPlayer()->getPosition() - mPosition;

		//Vec2d angle(atan2(playerPos.X-mPosition.X, playerPos.Z-mPosition.Z));

	playerPos = glm::normalize(playerPos) * 0.15f;

	//playerPos /= 8;

	CyborgBullet *b = New CyborgBullet(mPosition,playerPos, mShotColor);
	game->getEntityManager()->add(b);
}

void Cyborg::throwGrenade()
{
	Vec3d playerPos = game->getPlayer()->getPosition() - mPosition;

	playerPos.y *= 1.1;

		//Vec2d angle(atan2(playerPos.X-mPosition.X, playerPos.Z-mPosition.Z));

	playerPos = glm::normalize(playerPos) * 0.2f;

	//playerPos /= 8;

	//Grenade *b = New Grnade(mPosition,playerPos,fireballAnim);
	game->getEntityManager()->add(New Grenade(mPosition, playerPos, game->getAnimationManager()->get("stick_grenade"), Vec2d(5,10), ENTITY_ATTACK_ENEMY, 30.0));
}

void Cyborg::endAttack()
{
	//if(mpWeaponSound)
	//{
          //mpWeaponSound->stop();
		//mpWeaponSound->drop();
		//mpWeaponSound = NULL;
				
		//delete mpAnim;

		//if(mIsMech)
			//mpAnim = New Animation(*game->getAnimationManager()->get("cyborg2Walk"));
		//else
			//mpAnim = New Animation(*game->getAnimationManager()->get("cyborg1Walk"));
	//}
}

std::string Cyborg::getSoundFireScream()
{
	static std::vector<std::string*> toBeSaid;

	ResourceManager *manager = game->getResourceManager();

	if(toBeSaid.size() < 1)
	{
		unsigned i = 1;

		std::string *path;

		path = manager->get("cyborg_onfire" + std::to_string(i));

		while(path)
		{
			toBeSaid.push_back(path);
			i++;
			path = manager->get("cyborg_onfire" + std::to_string(i));
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
