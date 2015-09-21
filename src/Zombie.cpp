#include "Zombie.h"

#include "Game.h"

#include "Map.h"

#include "Player.h"

#include "SoundEngine.h"

#include "AnimationManager.h"

#include "EntityManager.h"

#include "Projectile.h"

#include "ResourceManager.h"

#include "Utils.h"

#include "Gib.h"

#include "HealthPotion.h"
#include <glm/glm.hpp>

extern Game *game;

const float MONSTER_DAMPING = 0.5f;

Zombie::Zombie(Vec3d pos) : Monster(pos)
{
	mPosition = pos;
	playerPos = mPosition;
	mRotation = 0.0f;
	mIsStunned = false;
	mIsAlerted = false;
	mIsDying = false;

	mType = ENTITY_ENEMY;
	mMonsterType = MINOTAUR_MONSTER;
	
	mpAnim = New Animation(*game->getAnimationManager()->get("zombieWalk"));
}

void Zombie::think(const double elapsedTime)
{
	Monster::think(elapsedTime);

	if(rand() % 750 == 0)
		attack();
}

void Zombie::collide(Entity* other)
{
	if(other->getType() == ENTITY_ATTACK_PLAYER){
		//mShouldDelete = true;
	}
}

void Zombie::kill()
{
	Monster::kill();

	mHealth = 0;

	//ResourceManager *resources = game->getResourceManager();

	if(!mIsDying)
	{
		//if(mVoice)
			//mVoice->stop();

		makeMonsterNoise(true);

		Player *player = game->getPlayer();

		switch(rand() % 3)
		{
			case 0:
				player->speak(player->getQuipKillZombie());
				break;
			case 2:
				if(mAttachments.size() > 0)
					player->speak(player->getQuipBurn());
				break;
			case 1:
				player->speak(player->getQuipKill());
				break;
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
		//mDeathTimer.start();

		delete mpAnim;

		mpAnim = New Animation(*game->getAnimationManager()->get("vaporize"));

		//if(rand() % 5 == 0)
				game->getEntityManager()->add(New HealthPotion(mPosition));
	}
}

void Zombie::setFrozen(bool frozen)
{
	Monster::setFrozen(frozen);

	
}

void Zombie::spawnGib(float velocity)
{
	if(!game->getAllowGibs())
		return;

	Animation *arm = game->getAnimationManager()->get("zombie_arm");
	Animation *leg = game->getAnimationManager()->get("zombie_leg");
	Animation *torso = game->getAnimationManager()->get("zombie_torso");
	Animation *head = game->getAnimationManager()->get("zombie_head");
	
	for(int i = 0; i < 2; i++)
		game->getEntityManager()->add(New Gib(arm, mPosition, Vec3d(randomFloat(0.0f,0.075f),randomFloat(0.0f,0.075f),randomFloat(0.0f,0.075f)), mIsFrozen, GIB_ARM));
	for(int i = 0; i < 2; i++)
		game->getEntityManager()->add(New Gib(leg, mPosition, Vec3d(randomFloat(0.0f,0.075f),randomFloat(0.0f,0.075f),randomFloat(0.0f,0.075f)), mIsFrozen, GIB_LEG));
		
	game->getEntityManager()->add(New Gib(head, mPosition, Vec3d(randomFloat(0.0f,0.075f),randomFloat(0.0f,0.075f),randomFloat(0.0f,0.075f)), mIsFrozen, GIB_HEAD));

	game->getEntityManager()->add(New Gib(torso, mPosition, Vec3d(randomFloat(0.0f,0.075f),randomFloat(0.0f,0.075f),randomFloat(0.0f,0.075f)), mIsFrozen, GIB_TORSO));
}

void Zombie::makeMonsterNoise(bool priority)
{
	speak(game->getResourceManager()->get("zombie_die1"), priority);
}

/*void Zombie::speak(std::string *soundFile, bool shouldInterrupt)
{
	if(mVoice == NULL)
	{
		mVoice = game->getSoundEngine()->play3DSoundKeep(soundFile, mPosition, 0.5f, randomFloat(0.75f, 1.35f));
	}
	else if(mVoice->isFinished() || shouldInterrupt)
	{
		mVoice->stop();
		mVoice->drop();

		mVoice = game->getSoundEngine()->play3DSoundKeep(soundFile, mPosition, 0.5f, randomFloat(0.75f, 1.35f));
	}
}*/

void Zombie::removeAttachment(Entity *toBeRemoved)
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

void Zombie::removeAttachment(std::string key)
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

void Zombie::takeDamage(float hp)
{
	Monster::takeDamage(hp);
	//makeMonsterNoise();
}

void Zombie::attack()
{
	Vec3d playerPos = game->getPlayer()->getPosition() - mPosition;

	//Vec2d angle(atan2(playerPos.X-mPosition.X, playerPos.Z-mPosition.Z));

    glm::normalize(playerPos);

	playerPos /= 8;

	//range.setLength(1.0);

	game->getEntityManager()->add(New Projectile(mPosition,playerPos,game->getAnimationManager()->get("fireball"),Vec2d(5,10),ENTITY_ATTACK_ENEMY));

	speak(getAttackSound());
	//game->getSoundEngine()->play3DSound(game->getResourceManager()->get("flame_attack"),mPosition);
}

std::string Zombie::getAttackSound()
{
	return *game->getResourceManager()->get("monster_grunt1");
}
