#include "FlamePuff.h"

#include "Map.h"
#include "Tile.h"

#include "Explosion.h"

FlamePuff::FlamePuff(Vec3d position, Vec3d velocity, Animation *animation, Vec2d damageRange, entity_types type) : Projectile(position, velocity, animation, damageRange, type)
{
	setSize(0.01);

	mpAnim->setScale(Vec2d(getSize(), getSize()));

	mOffset = Vec3d(0);
	stuck = NULL;

	mLifeTime = 500.0f;
	
}

FlamePuff::~FlamePuff()
{
}

void FlamePuff::think(const double elapsedTime)
{
	//Run entity think routines.
	Entity::think(elapsedTime);

	//if(mLifeTimer.getElapsedTime() > mLifeTime)
	//{
		//if(stuck)
		//{
			//stuck->removeAttachment(this);
			////stuck->kill();
		//}
		//else
		//{
			//setShouldDelete(true);
			////game->getEntityManager()->add(New Explosion(mPosition,1.0f,Vec2d(25,75)));
		//}
	//}

	setSize(getSize() + 0.01f);

	mpAnim->setScale(Vec2d(getSize(),getSize()));

	/*if(stuck)
	{
		Vec3d tmp = stuck->getPosition();
		tmp.Y = mPosition.Y;

		mPosition = (stuck->getPosition() + mOffset).getInterpolated(tmp, 0.85f);

		stuck->takeDamage(0.1f);
		return;
	}*/



	//Add velocity to current position.
	mPosition += mVelocity;

	mVelocity.y  -= 0.001f;

	//Check if FlamePuff lifetime is over.
	if(game->getCurrentMap()->getTile(int(mPosition.x), int(mPosition.z)).type == TYPE_WALL || mPosition.y <= -1 + getSize())
	{
		//game->getEntityManager()->add(New Explosion(mPosition));
		//mVelocity.X = -mVelocity.X * 0.8;
		//mVelocity.Z = -mVelocity.Z * 0.8;
		mVelocity = Vec3d(0);
		mLifeTime = 4000.0f;
		//mVelocity = -mVelocity * 0.8;
		//setShouldDelete(true);
	}

}

void FlamePuff::draw()
{
	Entity::draw();
}

void FlamePuff::collide(Entity* other)
{
	if(!stuck && mVelocity != Vec3d(0,0,0))
	{
		if(other->getFrozen())
		{
				other->setFrozen(false);
				setShouldDelete(true);

				if(other->getType() == ENTITY_ENEMY)
				{
					if(rand() % 3 == 0)
						game->getPlayer()->speak(game->getPlayer()->getQuipThaw());
				}
				return;
		}

		/*if(other->getType() == ENTITY_ENEMY || other->getType() == ENTITY_GIB)
		{
			stuck = other;

			other->addAttachment(this);

			mLifeTime = 3000.0f * (other->getAttachments()->size()/2 + 1);

			mVelocity.set(0,0,0);

			mOffset = mPosition - stuck->getPosition();

			mLifeTimer.start();

			mpAnim->setScale(Vec2d(0.25f, 0.50f));

			//game->getEntityManager()->add(New Explosion(mPosition));
			//setShouldDelete(true);

			//mVelocity = -mVelocity * 0.8;
		}*/

		if(other->getType() == ENTITY_ENEMY)
		{
			other->takeDamage(2.0f);
		}
	}
	else
	{
		if(other->getType() == ENTITY_ENEMY || other->getType() == ENTITY_PLAYER)
		{
			other->takeDamage(0.01f);
		}
	}
}
