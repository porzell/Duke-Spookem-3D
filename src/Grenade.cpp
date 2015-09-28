#include "Grenade.h"

#include "Map.h"
#include "Tile.h"

#include "Explosion.h"

#include "Game.h"

#include "SoundEngine.h"

#include "ResourceManager.h"

#include "Utils.h"
#include <glm/glm.hpp>

extern Game *game;

const float GRENADE_DAMPING = 0.9f;

Grenade::Grenade(Vec3d position, Vec3d velocity, Animation *animation, Vec2d damageRange, entity_types type, float radius) : Projectile(position, velocity, animation, damageRange, type)
{
	mBlastRadius = radius;

	mpAnim->setScale(Vec2d(0.25,0.25));
	ttl = 4500;

	rotator = randomFloat(-20.0, 20.0);

	//mVoice = game->getSoundEngine()->play3DSoundKeep(game->getResourceManager()->get("grenade_fuse"), mPosition, 1.0, randomFloat(0.75,2.0));

	//mVoice->setIsLooped(true);
}

Grenade::~Grenade()
{
	//if(mVoice)
	//{
		//mVoice->stop();
		//mVoice->drop();
		//mVoice = NULL;
	//}
}

void Grenade::think(const double elapsedTime)
{
	//Run entity think routines.
	Entity::think(elapsedTime);

	//Check if Grenade lifetime is over.
    if(mLifeTimer.getElapsedTime() > ttl)
    {
        game->getEntityManager()->add(New Explosion(mPosition,3.0f));
        setShouldDelete(true);
    }

	mVelocity.y -= 0.001;

	if(mPosition.y < -1.f)
	{
		mVelocity.y = -mVelocity.y;
		mVelocity.y *= GRENADE_DAMPING;

		//if(abs(mVelocity.Y) > 0.01f)
			//game->getSoundEngine()->play3DSound(game->getResourceManager()->get("squishsound"), mPosition, 3.0, randomFloat(0.75,1.25));

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

		//game->getSoundEngine()->play3DSound(game->getResourceManager()->get("grenade_clink"), mPosition, 0.8f, randomFloat(0.75,1.25));
	}

	if(game->getCurrentMap()->getTile(int(mPosition.x + mVelocity.x), int(mPosition.z)).type == TYPE_WALL)
	{
		mVelocity.x = -mVelocity.x;

		mVelocity *= GRENADE_DAMPING;

		//game->getSoundEngine()->play3DSound(game->getResourceManager()->get("grenade_clink"), mPosition, 0.8f, randomFloat(0.75,1.25));

	}
	if(game->getCurrentMap()->getTile(int(mPosition.x), int(mPosition.z + mVelocity.z)).type == TYPE_WALL)
	{
		mVelocity.z = -mVelocity.z;

		mVelocity *= GRENADE_DAMPING;

		//game->getSoundEngine()->play3DSound(game->getResourceManager()->get("grenade_clink"), mPosition, 0.8f, randomFloat(0.75,1.25));
	}

	//Add velocity to current position.
	mPosition += mVelocity;

	mpAnim->setRotation(mpAnim->getRotation() + rotator);

	//if(mVoice)
		//mVoice->setPosition(mPosition);
}

void Grenade::draw()
{
	Entity::draw();
}

void Grenade::collide(Entity* other)
{
	if(other->getType() == ENTITY_ENEMY)
	{
		mVelocity.x *= 0.9f;
		mVelocity.z *= 0.9f;
		ttl = 1500;
	}
	else if(other->getType() == ENTITY_PLAYER_KICK)
	{
		//We should have this be called in Kick.cpp somehow.
		game->getSoundEngine()->play2DSound(game->getResourceManager()->get("kick"));

		mVelocity = mPosition - other->getPosition();

		mVelocity = glm::normalize(mVelocity) * 0.25f;

		mVelocity.y = 0.075;

		mPosition = mPosition + mVelocity;
	}
}

//void Grenade::quiet() { if(mVoice) mVoice->setIsPaused(true); if(mVoice) mVoice->setIsPaused(true); }
//void Grenade::unQuiet() { if(mVoice) mVoice->setIsPaused(false); if(mVoice) mVoice->setIsPaused(false); }
