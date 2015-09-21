#include "Entity.h"

#include "Animation.h"

#include "Game.h"

#include "Player.h"

#include "SoundEngine.h"

#include "Map.h"

#include "Monster.h"

extern Game *game;

using namespace std;

Entity::Entity()
{
	mShouldDeleteAnim = true;
	//mHasOwner = false;
	mShouldDelete = false;
	mpAnim = NULL;
	mPaused = false;
	mSeenPlayer = false;
	mIsFrozen = false;
	mType = ENTITY_GENERIC;
	size = 1;

	mHealth = 100;
}

Entity::Entity(Animation &animation)
{
	mShouldDeleteAnim = true;
	//mHasOwner = false;
	mShouldDelete = false;
	mpAnim = New Animation(animation);
	mPaused = false;
	mSeenPlayer = false;
	mIsFrozen = false;
	mType = ENTITY_GENERIC;
	size = 1;

	mHealth = 100;
}

Entity::Entity(Animation *animation, bool shouldDeleteAnim)
{
	mShouldDeleteAnim = shouldDeleteAnim;
	//mHasOwner = false;
	mShouldDelete = false;
	mpAnim = New Animation(*animation);
	mPaused = false;
	mSeenPlayer = false;
	mIsFrozen = false;
	mType = ENTITY_GENERIC;
	size = 1;

	mHealth = 100;
}

Entity::Entity(Animation &animation, Vec3d &position)
{
	mShouldDeleteAnim = true;
	//mHasOwner = false;
	mShouldDelete = false;
	mpAnim = New Animation(animation);
	mPosition = position;
	mPaused = false;
	mSeenPlayer = false;
	mIsFrozen = false;
	mType = ENTITY_GENERIC;
	size = 1;

	mHealth = 100;
}

Entity::Entity(Animation *animation, Vec3d &position)
{
	mShouldDeleteAnim = true;
	//mHasOwner = false;
	mShouldDelete = false;

	if(animation)
		mpAnim = New Animation(*animation);
	else
		mpAnim = NULL;

	mPosition = position;
	mPaused = false;
	mSeenPlayer = false;
	mIsFrozen = false;
	mType = ENTITY_GENERIC;
	size = 1;

	mHealth = 100;
}

Entity::Entity(Animation *animation, Vec3d &position, Vec2d &scale)
{
	mShouldDeleteAnim = true;
	//mHasOwner = false;
	mShouldDelete = false;
	mpAnim = New Animation(*animation);

	mpAnim->setScale(scale);

	mPosition = position;
	mPaused = false;
	mSeenPlayer = false;
	mIsFrozen = false;
	mType = ENTITY_GENERIC;
	size = 1;

	mHealth = 100;
}

Entity::Entity(const Entity& entity)
{
	mShouldDeleteAnim = true;
	//mHasOwner = false;
	mShouldDelete = false;
	mpAnim = New Animation(*(entity.mpAnim));
	mPosition = entity.mPosition;
	mPaused = false;
	mSeenPlayer = false;
	mIsFrozen = false;
	mType = ENTITY_GENERIC;
	size = 1;

	mHealth = 100;
}

Entity::~Entity()
{
	removeAllAttachments();

	if(mpAnim)
		delete mpAnim;
}

void Entity::removeAllAttachments()
{
	//Remove all attachments left.
	for(auto it = mAttachments.begin(); it != mAttachments.end();  ++it)
	{
		//game->getEntityManager()->remove((*it)->getID(), false);
		//delete (*it);
		//(*it)->setOwned(false);
		it->second->setShouldDelete(true);
	}

	mAttachments.clear();
}

vector<pair<int, int>> Entity::getSections()
{
	vector<pair<int, int>> pos;
	for(float i = mPosition.X - 0.5f - size/2; i < mPosition.X + 0.5f + size/2; ++i)
	{
		for(float j = mPosition.Z - 0.5f - size/2; j < mPosition.Z + 0.5f + size/2; ++j)
		{
			pos.push_back(pair<int, int>(int(i + 0.5f), int(j + 0.5f)));
		}
	}
	return pos;
}

void Entity::draw()
{
	if(mpAnim)
		mpAnim->draw(mPosition);

	//Vec3d playerPos = game->getPlayer()->getPosition();

	/*glBegin(GL_QUADS);
	glColor3b(255,0,0);
	glVertex3f(playerPos.X, playerPos.Y, playerPos.Z);
	glVertex3f(playerPos.X, playerPos.Y, playerPos.Z+2);
	glVertex3f(mPosition.X, mPosition.Y, mPosition.Z);
	glVertex3f(mPosition.X, mPosition.Y, mPosition.Z+2);
	glEnd();*/
}

Animation& Entity::getAnimation()
{
	return *mpAnim;
}

void Entity::think(const double mElapsedTime)
{
	if(!mPaused && mType != ENTITY_PLAYER)
	{
		if(mpAnim)
			mpAnim->update(mElapsedTime);
	}
}

/*bool Entity::isWithinBounds(Vec2d point)
{
	return (point.X() >= mPosition.X() &&
			point.Y() >= mPosition.Y() &&
			point.X() <= mPosition.X() + (abs(mpAnim->getCurrentSprite()->getSizeAndScale().X())) &&
			point.Y() <= mPosition.Y() + (abs(mpAnim->getCurrentSprite()->getSizeAndScale().Y()))
			);
}

bool Entity::isColliding(Entity *other)
{
	Vec2d dimensions = mpAnim->getCurrentSprite()->getSizeAndScale();

	Vec2d otherDimensions = other->mpAnim->getCurrentSprite()->getSizeAndScale();

	if( mPosition.X() + dimensions.X() < other->mPosition.X() || mPosition.X() > other->mPosition.X() + otherDimensions.X() )
		return false;

	if( mPosition.Y() + dimensions.Y() < other->mPosition.Y() || mPosition.Y() > other->mPosition.Y() + otherDimensions.Y() )
		return false;

	return true;
}*/

void Entity::writeSave(FILE* outfile, unsigned pos)
{
   /* To save:
	* Id, string
	* Animation, ???
	* Attachments, ???
	* Position, 3 floats
	* Paused, bool
	* Seen Player, bool
	* Type, uchar
	* Size, float
	*/
}

void Entity::readSave(FILE* infile, unsigned pos)
{
   /* To load:
	* Id, string
	* Animation, ???
	* Attachments, ???
	* Position, 3 floats
	* Paused, bool
	* Seen Player, bool
	* Type, uchar
	* Size, float
	*/
}