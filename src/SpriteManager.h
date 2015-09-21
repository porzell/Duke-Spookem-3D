#pragma once
#ifndef _SPRITE_MANAGER_H
#define _SPRITE_MANAGER_H

#include "Sprite.h"

#include "Trackable.h"
#include <string>
#include <map>

#include "Header.h"

class SpriteManager : public Trackable
{
	const static std::string TYPE;
	sizeType mKey;

	protected:
		std::map<std::string, Sprite*> itemMap;

	public:
		SpriteManager() { mKey = 0; };
		~SpriteManager();

		Sprite* get(std::string key);
		void add(Sprite *item);
		Sprite* add(std::string key, Sprite *item);
		void remove(std::string key);

		size_t getSize();
};

#endif