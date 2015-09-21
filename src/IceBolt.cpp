#include "IceBolt.h"

#include "Map.h"
#include "Tile.h"

#include "Explosion.h"

#include "Player.h"

#include "Game.h"

extern Game *game;

IceBolt::IceBolt(Vec3d &position, Vec3d &velocity, Animation *animation, Vec2d& damageRange, entity_types type) : Projectile(position, velocity, animation, damageRange, type)
{
	mpAnim->setScale(Vec2d(0.25,0.25));
}

IceBolt::~IceBolt()
{
}

void IceBolt::think(const double elapsedTime)
{
	//Run entity think routines.
	Entity::think(elapsedTime);

	//Add velocity to current position.
	mPosition += mVelocity;

	mVelocity.y -= 0.0005;

	if(mPosition.y < -1)
	{
		setShouldDelete(true);
	}

	//Check if IceBolt lifetime is over.
	if(game->getCurrentMap()->getTile(int(mPosition.x), int(mPosition.z)).type == TYPE_WALL)
	{
		setShouldDelete(true);
	}

}

void IceBolt::draw()
{
	Entity::draw();
}

void IceBolt::collide(Entity* other)
{
	if(other->getType() == ENTITY_ENEMY)
	{
		if(!other->getFrozen())
		{
			Player *player = game->getPlayer();
			player->speak(player->getQuipFreeze());
		}

		other->removeAllAttachments();

		setShouldDelete(true);
	}
	else if(other->getType() == ENTITY_ATTACK_ENEMY)
	{
		other->setShouldDelete(true);
		setShouldDelete(true);
	}

	other->setFrozen(true);
}
