#include "Monster.h"

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

extern Game *game;

const float MONSTER_DAMPING = 0.5f;

Monster::Monster(Vec3d pos) : Entity()
{
	//mVoice = NULL;

	mPosition = pos;
	playerPos = mPosition;
	mRotation = 0.0f;
	mIsStunned = false;
	mIsAlerted = false;
	mIsFleeing = false;
	mIsDying = false;

	mType = ENTITY_ENEMY;

	/*switch(rand() % 3)
	{
		case 0:
			mpAnim = New Animation(*game->getAnimationManager()->get("minotaurWalk"));
			mMonsterType = MINOTAUR_MONSTER;
			break;
		case 1:
			mpAnim = New Animation(*game->getAnimationManager()->get("swampmonsterWalk"));
			mMonsterType = SWAMP_MONSTER;
			break;
		case 2:
			mpAnim = New Animation(*game->getAnimationManager()->get("zombieWalk"));
			mMonsterType = ZOMBIE_MONSTER;
			break;
		default:
			break;
	}*/
}

Monster::~Monster()
{
	//if(mVoice)
	//{
		//mVoice->stop();
		//mVoice->drop();
		//mVoice = NULL;
	//}

	//Entity base class destructor will delete mpAnim for us.

	if(!game->isEnding() && !game->getCurrentMap()->getCleared())
	{
		EntityManager *e = game->getEntityManager();

		unsigned int monsterCount = 0;

		for(auto i = e->itemMap.begin(); i != e->itemMap.end(); ++i)
		{
			if(i->second->getType() == ENTITY_ENEMY)
				monsterCount++;
		}

		if(monsterCount < 2)
		{
			game->getCurrentMap()->setCleared(true);

			//Queue end level quip if not able to say immediately.
			game->getPlayer()->speak(game->getPlayer()->getQuipLevelCleared(), false, true);
		}
	}
}

void Monster::draw()
{
	Entity::draw();
}

