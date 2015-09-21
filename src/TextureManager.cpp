#include "TextureManager.h"

//Set item type.
const std::string TextureManager::TYPE = "Texture";

TextureManager::~TextureManager()
{ 
	for(std::map<std::string,Texture*>::iterator it = itemMap.begin(); it != itemMap.end(); ++it)
	{
		delete it->second;
	}

	itemMap.clear();
};

Texture* TextureManager::get(std::string key)
{
	std::map<std::string,Texture*>::iterator it;

	it = itemMap.find(key);

	if(it == itemMap.end())
		return nullptr;
	else
		return it->second;
}

void TextureManager::add(Texture *item)
{
	std::string tmp = TYPE + std::to_string(mKey++);
	
	itemMap[tmp] = item;
}

Texture* TextureManager::add(std::string key, Texture *item)
{
	itemMap[key] = item;
	return item;
}

void TextureManager::remove(std::string key)
{
	std::map<std::string,Texture*>::iterator it;

	it = itemMap.find(key);

	if(it != itemMap.end())
	{
		delete it->second;
		itemMap.erase(it);
	}
}

Texture* TextureManager::findTextureByGLuint(GLuint id)
{
	for(std::map<std::string,Texture*>::iterator it = itemMap.begin(); it != itemMap.end(); ++it)
	{
		if(it->second->getTextureID() == id)
			return it->second;
	}

	return NULL;
}

size_t TextureManager::getSize()
{
	return itemMap.size();
}