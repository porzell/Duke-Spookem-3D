#include "Piss.h"

#include "Map.h"
#include "Tile.h"

#include "Explosion.h"

#include "Monster.h"

#include "Puddle.h"

#include "Utils.h"
#include <glm/glm.hpp>
#include <algorithm>

Piss::Piss(std::string lastPiss, Vec3d position, Vec3d velocity, entity_types type) : Projectile(position, velocity, NULL, Vec2d(0,0), type)
{
	mLastPiss = lastPiss;

	mOffset = Vec3d(0);

	mLifeTime = 1000.0f;

	mLastPosition = mPosition - velocity;
}

Piss::~Piss()
{
}

void Piss::think(const double elapsedTime)
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

	mLastPosition = mPosition;

	//Add velocity to current position.
	mPosition += mVelocity;

	mVelocity.y -= 0.001f;

	mVelocity += Vec3d(randomFloat(-0.001,0.001), randomFloat(-0.001,0.001), randomFloat(-0.001,0.001));

	//Check if Piss lifetime is over.
	if(game->getCurrentMap()->getTile(int(mPosition.x), int(mPosition.z)).type == TYPE_WALL || mPosition.y <= -1 + getSize())
	{
		game->getEntityManager()->add(New Puddle(mPosition, ENTITY_PLAYER_DECAPITATABLE));
		//mVelocity.X = -mVelocity.X * 0.8;
		//mVelocity.Z = -mVelocity.Z * 0.8;
		mVelocity = Vec3d(0);
		//mVelocity = -mVelocity * 0.8;
		setShouldDelete(true);

	}

}

void Piss::draw()
{
	Piss *piss = (Piss*)game->getEntityManager()->get(mLastPiss);

	if(piss)
	{
		glPushMatrix();

		glDisable(GL_TEXTURE_2D);

		glLineWidth(std::max<float>(0,10-glm::distance(game->getPlayer()->getPosition(), mPosition)));

		glBegin(GL_LINES);

		glColor3f(1.0f,1.0f,0.0f);

	
		Vec3d lastPiss = piss->getPosition();

		//glVertex3f(mLastPosition.X, mLastPosition.Y, mLastPosition.Z);
		glVertex3f(lastPiss.x, lastPiss.y, lastPiss.z);
		glColor3f(1.0f,1.0f,0.0f);
		glVertex3f(mPosition.x, mPosition.y, mPosition.z);

		glEnd();

		glPopMatrix();

		glEnable(GL_TEXTURE_2D);
	}
}

void Piss::collide(Entity* other)
{
	if(other->getAttachments()->size() > 0)
	{
		//other->removeAllAttachments();

		auto it = other->getAttachments()->begin();

		it->second->setShouldDelete(true);
		other->getAttachments()->erase(it); 

		if(other->getType() == ENTITY_ENEMY)
			((Monster*)other)->shutUp();
	}
	if(mVelocity != Vec3d(0,0,0))
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
			//other->takeDamage(0.1f);

			Vec3d vel = other->getPosition() - mPosition;

			vel = glm::normalize(vel) * 0.1f;

			//vel.Y = 0.075;

			((Monster*)other)->stun(vel);
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