void Monster::think(const double elapsedTime)
{
	Entity::think(elapsedTime);

	if(!mIsDying && mHealth <= 0)
		kill();

	if(mIsStunned)
	{
		if(mPosition.y > -0.5)
		{
			mVelocity.y -= 0.005;
			mPosition += mVelocity;

			if(game->getCurrentMap()->getTile(int(mPosition.x + mVelocity.z), int(mPosition.z)).type == TYPE_WALL)
			{
				mVelocity.x = -mVelocity.x;

				mVelocity *= MONSTER_DAMPING;

			}
			if(game->getCurrentMap()->getTile(int(mPosition.x), int(mPosition.z + mVelocity.z)).type == TYPE_WALL)
			{
				mVelocity.z = -mVelocity.z;

				mVelocity *= MONSTER_DAMPING;
			}

		}
		else
		{
			mIsStunned = false;
			mPosition.y = -0.5;
		}
	}

	if(mIsDying)
	{
		//if(mDeathTimer.getElapsedTime() >= 500)
			//mShouldDelete = true;

		return;
	}

	if(mIsFrozen)
	{
		//if(mFreezeTimer.getElapsedTime() < 5000)
		//{
			//return;
		//}
		//else
			//setFrozen(false);
	}

	if(mIsFleeing)
	{
		Vec2d motion = Vec2d(playerPos.x-mPosition.x,playerPos.z-mPosition.z);

		motion.normalize();

		motion /= 35;
	
		if((playerPos.x - mPosition.x) + (playerPos.z - mPosition.z) < 0.3)
		{
			int goalStr = game->getCurrentMap()->getTile(int(mPosition.x), int(mPosition.z)).goodCreatureGoal;
			playerPos = mPosition;
			for(int i = -1; i < 2; ++i)
				for(int j = -1; j < 2; ++j)
					if(game->getCurrentMap()->getTile(i + int(mPosition.x), j + int(mPosition.z)).type != TYPE_WALL && goalStr < game->getCurrentMap()->getTile(i + int(mPosition.x), j + int(mPosition.z)).goodCreatureGoal && game->getCurrentMap()->getTile(i + int(mPosition.x), j + int(mPosition.z)).goodCreatureGoal < 100)
					{
						goalStr = game->getCurrentMap()->getTile(i + int(mPosition.x), j + int(mPosition.z)).goodCreatureGoal;
						playerPos.x = mPosition.x + i;
						playerPos.z = mPosition.z + j;
					}
		}

		game->getCurrentMap()->setGoal(int(playerPos.x), int(playerPos.z), 0);
		game->getCurrentMap()->setGoal(int(mPosition.x), int(mPosition.z), 0);

		//Increase position of monster.
		mPosition -= Vec3d(motion.X(),0,motion.Y());
	}
	else
	{
		Vec2d motion = Vec2d(playerPos.x-mPosition.x,playerPos.z-mPosition.z);

		motion.normalize();

		motion /= 50;
	
		if((playerPos.x - mPosition.x) + (playerPos.z - mPosition.z) < 0.3)
		{
			int goalStr = game->getCurrentMap()->getTile(int(mPosition.x), int(mPosition.z)).goodCreatureGoal;
			playerPos = mPosition;
			for(int i = -1; i < 2; ++i)
				for(int j = -1; j < 2; ++j)
					if(game->getCurrentMap()->getTile(i + int(mPosition.x), j + int(mPosition.z)).type != TYPE_WALL && goalStr < game->getCurrentMap()->getTile(i + int(mPosition.x), j + int(mPosition.z)).goodCreatureGoal && game->getCurrentMap()->getTile(i + int(mPosition.x), j + int(mPosition.z)).goodCreatureGoal < 100)
					{
						goalStr = game->getCurrentMap()->getTile(i + int(mPosition.x), j + int(mPosition.z)).goodCreatureGoal;
						playerPos.x = mPosition.x + i;
						playerPos.z = mPosition.z + j;
					}
		}

		game->getCurrentMap()->setGoal(int(playerPos.x), int(playerPos.z), 0);
		game->getCurrentMap()->setGoal(int(mPosition.x), int(mPosition.z), 0);

		//Increase position of monster.
		mPosition += Vec3d(motion.X(),0,motion.Y());
	}
	

	/*if(rand() % 500 == 0)
	{
		attack();
	}

	if(rand() % 5000 == 0)
	{
		makeMonsterNoise();
	}*/

	//if(mVoice)
		//mVoice->setPosition(mPosition);
}

void Monster::collide(Entity* other)
{
	if(other->getType() == ENTITY_ATTACK_PLAYER){
		//mShouldDelete = true;
	}
}

void Monster::kill()
{
	mHealth = 0;
}

void Monster::shutUp()
{
	//if(mVoice)
	//{
		//mVoice->stop();
		//mVoice->drop();
		//mVoice = NULL;
	//}
}

void Monster::setFrozen(bool frozen)
{
	Entity::setFrozen(frozen);

	if(mIsFrozen)
	{
		//mFreezeTimer.start();

		if(mpAnim)
		{
			mpAnim->setPaused(true);
			mpAnim->setTint(Color(0.35f,0.35f,2.0f));
		}

		shutUp();

		//game->getSoundEngine()->play3DSound(*game->getResourceManager()->get("freeze" + std::to_string(rand() % 5 + 1)), mPosition);
	}
	else
	{
		//mFreezeTimer.stop();

		if(mpAnim)
		{
			mpAnim->setPaused(false);
			mpAnim->setTint(Color(1,1,1));
		}
	}
}

void Monster::spawnGib(float velocity)
{
	//game->getEntityManager()->add(New Gib(mPosition, Vec3d(randomFloat(0.0f,0.075f),randomFloat(0.0f,0.075f),0), mMonsterType, mIsFrozen));

}

