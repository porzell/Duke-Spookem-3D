#include "ResourceManager.h"

//virtual std::string ResourceManager::itemName = "item";

#include "Game.h"

#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>

#include <sstream>

using namespace rapidxml;

extern Game *game;

//Set item type.
const std::string ResourceManager::TYPE = "FilePath";

ResourceManager::~ResourceManager()
{ 
	for(std::map<std::string,std::string*>::iterator it = itemMap.begin(); it != itemMap.end(); ++it)
	{
		delete it->second;
	}

	itemMap.clear();
};

bool ResourceManager::load(std::string path)
{
	xml_document<> doc;
	
	std::ifstream file(path);
	
	std::stringstream buffer;
	buffer << file.rdbuf();

	file.close();

	std::string content(buffer.str());

	try
	{
		doc.parse<parse_full>(&content[0]);
	}
	catch(parse_error error)
	{
		//Make an array of characters to hold everything up to where the error occurred.
		//Use some clever pointer arithmetic. :P
		unsigned int size = error.where<char>() - &content[0];
		char *test = new char[size];

		memcpy(test, &content[0], size);

		unsigned int lineCount = 1;

		for(unsigned int i = 0; i < size; i++)
			if(test[i] == '\r')
				lineCount++;

		delete [] test;

		//Now we have the line the error occurred on!
		game->shout(error.what() + std::string(": on line ") + std::to_string(lineCount));

		return false;
	}


	xml_node<> *node;

	node = doc.first_node();

	int entry = 0;

	while(node != NULL)
	{
		xml_attribute<> *name, *path;

		name = node->first_attribute("name");
		path = node->first_attribute("path");

		node = node->next_sibling();

		entry++;

		if(!name)
		{
			game->shout("Entry #" + std::to_string(entry) + " missing name!\n");
			continue;
		}

		if(!path)
		{
			game->shout("Entry #" + std::to_string(entry) + " missing path!\n");
			continue;
		}

		add(name->value(), new std::string(path->value()));

		game->shout('\"' + name->value() + std::string("\" added with path: \"") + path->value() + '\"');
	}

	return true;
}

std::string* ResourceManager::get(std::string key)
{
	std::map<std::string,std::string*>::iterator it;

	it = itemMap.find(key);

	if(it == itemMap.end())
		return nullptr;
	else
		return it->second;
}

void ResourceManager::add(std::string key, std::string *item)
{
	itemMap[key] = item;
}

void ResourceManager::add(std::string key, std::string item)
{
	itemMap[key] = new std::string(item);
}

void ResourceManager::remove(std::string key)
{
	std::map<std::string,std::string*>::iterator it;

	it = itemMap.find(key);

	if(it != itemMap.end())
	{
		delete it->second;
		itemMap.erase(it);
	}
}

size_t ResourceManager::getSize()
{
	return itemMap.size();
}