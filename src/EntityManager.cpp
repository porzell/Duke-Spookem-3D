#include "EntityManager.h"

#include <vector>
#include <map>
#include <forward_list>

#include "Entity.h"

//virtual std::string EntityManager::itemName = "item";

#include "Game.h"

extern Game *game;

//Set item type.
const std::string EntityManager::TYPE = "Entity";

EntityManager::EntityManager()
{
	mKey = 0;
}

void EntityManager::think(const double elapsedTime)
{
	std::vector<std::unordered_map<std::string,Entity*>::iterator> toBeDeleted;
	
	for(auto it = itemMap.begin(); it != itemMap.end(); ++it)
	{
		if(it->second->shouldBeDeleted())
		{
			toBeDeleted.push_back(it);
			continue;
		}

		if(it->second->getType() != ENTITY_PLAYER)
			it->second->think(elapsedTime);
	}

	//If there is anything to be deleted, delete it.
	for(auto it = toBeDeleted.begin(); it != toBeDeleted.end(); ++it)
	{
		delete (*it)->second;
		itemMap.erase(*it);
	}
}

void EntityManager::draw()
{
	for(auto it = itemMap.begin(); it != itemMap.end(); ++it)
	{
		if(it->second->getType() != ENTITY_PLAYER)
			it->second->draw();
	}
}

void EntityManager::quiet()
{
	for(auto it = itemMap.begin(); it != itemMap.end(); ++it)
	{
		it->second->quiet();
	}
}

void EntityManager::unQuiet()
{
	for(auto it = itemMap.begin(); it != itemMap.end(); ++it)
	{
		it->second->unQuiet();
	}
}

void EntityManager::runCollisions()
{
	std::map<std::pair<int, int>, std::forward_list<Entity*>> collisionTable;
	for(auto i : itemMap)
	{
		std::vector<std::pair<int, int>> v = i.second->getSections();
		for(auto j : v)
			collisionTable[j].push_front(i.second);
	}

	for(auto i : collisionTable)
	{
		for(auto j = i.second.begin(); j != i.second.end(); ++j)
		{
			for(auto k = j; k != i.second.end(); ++k)
			{
				//Peter: Simple optimization. Hack sorta.
				/*if((*k)->getType() == ENTITY_GIB || (*j)->getType() == ENTITY_GIB)
				{
					if((*k)->getType() != ENTITY_PLAYER_KICK && (*j)->getType() != ENTITY_PLAYER_KICK)
						continue;
				}*/

				if(pow((*k)->getPosition().x - (*j)->getPosition().x, 2) + pow((*k)->getPosition().z - (*j)->getPosition().z, 2) <= pow((*k)->getSize()/2.0f + (*j)->getSize()/2.0f, 2) && k != j)
				{
					(*j)->collide((*k));
					(*k)->collide((*j));
				}
			}
		}
	}
}

EntityManager::~EntityManager()
{ 
	clear();
}

void EntityManager::clear()
{
	for(std::unordered_map<std::string,Entity*>::iterator it = itemMap.begin(); it != itemMap.end(); ++it)
	{
		if(it->second != (Entity*)game->getPlayer())
			delete it->second;
	}

	itemMap.clear();

	mKey = 0;
}

Entity* EntityManager::get(std::string key)
{
	std::unordered_map<std::string,Entity*>::iterator it;

	it = itemMap.find(key);

	if(it == itemMap.end())
		return nullptr;
	else
		return it->second;
}

std::string EntityManager::add(Entity *item)
{
	std::string tmp = TYPE + std::to_string(mKey++);
	
	itemMap[tmp] = item;

	//Set entity internal ID.
	item->mID = tmp;

	//game->shout("Added \"" + tmp + '\"');

	return tmp;

}

void EntityManager::add(std::string key, Entity *item)
{
	itemMap[key] = item;

	//Set entity internal ID.
	item->mID = key;
}

void EntityManager::remove(std::string key, bool true_delete = true)
{
	std::unordered_map<std::string,Entity*>::iterator it;

	it = itemMap.find(key);

	if(it != itemMap.end())
	{
		if(true_delete)
			delete it->second;
		itemMap.erase(it);
	}
}

size_t EntityManager::getSize()
{
	return itemMap.size();
}

void EntityManager::writeSave(FILE* outfile, unsigned pos)
{
	for(auto it = itemMap.begin(); it != itemMap.end(); ++it)
	{
		//it->second->writeSave(outfile, pos);
	}
}

void EntityManager::readSave(FILE* infile, unsigned pos)
{

}
