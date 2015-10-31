#include "Hitler.h"

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

#include "ShellCasing.h"

#include "HealthPotion.h"

#include "Grenade.h"
#include <glm/glm.hpp>

extern Game *game;

const float MONSTER_DAMPING = 0.5f;

Animation* Hitler::fireballAnim = NULL;
Animation* Hitler::shellCasingAnim = NULL;

Hitler::Hitler(Vec3d pos) : Monster(pos)
{
	mHealth = 2000;

	mPosition = pos;
	playerPos = mPosition;
	mRotation = 0.0f;
	mIsStunned = false;
	mIsAlerted = false;
	mIsDying = false;

	mType = ENTITY_ENEMY;
	mMonsterType = HITLER_BOSS;
	
	mpAnim = New Animation(*game->getAnimationManager()->get("hitler_walk"));
	
	mFireTime = 0.0;
	//mpWeaponSound = NULL;
	
	if(!fireballAnim)
		fireballAnim = game->getAnimationManager()->get("shot");

	if(!shellCasingAnim)
		shellCasingAnim = game->getAnimationManager()->get("brass_shell");
}

void Hitler::think(const double elapsedTime)
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
		
		//if(mShotTimer.getElapsedTime() > 106/4)
		//{
			//fireWeapon();
			//mShotTimer.start();
		//}
		
		//if(mWeaponTimer.getElapsedTime() > mFireTime)
		//{
			//endAttack();
			////mpAnim->setPaused(false);
		//}
	//}
	//else
	//{	
		//Monster::think(elapsedTime);
		
		//if(!mIsDying && !mIsFrozen && !mIsStunned)
		//{
			//if(rand() % 50 == 0)
			//{
				//attack();
			//}
		//}
	//}
	
	if(rand() % 100 == 0)
	{
		makeMonsterNoise();
	}

	if(rand() % 1000 == 0)
	{
		game->getPlayer()->speak(game->getPlayer()->getQuipTauntHitler(), false, true);
	}
}

Hitler::~Hitler()
{
	/*if(oldAnim)
	{
		delete oldAnim;
	}*/
}

void Hitler::collide(Entity* other)
{
	if(other->getType() == ENTITY_ATTACK_PLAYER){
		//mShouldDelete = true;
	}

	Grenade *p = dynamic_cast<Grenade*>(other);
	if(p)
	{
		p->setVelocity(-p->getVelocity());
	}

	//if(other->getType() == ENTITY_PLAYER_KICK)
	//{
		//game->getSoundEngine()->play3DSound(game->getResourceManager()->get("metal_bump"), mPosition, 1.0f, randomFloat(0.8f,1.2f));
	//}
}

void Hitler::stun(Vec3d startPos)
{
	endAttack();

	speak(getDeathSound(), true);

	Monster::stun(startPos);
}

