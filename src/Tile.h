#ifndef TILE_H
#define TILE_H

#include "Trackable.h"

enum tile_types {TYPE_VOID, TYPE_FLOOR, TYPE_WALL, TYPE_STAIRS_DOWN, TYPE_STAIRS_UP};

struct Tile : public Trackable
{
	Tile(){type = TYPE_VOID; goodCreatureGoal = 0; goodCreatureLock = false;}
	Tile(tile_types tp){type = tp;}
	int type;
	void clearGoals(){if(goodCreatureGoal > 0) goodCreatureGoal -= 1;}
	int goodCreatureGoal;
	bool goodCreatureLock;
};

#endif