#pragma once
#ifndef _ANIMATION_MANAGER_H
#define _ANIMATION_MANAGER_H

#include "Animation.h"

#include "Trackable.h"
#include <string>
#include <map>

#include "Header.h"

class AnimationManager : public Trackable
{
	const static std::string TYPE;

	sizeType mKey;

	protected:
		std::map<std::string, Animation*> itemMap;

	public:
		AnimationManager() { mKey = 0; };
		~AnimationManager();

		Animation* get(std::string key);
		void add(Animation *item);
		Animation* add(std::string key, Animation *item);
		void remove(std::string key);

		size_t getSize();
};

#endif