void Monster::addAttachment(Entity *attachment)
{
	Entity::addAttachment(attachment);

	if(/*rand() % 3 == 0*/ mAttachments.size() >= 3)
	{
		if(rand() % 3 == 0)
			game->getPlayer()->speak(game->getPlayer()->getQuipSetFire());
		
		if(rand() % 2 == 0)
		{
			speak(getSoundFireScream());

			//mVoice->setIsLooped(true);

			/*if(rand() % 2)
				mIsFleeing = true;*/

			if(rand() % 4 == 0)
				game->getPlayer()->speak(game->getPlayer()->getQuipRespondScreams());
		}
	}

	//game->shout("Health: " + std::to_string(mHealth));
}

void Monster::stun(Vec3d velocity)
{
	//game->getSoundEngine()->play3DSound(*game->getResourceManager()->get("monster_grunt1"), mPosition);

	makeMonsterNoise();

	mIsStunned = true;
	
	mVelocity = velocity;

	mPosition = mPosition + mVelocity;
}

void Monster::makeMonsterNoise(bool priority)
{
	
}

void Monster::speak(std::string *soundFile, bool shouldInterrupt)
{
	//if(mVoice == NULL)
	//{
		//mVoice = game->getSoundEngine()->play3DSoundKeep(soundFile, mPosition, 0.5f, randomFloat(0.75f, 1.35f));
	//}
	//else if(mVoice->isFinished() || shouldInterrupt)
	//{
		//mVoice->stop();
		//mVoice->drop();

		//mVoice = game->getSoundEngine()->play3DSoundKeep(soundFile, mPosition, 0.5f, randomFloat(0.75f, 1.35f));
	//}
}

void Monster::speak(std::string soundFile, bool shouldInterrupt)
{
	//if(isSpeaking() == NULL)
	//{
		//mVoice = game->getSoundEngine()->play3DSoundKeep(soundFile, mPosition, 1.0f, randomFloat(0.75f, 2.0f));
	//}
	//else if(mVoice->isFinished() || shouldInterrupt)
	//{
		//mVoice->stop();
		//mVoice->drop();

		//mVoice = game->getSoundEngine()->play3DSoundKeep(soundFile, mPosition, 1.0f, randomFloat(0.75f, 2.0f));
	//}
}

//Get the filename of a selfie quote.
std::string Monster::getSoundFireScream()
{
	static std::vector<std::string*> toBeSaid;

	ResourceManager *manager = game->getResourceManager();

	if(toBeSaid.size() < 1)
	{
		unsigned i = 1;

		std::string *path;

		path = manager->get("monster_scream" + std::to_string(i));

		while(path)
		{
			toBeSaid.push_back(path);
			i++;
			path = manager->get("monster_scream" + std::to_string(i));
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

void Monster::removeAttachment(Entity *toBeRemoved)
{
	Entity::removeAttachment(toBeRemoved);

	if(mAttachments.empty())
		mIsFleeing = false;

	//Moved to children classes.
	/*if(mAttachments.size() < 1)
	{
		if(mVoice)
		{
			mVoice->stop();
			mVoice->drop();
			mVoice = NULL;
		}
	}*/
}

void Monster::removeAttachment(std::string key)
{
	Entity::removeAttachment(key);

	if(mAttachments.empty())
		mIsFleeing = false;

	/*if(mAttachments.size() < 1)
	{
		if(mVoice)
		{
			mVoice->stop();
			mVoice->drop();
			mVoice = NULL;
		}
	}*/
}

void Monster::takeDamage(float hp)
{
	Entity::takeDamage(hp);

	if(mHealth <= 0)
		mIsDying = true;
	//makeMonsterNoise();
}

void Monster::attack()
{
	/*Vec3d playerPos = game->getPlayer()->getPosition() - mPosition;

	//Vec2d angle(atan2(playerPos.X-mPosition.X, playerPos.Z-mPosition.Z));

	playerPos.normalize();

	playerPos /= 8;

	//range.setLength(1.0);

	game->getEntityManager()->add(New Projectile(mPosition,playerPos,game->getAnimationManager()->get("fireball"),Vec2d(5,10),ENTITY_ATTACK_ENEMY));

	speak(game->getResourceManager()->get(getAttackSound()));
	game->getSoundEngine()->play3DSound(game->getResourceManager()->get("flame_attack"),mPosition);*/
}
