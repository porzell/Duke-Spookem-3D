#include "ShellCasing.h"

#include "Map.h"
#include "Tile.h"

#include "Game.h"
#include "AnimationManager.h"
#include "SoundEngine.h"
#include "ResourceManager.h"

#include "Utils.h"

extern Game *game;

const float DAMPING = 0.85f;

ShellCasing::ShellCasing(Vec3d &position, Vec3d &velocity) : Entity(game->getAnimationManager()->get("brass_shell"))
{
	
	mTimeToLive = 1000;

	mPosition = position;

	mVelocity = velocity;
	
	//mpAnim->setScale(Vec2d(0.15f, 0.15f));

	//mLifeTimer.start();

	rotator = randomFloat(-20.0, 20.0);
}

ShellCasing::ShellCasing(Animation *anim, Vec3d &position, Vec3d &velocity) : Entity(anim)
{
	
	mTimeToLive = 1000;

	mPosition = position;

	mVelocity = velocity;

	//mLifeTimer.start();

	rotator = randomFloat(-20.0, 20.0);
}

ShellCasing::~ShellCasing()
{
	//Entity class will delete mpAnim.
}

void ShellCasing::think(const double elapsedTime)
{
	//Run entity think routines.
	Entity::think(elapsedTime);

	//Check if ShellCasing lifetime is over.
	//if(mLifeTimer.getElapsedTime() > mTimeToLive)
	//{
		//setShouldDelete(true);
	//}

	mVelocity.y -= 0.001;

	if(mPosition.y < -0.9f)
	{
		mVelocity.y = -mVelocity.y;
		mVelocity *= DAMPING;

		if(abs(mVelocity.y) > 0.01f)
			playShellCasingSound();

		if(rotator < 0)
		{
			rotator += 5;

			if(rotator > 0)
			{
				rotator = 0;
			}
		}
		else if(rotator > 0)
		{
			rotator -= 5;

			if(rotator < 0)
				rotator = 0;
		}
	}

	if(game->getCurrentMap()->getTile(int(mPosition.x + mVelocity.x), int(mPosition.z)).type == TYPE_WALL)
	{
		mVelocity.x = -mVelocity.x;

		mVelocity *= DAMPING;

		if(abs(mVelocity.x) > 0.01f)
			playShellCasingSound();
	}
	if(game->getCurrentMap()->getTile(int(mPosition.x), int(mPosition.z + mVelocity.z)).type == TYPE_WALL)
	{
		mVelocity.z = -mVelocity.z;

		mVelocity *= DAMPING;

		if(abs(mVelocity.z) > 0.01f)
			playShellCasingSound();
	}

	//Add velocity to current position.
	mPosition += mVelocity;

	mpAnim->setRotation(mpAnim->getRotation() + rotator);
}

void ShellCasing::draw()
{
	//Color c(1.0f,convertScale(mLifeTimer.getElapsedTime(), 0.0f, mTimeToLive, 0.0f, 1.0f),0.0f);

	//mpAnim->setTint(c);

	Entity::draw();
	/*Vec3d move = mVelocity;
	move.setLength(0.05f);

	glPushMatrix();

	glTranslatef(mPosition.X, mPosition.Y, mPosition.Z);

	glRotatef(mpAnim->getRotation(), 0.0, 0.0, 1.0);

	glDisable(GL_TEXTURE_2D);

	glLineWidth(5);

	glBegin(GL_LINES);

	glColor3f(1.0f,convertScale(mLifeTimer.getElapsedTime(), 0.0f, mTimeToLive, 0.0f, 1.0f),0.0f);

	glVertex3f(0,0,0);
	glVertex3f(move.X, move.Y, move.Z);

	glEnd();

	glPopMatrix();

	glEnable(GL_TEXTURE_2D);*/
}

void ShellCasing::playShellCasingSound()
{
	//game->getSoundEngine()->play3DSound(game->getResourceManager()->get("glass_impact" + std::to_string(rand() % 6 + 1)), mPosition, mVelocity.getLength() * 10, randomFloat(0.75,1.25));
}
