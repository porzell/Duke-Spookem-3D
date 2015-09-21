#include "SpriteManager.h"

//Set item type.
const std::string SpriteManager::TYPE = "Entity";

SpriteManager::~SpriteManager()
{ 
	for(std::map<std::string,Sprite*>::iterator it = itemMap.begin(); it != itemMap.end(); ++it)
	{
		delete it->second;
	}

	itemMap.clear();
}

Sprite* SpriteManager::get(std::string key)
{
	std::map<std::string,Sprite*>::iterator it;

	it = itemMap.find(key);

	if(it == itemMap.end())
		return nullptr;
	else
		return it->second;
}

void SpriteManager::add(Sprite *item)
{
	std::string tmp = TYPE + std::to_string(mKey++);
	
	itemMap[tmp] = item;
}

Sprite* SpriteManager::add(std::string key, Sprite *item)
{
	itemMap[key] = item;

	return item;
}

void SpriteManager::remove(std::string key)
{
	std::map<std::string,Sprite*>::iterator it;

	it = itemMap.find(key);

	if(it != itemMap.end())
	{
		delete it->second;
		itemMap.erase(it);
	}
}

size_t SpriteManager::getSize()
{
	return itemMap.size();
}
