#include "StickyGrenade.h"

#include "Map.h"
#include "Tile.h"

#include "Explosion.h"

StickyGrenade::StickyGrenade(Vec3d position, Vec3d velocity, Animation *animation, Vec2d damageRange, entity_types type, float radius) : Projectile(position, velocity, animation, damageRange, type)
{
	mBlastRadius = radius;

	mpAnim->setScale(Vec2d(0.25,0.25));

	mOffset = Vec3d(0);
	stuck = NULL;

	mLifeTime = 2000.0f;
	
}

StickyGrenade::~StickyGrenade()
{
}

void StickyGrenade::think(const double elapsedTime)
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
			//game->getEntityManager()->add(New Explosion(mPosition,1.0f,Vec2d(25,75)));
		//}
	//}

	if(stuck)
	{
		//Vec3d tmp = stuck->getPosition();
		//tmp.y = mPosition.y;

		//mPosition = (stuck->getPosition() + mOffset).getInterpolated(tmp, 0.85f); // TODO: Replace this

		stuck->takeDamage(0.1f);
		return;
	}

	//Add velocity to current position.
	mPosition += mVelocity;

	mVelocity.y -= 0.001f;

	if(mPosition.y < -1)
	{
		//mVelocity.Y = -mVelocity.Y;

		//mVelocity *= 0.8;

		mVelocity = Vec3d(0);
	}

	//Check if StickyGrenade lifetime is over.
	if(game->getCurrentMap()->getTile(int(mPosition.x), int(mPosition.z)).type == TYPE_WALL)
	{
		//game->getEntityManager()->add(New Explosion(mPosition));
		//mVelocity.X = -mVelocity.X * 0.8;
		//mVelocity.Z = -mVelocity.Z * 0.8;
		mVelocity = Vec3d(0);
		//mVelocity = -mVelocity * 0.8;
		//setShouldDelete(true);
	}

}

void StickyGrenade::draw()
{
	Entity::draw();
}

void StickyGrenade::collide(Entity* other)
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

		if(other->getType() == ENTITY_ENEMY || other->getType() == ENTITY_GIB)
		{
			stuck = other;

			other->addAttachment(this);

			mLifeTime = 3000.0f * (other->getAttachments()->size()/2 + 1);

			mVelocity = Vec3d(0);

			mOffset = mPosition - stuck->getPosition();

			//mLifeTimer.start();

			mpAnim->setScale(Vec2d(0.25f, 0.50f));

			//game->getEntityManager()->add(New Explosion(mPosition));
			//setShouldDelete(true);

			//mVelocity = -mVelocity * 0.8;
		}
	}
}
