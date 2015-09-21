#pragma once
#ifndef _RESOURCE_MANAGER_H
#define _RESOURCE_MANAGER_H

#include "Trackable.h"
#include <string>
#include <map>

#include "Header.h"

class ResourceManager : public Trackable
{
	const static std::string TYPE;

	sizeType mKey;

	protected:
		std::map<std::string, std::string*> itemMap;

	public:
		ResourceManager() { mKey = 0; };
		~ResourceManager();

		std::string* get(std::string key);

		//Load resource paths from resource file.
		bool load(std::string path);

		void add(std::string key, std::string *item);
		void add(std::string key, std::string item);
		void remove(std::string key);

		size_t getSize();
};

#endif