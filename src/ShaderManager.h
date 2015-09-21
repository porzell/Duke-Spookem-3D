#pragma once
#ifndef _SHADER_MANAGER_H
#define _SHADER_MANAGER_H

#include "Shader.h"

#include "Trackable.h"

#include <string>
#include <map>

#include "Header.h"

class ShaderManager : public Trackable
{
	const static std::string TYPE;

	sizeType mKey;

	protected:
		std::map<std::string, Shader*> itemMap;

	public:
		ShaderManager() { mKey = 0; };
		~ShaderManager();

		Shader* get(std::string key);
		void add(Shader *item);
		void add(std::string key, Shader *item);
		void remove(std::string key);

		size_t getSize();
};

#endif