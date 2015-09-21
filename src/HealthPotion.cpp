#include "HealthPotion.h"

#include "Map.h"
#include "Tile.h"

#include "AnimationManager.h"

#include "Game.h"

#include "Utils.h"

extern Game *game;

HealthPotion::HealthPotion(Vec3d &position, Vec2d &healthRange) : Entity(game->getAnimationManager()->get("healthPotion"), position)
{
	mHealthRange = healthRange;

	mType = ENTITY_HEALTH_POTION;

	mpAnim->setScale(Vec2d(0.25f, 0.25f));

	mPosition.Y = -0.8f;
	
	//setSize(0.2);
}

HealthPotion::~HealthPotion()
{
	//Entity will delete stuff.
}

void HealthPotion::think(const double elapsedTime)
{
	//Run entity think routines.
	/*Entity::think(elapsedTime);

	if(mPosition.Y > -0.8)
	{
		mVelocity.Y -= 0.005;
		mPosition += mVelocity;
	}*/

	/*//Add velocity to current position.
	mPosition += mVelocity;

	//Check if HealthPotion lifetime is over.
	if(mLifeTimer.getElapsedTime() > 5000 || game->getCurrentMap()->getTile(int(mPosition.X + 0.5), int(mPosition.Z + 0.5)).type == TYPE_WALL)
	{
		setShouldDelete(true);
	}*/
}

void HealthPotion::draw()
{
	Entity::draw();
}

void HealthPotion::collide(Entity* other)
{
	if(other->getType() == ENTITY_PLAYER)
	{
		if(game->getPlayer()->getHealth() <= 100 - mHealthRange.X())
		{
			game->getPlayer()->heal(randomFloat(mHealthRange.X(), mHealthRange.Y()));
			mShouldDelete = true;
		}
	}
}