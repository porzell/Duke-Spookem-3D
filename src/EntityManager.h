#pragma once
#ifndef _ENTITY_MANAGER_H
#define _ENTITY_MANAGER_H

#include "Header.h"

#include "Trackable.h"

#include <string>
#include <unordered_map>
#include "ISaveWritable.h"

class Entity;

class EntityManager : public Trackable, public ISaveWritable
{
	const static std::string TYPE;

	bool mPaused;

	sizeType mKey;

	protected:
		std::unordered_map<std::string, Entity*> itemMap;

	public:
		EntityManager();
		virtual ~EntityManager();

		void clear();

		Entity* get(std::string key);
		std::string add(Entity *item);
		void add(std::string key, Entity *item);
		void remove(std::string key, bool true_delete);

		void quiet();

		void unQuiet();

		void runCollisions();

		size_t getSize();

		//Draws all entities.
		void draw();

		//Runs update for all entities.
		void think(const double elapsedTime);

		//Handles incoming events from eventsystem.
		//void handleEvent( const Event& theEvent );

		virtual void writeSave(FILE* outfile, unsigned pos);
		virtual void readSave(FILE* infile, unsigned pos);

		friend class Monster;
};

#endif
