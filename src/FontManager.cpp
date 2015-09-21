#include "FontManager.h"

//virtual std::string FontManager::itemName = "item";

#include "Game.h"

extern Game *game;

//Set item type.
const std::string FontManager::TYPE = "Font";

FontManager::~FontManager()
{ 
	for(std::map<std::string,Font*>::iterator it = itemMap.begin(); it != itemMap.end(); ++it)
	{
		delete it->second;
	}

	itemMap.clear();
};

Font* FontManager::get(std::string key)
{
	std::map<std::string,Font*>::iterator it;

	it = itemMap.find(key);

	if(it == itemMap.end())
		return nullptr;
	else
		return it->second;
}

void FontManager::add(Font *item)
{
	std::string tmp = TYPE + std::to_string(mKey++);
	
	itemMap[tmp] = item;
}

void FontManager::add(std::string key, Font *item)
{
	itemMap[key] = item;
}

void FontManager::remove(std::string key)
{
	std::map<std::string,Font*>::iterator it;

	it = itemMap.find(key);

	if(it != itemMap.end())
	{
		delete it->second;
		itemMap.erase(it);
	}
}

size_t FontManager::getSize()
{
	return itemMap.size();
}