void Hitler::kill()
{
	Monster::kill();

	endAttack();
	
	mHealth = 0;

	//ResourceManager *resources = game->getResourceManager();

	if(!mIsDying)
	{
		//if(mVoice)
			//mVoice->stop();

		speak(std::string(),true);


		//Sound *die = game->getSoundEngine()->play3DSoundKeep(getDeathSound(), mPosition, 1.0, randomFloat(0.8f,1.2f));

		//ISoundEffectControl *fx = die->getSoundEffectControl();

		//fx->enableCompressorSoundEffect(5.f);
		//fx->enableChorusSoundEffect(50.f,100.f);


		if(rand() % 3 == 0)
		{
			Player *player = game->getPlayer();

			if(rand() % 2 == 0)
				player->speak(player->getQuipKillHitler());
			else if(rand() % 2 == 0)
			{
				if(mAttachments.size() > 0)
					player->speak(player->getQuipBurn());
			}
			else
				player->speak(player->getQuipKill());
		}

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

void Hitler::setFrozen(bool frozen)
{
	Monster::setFrozen(true);
	Monster::setFrozen(false);

	endAttack();
}

void Hitler::spawnGib(float velocity)
{
	if(!game->getAllowGibs())
		return;

	//if(mHealth <= 0)
		//return;

	Animation *arm, *leg, *torso, *head;

	arm = game->getAnimationManager()->get("guard_arm");
	leg = game->getAnimationManager()->get("guard_leg");
	torso = game->getAnimationManager()->get("guard_torso");
	head = game->getAnimationManager()->get("guard_head");
	
	for(int i = 0; i < 2; i++)
		game->getEntityManager()->add(New Gib(arm, mPosition, Vec3d(randomFloat(0.0f,0.075f),randomFloat(0.0f,0.075f),randomFloat(0.0f,0.075f)), mIsFrozen, GIB_ARM));
	for(int i = 0; i < 2; i++)
		game->getEntityManager()->add(New Gib(leg, mPosition, Vec3d(randomFloat(0.0f,0.075f),randomFloat(0.0f,0.075f),randomFloat(0.0f,0.075f)), mIsFrozen, GIB_LEG));
		
	game->getEntityManager()->add(New Gib(head, mPosition, Vec3d(randomFloat(0.0f,0.075f),randomFloat(0.0f,0.075f),randomFloat(0.0f,0.075f)), mIsFrozen, GIB_HEAD));
	game->getEntityManager()->add(New Gib(torso, mPosition, Vec3d(randomFloat(0.0f,0.075f),randomFloat(0.0f,0.075f),randomFloat(0.0f,0.075f)), mIsFrozen, GIB_TORSO));
	
}

void Hitler::makeMonsterNoise(bool priority)
{
	speak(getYellSound(), priority);
}

void Hitler::speak(std::string *soundFile, bool shouldInterrupt)
{
	//if(mVoice == NULL)
	//{
		//mVoice = game->getSoundEngine()->play3DSoundKeep(soundFile, mPosition, 4.0f, randomFloat(0.95f, 1.05f));

		//ISoundEffectControl *fx = mVoice->getSoundEffectControl();

		//fx->enableCompressorSoundEffect(5.f);
		//fx->enableChorusSoundEffect(50.f,100.f);

	//}
	//else if(mVoice->isFinished() || shouldInterrupt)
	//{
		//mVoice->stop();
		//mVoice->drop();

		//mVoice = game->getSoundEngine()->play3DSoundKeep(soundFile, mPosition,  4.0f, randomFloat(0.95f, 1.05f));
		
		//mVoice->setMinDistance(2);

		//ISoundEffectControl *fx = mVoice->getSoundEffectControl();

		//fx->enableCompressorSoundEffect(5.f);
		//fx->enableChorusSoundEffect(50.f,100.f);
		////fx->enableDistortionSoundEffect(-5.0f,75.0f,8000.f);
	//}
}

void Hitler::speak(std::string soundFile, bool shouldInterrupt)
{
	//if(mVoice == NULL)
	//{
		//mVoice = game->getSoundEngine()->play3DSoundKeep(soundFile, mPosition, 4.0f, randomFloat(0.95f, 1.05f));

		//if(!mVoice)
			//return;

		//ISoundEffectControl *fx = mVoice->getSoundEffectControl();

		//fx->enableCompressorSoundEffect(10.f);
		//fx->enableChorusSoundEffect(50.f,100.f);
	//}
	//else if(mVoice->isFinished() || shouldInterrupt)
	//{
		//mVoice->stop();
		//mVoice->drop();

		//mVoice = game->getSoundEngine()->play3DSoundKeep(soundFile, mPosition, 4.0f, randomFloat(0.95f, 1.05f));

		//if(!mVoice)
			//return;
		
		//mVoice->setMinDistance(2);

		//ISoundEffectControl *fx = mVoice->getSoundEffectControl();

		//fx->enableCompressorSoundEffect(10.f);
		//fx->enableChorusSoundEffect(50.f,100.f);
		////fx->enableDistortionSoundEffect(-5.0f,75.0f,8000.f);
	//}
}

void Hitler::removeAttachment(Entity *toBeRemoved)
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

void Hitler::removeAttachment(std::string key)
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

void Hitler::takeDamage(float hp)
{
	Monster::takeDamage(hp);
	//makeMonsterNoise();
}

void Hitler::attack()
{
	//if(!mpWeaponSound)
	//{

		//if(rand() % 5 == 0 && mPosition.getDistanceFrom(game->getPlayer()->getPosition()) >= 5)
		//{
			//throwGrenade();
		//}
		////range.setLength(1.0);

		//fireWeapon();

		//speak(&getAttackSound());
		////game->getSoundEngine()->play3DSound(game->getResourceManager()->get("flame_attack"),mPosition);
	
		//Sound *tmp = game->getSoundEngine()->play3DSoundKeep(game->getResourceManager()->get("mp40_beginfire"), mPosition, 1.0f, randomFloat(0.75f, 1.35f));

		//tmp->setMinDistance(6);

		//mpWeaponSound = game->getSoundEngine()->play3DSoundKeep(game->getResourceManager()->get("mp40_fireloop"), mPosition, 1.0f, 4.0f);
	
		//mpWeaponSound->setMinDistance(6);

		//mpWeaponSound->setIsLooped(true);
	
		//mFireTime = randomFloat(10.f, 800.f);

		//mWeaponTimer.start();
		//mShotTimer.start();
		////mpAnim->setPaused(true);

		////oldAnim = mpAnim;

		//delete mpAnim;

		//mpAnim = New Animation(*game->getAnimationManager()->get("hitler_shoot"));
	//}
}

std::string Hitler::getAttackSound()
{
	static std::vector<std::string*> toBeSaid;

	ResourceManager *manager = game->getResourceManager();

	if(toBeSaid.size() < 1)
	{
		unsigned i = 1;

		std::string *path;

		path = manager->get("hitler_attack" + std::to_string(i));

		while(path)
		{
			toBeSaid.push_back(path);
			i++;
			path = manager->get("hitler_attack" + std::to_string(i));
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

std::string Hitler::getDeathSound()
{
	static std::vector<std::string*> toBeSaid;

	ResourceManager *manager = game->getResourceManager();

	if(toBeSaid.size() < 1)
	{
		unsigned i = 1;

		std::string *path;

		path = manager->get("hitler_die" + std::to_string(i));

		while(path)
		{
			toBeSaid.push_back(path);
			i++;
			path = manager->get("hitler_die" + std::to_string(i));
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

std::string Hitler::getYellSound()
{
	static std::vector<std::string*> toBeSaid;

	ResourceManager *manager = game->getResourceManager();

	if(toBeSaid.size() < 1)
	{
		unsigned i = 1;

		std::string *path;

		path = manager->get("hitler_yell" + std::to_string(i));

		while(path)
		{
			toBeSaid.push_back(path);
			i++;
			path = manager->get("hitler_yell" + std::to_string(i));
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

void Hitler::fireWeapon()
{
	static unsigned char count = 0;

	Vec3d playerPos = game->getPlayer()->getPosition();
	
	playerPos.y = mPosition.y;
	
	playerPos -= mPosition;

	//Vec2d angle(atan2(playerPos.X-mPosition.X, playerPos.Z-mPosition.Z));

	Vec3d change = playerPos;

    /* TODO: Replace this with proper matrix math stuff
	change.rotateXYBy(90.0,change);
	change.setLength(0.60f);

	playerPos.setLength(0.15);
    */

	//playerPos.Y += 0.1f;

	//playerPos /= 8;

	/*NaziBullet *b = New NaziBullet(mPosition-change,playerPos,NULL);
	NaziBullet *b2 = New NaziBullet(mPosition+change,playerPos,NULL);
	NaziBullet *b3 = New NaziBullet((mPosition - change) + Vec3d(0,-0.2,0),playerPos,NULL);
	NaziBullet *b4 = New NaziBullet((mPosition + change) + Vec3d(0,-0.2,0),playerPos,NULL);*/

	mPosition.y += 0.1f;

	NaziBullet *b;
	
	switch(count % 4)
	{
		case 0:
			b = New NaziBullet(mPosition-change,playerPos,NULL);
			break;
		case 1:
			b = New NaziBullet(mPosition+change,playerPos,NULL);
			break;
		case 2:
			b = New NaziBullet((mPosition - change) + Vec3d(0,-0.3,0),playerPos,NULL);
			break;
		case 3:
			b = New NaziBullet((mPosition + change) + Vec3d(0,-0.3,0),playerPos,NULL);
			break;
		default:
			b = New NaziBullet(mPosition-change,playerPos,NULL);
			break;
	}

	mPosition.y -= 0.1f;

	Vec3d vel = (mPosition - b->getPosition());

	vel.y = -vel.z;

	vel = glm::normalize(vel) * 0.05f;

	vel.y = 0;

	ShellCasing *g = New ShellCasing(shellCasingAnim, b->getPosition(), vel);

	game->getEntityManager()->add(b);
	game->getEntityManager()->add(g);

	count++;
	/*game->getEntityManager()->add(b2);
	game->getEntityManager()->add(b3);
	game->getEntityManager()->add(b4);*/
}

void Hitler::throwGrenade()
{
	Vec3d playerPos = game->getPlayer()->getPosition() - mPosition;

	playerPos.y *= 1.1;

		//Vec2d angle(atan2(playerPos.X-mPosition.X, playerPos.Z-mPosition.Z));

	playerPos = glm::normalize(playerPos) * 0.2f;

	//playerPos /= 8;

	//Grenade *b = New Grnade(mPosition,playerPos,fireballAnim);
	//game->getEntityManager()->add(New Grenade(mPosition, playerPos, game->getAnimationManager()->get("stick_grenade"), Vec2d(5,10), ENTITY_ATTACK_ENEMY, 30.0));
}

void Hitler::endAttack()
{
	//if(mpWeaponSound)
	//{
          //mpWeaponSound->stop();
		//mpWeaponSound->drop();
		//mpWeaponSound = NULL;
				
		//mWeaponTimer.stop();
		//mShotTimer.stop();
				
		//Sound *tmp =game->getSoundEngine()->play3DSoundKeep(game->getResourceManager()->get("mp40_tail"), mPosition,  1.0f, randomFloat(0.95f, 1.05f));

		//tmp->setMinDistance(15);
				
		//delete mpAnim;

		//mpAnim = New Animation(*game->getAnimationManager()->get("hitler_walk"));
	//}
}
