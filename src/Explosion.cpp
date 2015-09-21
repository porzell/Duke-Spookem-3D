#include "Explosion.h"

#include "Map.h"
#include "Tile.h"

#include "SoundEngine.h"

#include "ResourceManager.h"

#include "Game.h"

#include "Monster.h"

#include "Utils.h"

extern Game *game;

Explosion::Explosion(Vec3d &position) : Entity(game->getAnimationManager()->get("fireball"), position)
{
	//mLifeTimer.start();
	//game->getSoundEngine()->play3DSound(game->getResourceManager()->get("grenade_explode"),mPosition, 2.0f, randomFloat(0.75, 1.25));

	mDamageRange.set(100,100);

	mDamage = -1;
}

Explosion::Explosion(Vec3d &position, GLfloat scale) : Entity(game->getAnimationManager()->get("fireball"), position, Vec2d(scale, scale))
{
	setSize(scale);
	//mLifeTimer.start();
	//game->getSoundEngine()->play3DSound(game->getResourceManager()->get("grenade_explode"),mPosition, 2.0f * scale, randomFloat(0.75, 1.25));

	mDamageRange.set(100,100);

	mDamage = -1;
}

Explosion::Explosion(Vec3d &position, GLfloat scale, Vec2d damageRange) : Entity(game->getAnimationManager()->get("fireball"), position, Vec2d(scale, scale))
{
	setSize(int(scale));
	//mLifeTimer.start();
	//game->getSoundEngine()->play3DSound(game->getResourceManager()->get("grenade_explode"),mPosition, 2.0f * scale, randomFloat(0.75, 1.25));

	mDamageRange = damageRange;

	mDamage = -1;
}

Explosion::~Explosion()
{

}

void Explosion::think(const double elapsedTime)
{
	//Run entity think routines.
	Entity::think(elapsedTime);

	//Check if Explosion lifetime is over.
	//if(mLifeTimer.getElapsedTime() > 1000)
		//setShouldDelete(true);
}

void Explosion::draw()
{
	Entity::draw();
}

void Explosion::collide(Entity* other)
{
	if(mDamage == -1)
		mDamage = randomFloat(mDamageRange.X(), mDamageRange.Y());


	if(other->getType() == ENTITY_ENEMY && !((Monster*)other)->isDying() && ((Monster*)other)->getMonsterType() != HITLER_BOSS )
	{
		//other->kill();
		other->takeDamage(/*(mDamage * float(size)/3) / mPosition.getDistanceFrom(other->getPosition())*/mDamage);

		if(other->getHealth() <= 0)
			other->spawnGib(0);

		if(rand() % 4 == 0)
			game->getPlayer()->speak(game->getPlayer()->getQuipExplosion());
	}
}
