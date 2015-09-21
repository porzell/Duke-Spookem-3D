#pragma once
#ifndef _TEXTURE_MANAGER_H
#define _TEXTURE_MANAGER_H

#include "Texture.h"

#include "Trackable.h"

#include <string>
#include <map>

#include "Header.h"

class TextureManager : public Trackable
{
	const static std::string TYPE;

	sizeType mKey;

	protected:
		std::map<std::string, Texture*> itemMap;

	public:
		TextureManager() { mKey = 0; };
		~TextureManager();

		Texture* get(std::string key);
		void add(Texture *item);
		Texture* add(std::string key, Texture *item);
		void remove(std::string key);

		Texture* findTextureByGLuint(GLuint id);

		size_t getSize();
};

#endif