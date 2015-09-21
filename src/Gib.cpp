#include "Gib.h"

#include "Map.h"
#include "Tile.h"

#include "Game.h"
#include "AnimationManager.h"

#include "ResourceManager.h"

#include "Utils.h"
#include <glm/glm.hpp>

extern Game *game;

const float GIB_DAMPING = 0.6f;



Gib::Gib(Animation *anim, Vec3d &position, Vec3d velocity, bool frozen, GIB_TYPE type) : Entity(anim)
{
	mGibType = type;
	
	mTimeToLive = 8000;

	mPosition = position;

	mVelocity = velocity;
	
	mpAnim->setScale(Vec2d(0.15f, 0.15f));

	//mLifeTimer.start();

	rotator = randomFloat(-20.0, 20.0);

	mType = ENTITY_GIB;
	
	if(frozen)
		setFrozen(true);

	mKicked = false;
}

Gib::~Gib()
{
	//Entity class will delete mpAnim.
}

void Gib::think(const double elapsedTime)
{
	//Run entity think routines.
	Entity::think(elapsedTime);

	//Check if Gib lifetime is over.
	//if(mLifeTimer.getElapsedTime() > mTimeToLive)
	//{
		//setShouldDelete(true);
	//}

	if(mPosition.y < -0.8f)
	{
		mVelocity.y = -mVelocity.y;
		mVelocity *= GIB_DAMPING;

		if(abs(mVelocity.y) > 0.01f)
			playGibSound();

		if(rotator < 0)
		{
			rotator += 5;

			if(rotator > 0)
			{
				rotator = 0;
			}
		}
		else if(rotator > 0)
		{
			rotator -= 5;

			if(rotator < 0)
				rotator = 0;
		}

		mKicked = false;
	}
	else
		mVelocity.y -= 0.001;

	if(game->getCurrentMap()->getTile(int(mPosition.x + mVelocity.x), int(mPosition.z)).type == TYPE_WALL)
	{
		mVelocity.x = -mVelocity.x;

		mVelocity *= GIB_DAMPING;

		if(abs(mVelocity.x) > 0.01f)
			playGibSound();

		mKicked = false;
	}
	if(game->getCurrentMap()->getTile(int(mPosition.x), int(mPosition.z + mVelocity.z)).type == TYPE_WALL)
	{
		mVelocity.z = -mVelocity.z  ;

		mVelocity *= GIB_DAMPING;

		if(abs(mVelocity.z) > 0.01f)
			playGibSound();

		mKicked = false;
	}

	//Add velocity to current position.
	mPosition += mVelocity;

	mpAnim->setRotation(mpAnim->getRotation() + rotator);
}

void Gib::draw()
{
	Entity::draw();
}

void Gib::collide(Entity* other)
{
	if(other->getType() == ENTITY_PLAYER_KICK)
	{
		rotator = randomFloat(-20.0, 20.0);
	
		mVelocity = mPosition - other->getPosition();

		mVelocity = glm::normalize(mVelocity) * 0.15f;

		mVelocity.y = 0.05;

		mPosition = mPosition + mVelocity;

		//mLifeTimer.start();

		mKicked = true;
		
		mKickPosition = other->getPosition();

	}
	else if(other->getType() == ENTITY_PLAYER_PISS)
	{
		rotator = randomFloat(5.0, 5.0);

		mpAnim->setRotation(mpAnim->getRotation() + randomFloat(0,5.0));

		mVelocity = mPosition - other->getPosition();

		mVelocity.setLength(0.02);

		mVelocity.Y = 0;

		mPosition = mPosition + mVelocity;

		mLifeTimer.start();

		//mKicked = true;

	}
	else if(mGibType == GIB_HEAD && mKicked && other->getType() == ENTITY_ENEMY)
	{
		//other->spawnGib(0);
		if(mKickPosition.getDistanceFrom(other->getPosition()) > 1.7)
		{
			other->spawnGib(0);
			other->kill();
			game->getPlayer()->speak(game->getPlayer()->getQuipHeadGolf(), true);
			setShouldDelete(true);
		}
		mKicked = false;
	}

	/*if(other->getType() == ENTITY_ENEMY)
	{
		mVelocity = -mVelocity * GIB_DAMPING;
	}*/
}

void Gib::setFrozen(bool frozen)
{
	Entity::setFrozen(frozen);

	if(getFrozen())
	{
		mpAnim->setTint(Color(0.35f,0.35f,2.0f));
	}
	else
	{
		mpAnim->setTint(Color(1,1,1));
	}
}

void Gib::playGibSound()
{
	if(getFrozen() || mGibType == GIB_CIRCUIT)
	{
		game->getSoundEngine()->play3DSound(game->getResourceManager()->get("glass_impact" + std::to_string(rand() % 6 + 1)), mPosition, mVelocity.getLength() * 10, randomFloat(0.75,1.25));
	}
	else
	{
		game->getSoundEngine()->play3DSound(game->getResourceManager()->get("squishsound"), mPosition, mVelocity.getLength() * 10, randomFloat(0.75,1.25));
	}
}
