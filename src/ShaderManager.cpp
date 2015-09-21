#include "ShaderManager.h"

//Set item type.
const std::string ShaderManager::TYPE = "Shader";

ShaderManager::~ShaderManager()
{ 
	for(std::map<std::string,Shader*>::iterator it = itemMap.begin(); it != itemMap.end(); ++it)
	{
		delete it->second;
	}

	itemMap.clear();
}

Shader* ShaderManager::get(std::string key)
{
	std::map<std::string,Shader*>::iterator it;

	it = itemMap.find(key);

	if(it == itemMap.end())
		return nullptr;
	else
		return it->second;
}

void ShaderManager::add(Shader *item)
{
	std::string tmp = TYPE + std::to_string(mKey++);
	
	itemMap[tmp] = item;
}

void ShaderManager::add(std::string key, Shader *item)
{
	itemMap[key] = item;
}

void ShaderManager::remove(std::string key)
{
	std::map<std::string,Shader*>::iterator it;

	it = itemMap.find(key);

	if(it != itemMap.end())
	{
		delete it->second;
		itemMap.erase(it);
	}
}

size_t ShaderManager::getSize()
{
	return itemMap.size();
}
