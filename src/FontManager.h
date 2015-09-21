#pragma once
#ifndef _FONT_MANAGER_H
#define _FONT_MANAGER_H

#include "Font.h"

#include "Trackable.h"
#include <string>
#include <map>

#include "Header.h"

class FontManager : public Trackable
{
	const static std::string TYPE;

	sizeType mKey;

	protected:
		std::map<std::string, Font*> itemMap;

	public:
		FontManager() { mKey = 0; };
		~FontManager();

		Font* get(std::string key);
		void add(Font *item);
		void add(std::string key, Font *item);
		void remove(std::string key);

		size_t getSize();
};

#endif