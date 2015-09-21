#include "AnimationManager.h"

//virtual std::string AnimationManager::itemName = "item";

#include "Game.h"

extern Game *game;

//Set item type.
const std::string AnimationManager::TYPE = "Animation";

AnimationManager::~AnimationManager()
{ 
	for(std::map<std::string,Animation*>::iterator it = itemMap.begin(); it != itemMap.end(); ++it)
	{
		delete it->second;
	}

	itemMap.clear();
};

Animation* AnimationManager::get(std::string key)
{
	std::map<std::string,Animation*>::iterator it;

	it = itemMap.find(key);

	if(it == itemMap.end())
		return nullptr;
	else
		return it->second;
}

void AnimationManager::add(Animation *item)
{
	std::string tmp = TYPE + std::to_string(mKey++);
	
	itemMap[tmp] = item;
}

Animation* AnimationManager::add(std::string key, Animation *item)
{
	itemMap[key] = item;
	return item;
}

void AnimationManager::remove(std::string key)
{
	std::map<std::string,Animation*>::iterator it;

	it = itemMap.find(key);

	if(it != itemMap.end())
	{
		delete it->second;
		itemMap.erase(it);
	}
}

size_t AnimationManager::getSize()
{
	return itemMap.size();
}