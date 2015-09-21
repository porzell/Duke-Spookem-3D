#include "Puddle.h"

#include "Map.h"
#include "Tile.h"

#include "Utils.h"

Puddle::Puddle(Vec3d &position, entity_types type) : Projectile(position, Vec3d(0,0,0), game->getAnimationManager()->get("pee_puddle"), Vec2d(0,0), Vec2d(0,0), type)
{
	mOffset = Vec3d(0);

	mLifeTime = 3000.0f;

	setSize(0.001f);
}

Puddle::~Puddle()
{
}

void Puddle::think(const double elapsedTime)
{
	//Run entity think routines.
	Entity::think(elapsedTime);

	//if(mLifeTimer.getElapsedTime() > mLifeTime)
	//{
			//setShouldDelete(true);
			////game->getEntityManager()->add(New Explosion(mPosition,1.0f,Vec2d(25,75)));
	//}

	/*if(stuck)
	{
		Vec3d tmp = stuck->getPosition();
		tmp.Y = mPosition.Y;

		mPosition = (stuck->getPosition() + mOffset).getInterpolated(tmp, 0.85f);

		stuck->takeDamage(0.1f);
		return;
	}*/

	//if(mLifeTime - mLifeTimer.getElapsedTime() < 1000 && size > 0)
	//{
		//setSize(getSize() - 0.01f);

		//mpAnim->setScale(Vec2d(getSize(),getSize()));
	//}
	////else if(getSize() < 0.10f)
	//else if(getSize() < 0.25f)
	//{
		//setSize(getSize() + 0.01f);

		//mpAnim->setScale(Vec2d(getSize(),getSize()));
	//}
}

void Puddle::draw()
{
	Projectile::draw();
}

void Puddle::collide(Entity* other)
{
}
