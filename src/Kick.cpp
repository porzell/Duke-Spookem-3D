#include "Kick.h"

#include "Map.h"
#include "Tile.h"

#include "SoundEngine.h"

#include "ResourceManager.h"

#include "Game.h"

#include "Monster.h"

#include "Gib.h"

#include "Utils.h"
#include <glm/glm.hpp>

extern Game *game;

Kick::Kick(Vec3d &position) : Entity(NULL, position)
{
	mLifeTimer.start();

	setSize(2);

	mType = ENTITY_PLAYER_KICK;

	//mVoice = NULL;

	mHasHitHeadGib = false;
}

Kick::~Kick()
{
	//if(mVoice)
	//{
		////mVoice->stop();
		//mVoice->drop();

		//mVoice = NULL;
	//}
}

void Kick::think(const double elapsedTime)
{
	//Run entity think routines.
	Entity::think(elapsedTime);

	//Check if Kick lifetime is over.
	if(mLifeTimer.getElapsedTime() > 100)
		setShouldDelete(true);
}

void Kick::draw()
{
	//Entity::draw();
}

void Kick::collide(Entity* other)
{
	if(other->getType() == ENTITY_GIB)
	{
		//if(!mVoice)
			//mVoice = game->getSoundEngine()->play2DSoundKeep(game->getResourceManager()->get("kick"));

		if(other->getType() == ENTITY_GIB)
		{
			if(!mHasHitHeadGib && ((Gib*)other)->getGibType() == GIB_HEAD)
			{
				if(rand() % 2 == 0)
					game->getPlayer()->speak(game->getPlayer()->getQuipDecap());
				mHasHitHeadGib = true;
			}
		}
		setShouldDelete(true);
	}
	if(other->getType() == ENTITY_ENEMY)
	{
		//We now know the other entity is a Monster.
		Monster *monster = (Monster*)other;

		//if(!mVoice)
			//mVoice = game->getSoundEngine()->play2DSoundKeep(game->getResourceManager()->get("kick"));

		//other->spawnGib(0);

		if(monster->getFrozen() && !monster->isDying())
		{
			std::cerr << "sPAWNING TEH GIBS on entity " << monster->getID() << ".\n";
			monster->spawnGib(0);

			switch(rand() % 5)
			{
				case 2:
					game->getPlayer()->speak(game->getPlayer()->getQuipShatter());
					break;
				case 3:
					game->getPlayer()->speak(game->getPlayer()->getQuipKick());
					break;
				default:
					break;
			}

			monster->kill();

		
			//game->getSoundEngine()->play2DSound(game->getResourceManager()->get("shatter" + std::to_string(rand() % 10 + 1)));

			//setShouldDelete(true);
		}
		else
		{

			Vec3d vel = other->getPosition() - mPosition;

			//vel.setLength(0.25);
            vel = glm::normalize(vel) * 0.25f;

			vel.y = 0.075;

			((Monster*)other)->stun(vel);

			other->takeDamage(10);

			/*Monster* monster = (Monster*)other;

			Animation *head = game->getAnimationManager()->get("swampmonster_head");

			monster->kill();
			game->getEntityManager()->add(New Gib(head, monster->getPosition(), Vec3d(randomFloat(0.0f,0.075f),randomFloat(0.0f,0.075f),randomFloat(0.0f,0.075f)), monster->getFrozen(), GIB_HEAD));*/	

			if(rand() % 2 == 0)
				game->getPlayer()->speak(game->getPlayer()->getQuipKick());
		}
		//else
		//{
			//game->getSoundEngine()->play2DSound(game->getResourceManager()->get("kick"));
			setShouldDelete(true);
		//}
	}
}
