#include "Projectile.h"

#include "Map.h"
#include "Tile.h"

#include "Explosion.h"

Projectile::Projectile(Vec3d position, Vec3d velocity, Animation *animation, Vec2d damageRange, entity_types type) : Entity(animation, position)
{
	mVelocity = velocity;

	/*mVelocity.X = sin((velocity.X * PI) / 180);
	mVelocity.Y = 0;
	mVelocity.Z = -cos((velocity.X * PI) / 180);*/

	mDamageRange = damageRange;

	//mLifeTimer.start();
	mType = type;
	size = 0.2;
}

Projectile::Projectile(Vec3d position, Vec3d velocity, Animation *animation, Vec2d scale, Vec2d damageRange, entity_types type) : Entity(animation, position, scale)
{
	mVelocity = velocity;

	/*mVelocity.X = sin((velocity.X * PI) / 180);
	mVelocity.Y = 0;
	mVelocity.Z = -cos((velocity.X * PI) / 180);*/

	mDamageRange = damageRange;

	//mLifeTimer.start();
	mType = type;
	size = 0.2;
}

Projectile::~Projectile()
{

}

void Projectile::think(const double elapsedTime)
{
	//Run entity think routines.
	Entity::think(elapsedTime);

	//Add velocity to current position.
	mPosition += mVelocity;

	//Check if projectile lifetime is over.
	//if(mLifeTimer.getElapsedTime() > 5000 || game->getCurrentMap()->getTile(int(mPosition.X + 0.5), int(mPosition.Z + 0.5)).type == TYPE_WALL)
	//{
		//setShouldDelete(true);
	//}
}

void Projectile::draw()
{
	Entity::draw();
}

void Projectile::collide(Entity* other)
{
	if((mType != ENTITY_ATTACK_ENEMY && other->getType() == ENTITY_ENEMY) || (mType != ENTITY_ATTACK_PLAYER && other->getType() == ENTITY_PLAYER))
		mShouldDelete = true;

	//Enemy attacks unfreeze other enemies.
	if(mType == ENTITY_ATTACK_ENEMY && other->getFrozen())
		other->setFrozen(false);
}
