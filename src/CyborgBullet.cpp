#include "CyborgBullet.h"

#include "Map.h"
#include "Tile.h"

#include "Explosion.h"

#include "Game.h"
#include "ResourceManager.h"
#include "SoundEngine.h"

#include "Utils.h"

extern Game *game;

CyborgBullet::CyborgBullet(Vec3d &position, Vec3d &velocity, Color &color) : Projectile(position,velocity, NULL, Vec2d(0.1,10.0), ENTITY_ATTACK_ENEMY)
{
	mColor = color;
	mVelocity = velocity;

	/*mVelocity.X = sin((velocity.X * PI) / 180);
	mVelocity.Y = 0;
	mVelocity.Z = -cos((velocity.X * PI) / 180);*/
	size = 0.75;

	mpSound = game->getSoundEngine()->play3DSoundKeep(game->getResourceManager()->get("laser_beam"), mPosition, 1.0, randomFloat(0.5,2.0));

	mpSound->setIsLooped(true);
}

CyborgBullet::~CyborgBullet()
{
	mpSound->stop();
	mpSound->drop();
}

void CyborgBullet::think(const double elapsedTime)
{
	//Run entity think routines.
	Entity::think(elapsedTime);

	//Add velocity to current position.
	mPosition += mVelocity;

	//Check if CyborgBullet lifetime is over.
	if(game->getCurrentMap()->getTile(int(mPosition.X + 0.5), int(mPosition.Z + 0.5)).type == TYPE_WALL)
	{
		game->getSoundEngine()->play3DSound(getRicochetSound(), mPosition, 1.0, randomFloat(0.5,2.0));
		setShouldDelete(true);
	}

	mpSound->setPosition(mPosition);
}

void CyborgBullet::draw()
{
	//Entity::draw();

	glPushMatrix();

	glTranslatef(mPosition.X, mPosition.Y, mPosition.Z);

	glDisable(GL_TEXTURE_2D);

	glLineWidth(5);

	glBegin(GL_LINES);

	mColor.setActiveColor();

	glVertex3f(0,0,0);
	glVertex3f(mVelocity.X, mVelocity.Y, mVelocity.Z);

	glEnd();

	glPopMatrix();

	glEnable(GL_TEXTURE_2D);
}

void CyborgBullet::collide(Entity* other)
{
	if((mType != ENTITY_ATTACK_ENEMY && other->getType() == ENTITY_ENEMY) || (mType != ENTITY_ATTACK_PLAYER && other->getType() == ENTITY_PLAYER))
		mShouldDelete = true;

	//Enemy attacks unfreeze other enemies.
	if(mType == ENTITY_ATTACK_ENEMY && other->getFrozen())
		other->setFrozen(false);
}

void CyborgBullet::quiet() { if(mpSound) mpSound->setIsPaused(true); if(mpSound) mpSound->setIsPaused(true); }
void CyborgBullet::unQuiet() { if(mpSound) mpSound->setIsPaused(false); if(mpSound) mpSound->setIsPaused(false); }

std::string CyborgBullet::getRicochetSound()
{
	static std::vector<std::string*> toBeSaid;

	ResourceManager *manager = game->getResourceManager();

	if(toBeSaid.size() < 1)
	{
		unsigned i = 1;

		std::string *path;

		path = manager->get("ricochet" + std::to_string(i));

		while(path)
		{
			toBeSaid.push_back(path);
			i++;
			path = manager->get("ricochet" + std::to_string(i));
		}
	}

	unsigned index = rand() % toBeSaid.size();

	//Get a pointer to a string to return.
	std::string *out = toBeSaid[index];

	//Remove the quip we just said.
	toBeSaid.erase(toBeSaid.begin() + index);

	//Return the string path to a quip.
	return *out;
}