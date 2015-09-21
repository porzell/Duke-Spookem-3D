#include "NaziBullet.h"

#include "Map.h"
#include "Tile.h"

#include "Explosion.h"

#include "Game.h"
#include "ResourceManager.h"
#include "SoundEngine.h"
#include <glm/glm.hpp>

extern Game *game;

NaziBullet::NaziBullet(Vec3d position, Vec3d velocity, Animation *anim) : Projectile(position,velocity,anim, Vec2d(0.1,2.0), ENTITY_ATTACK_ENEMY)
{
	mVelocity = velocity;

	/*mVelocity.X = sin((velocity.X * PI) / 180);
	mVelocity.Y = 0;
	mVelocity.Z = -cos((velocity.X * PI) / 180);*/
	size = 0.2;
}

NaziBullet::~NaziBullet()
{

}

void NaziBullet::think(const double elapsedTime)
{
	//Run entity think routines.
	Entity::think(elapsedTime);

	//Add velocity to current position.
	mPosition += mVelocity;

	//Check if NaziBullet lifetime is over.
	if(game->getCurrentMap()->getTile(int(mPosition.x + 0.5), int(mPosition.z + 0.5)).type == TYPE_WALL)
	{
		//game->getSoundEngine()->play3DSound(getRicochetSound(), mPosition);
		setShouldDelete(true);
	}
}

void NaziBullet::draw()
{
	//Entity::draw(); 

	Vec3d move = mVelocity;

	move = glm::normalize(move) * 0.5f;

	glPushMatrix();

	glTranslatef(mPosition.x, mPosition.y, mPosition.z);

	glDisable(GL_TEXTURE_2D);

	glLineWidth(5);

	glBegin(GL_LINES);

	glColor3f(1.0f,1.0f,0.0f);

	glVertex3f(0,0,0);
	glVertex3f(move.x, move.y, move.z);

	glEnd();

	glPopMatrix();

	glEnable(GL_TEXTURE_2D);
}

void NaziBullet::collide(Entity* other)
{
	if((mType != ENTITY_ATTACK_ENEMY && other->getType() == ENTITY_ENEMY) || (mType != ENTITY_ATTACK_PLAYER && other->getType() == ENTITY_PLAYER))
		mShouldDelete = true;

	//Enemy attacks unfreeze other enemies.
	/*if(mType == ENTITY_ATTACK_ENEMY && other->getFrozen())
		other->setFrozen(false);*/
}

std::string NaziBullet::getRicochetSound()
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
