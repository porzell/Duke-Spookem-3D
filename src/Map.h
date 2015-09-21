#pragma once
#ifndef MAP_H
#define MAP_H
#include "Tile.h"
#include "Trackable.h"
#include "Game.h"
#include "Player.h"
#include "EntityManager.h"
#include "ISaveWritable.h"

enum generation_types {GENERATION_EMPTY, GENERATION_DRUNKARD, GENERATION_BSP};

enum map_themes {THEME_GRAVEYARD, THEME_DUNGEON, THEME_NAZI, THEME_TECH};

extern Game *game;

class Map : public Trackable, public ISaveWritable
{
public:
	Map();
	Map(generation_types gen, unsigned width, unsigned height, int startx = -1, int starty = -1, int level = 0, map_themes theme = THEME_DUNGEON);
	~Map();
	Tile getTile(int x, int y){if(x <= 0 || y <= 0 || x >= (int)_mpWidth || y >= (int)_mpHeight) return Tile();  return _mpTiles[x][y];}
	void diffuseGoals();
	void setGoal(int x, int y, int value){_mpTiles[x][y].goodCreatureGoal = value; /*_mpTiles[x][y].goodCreatureLock = true;*/}
	unsigned getWidth(){return _mpWidth;}
	unsigned getHeight(){return _mpHeight;}
	unsigned getUpStairsX(){return _upsx;}
	unsigned getUpStairsY(){return _upsy;}
	unsigned getDownStairsX(){return _dnsx;}
	unsigned getDownStairsY(){return _dnsy;}

	bool getCleared() { return _isCleared; };
	void setCleared(bool cleared) { _isCleared = cleared; };

	bool getVisited() { return _isVisited; };
	void setVisited(bool visited) { _isVisited = visited; };

	int getLevel() { return _level; }
	void setLevel(int level) { _level = level; }

	map_themes getTheme() { return _theme; };

	virtual void writeSave(FILE* outfile, unsigned pos);
	virtual void readSave(FILE* infile, unsigned pos);

	void draw();
	void drawSky();

	inline EntityManager* getEntityManager() { return entityManager; }
private:
	Tile** _mpTiles;
	unsigned _mpWidth;
	unsigned _mpHeight;
	unsigned _upsx, _upsy;
	unsigned _dnsx, _dnsy;
	bool _isCleared;
	bool _isVisited;

	map_themes _theme;

	int _level;

	Texture *_skyTexture;

	Texture *_wallTexture;
	Texture *_ceilingTexture;
	Texture *_floorTexture;

	EntityManager* entityManager;
};

#endif
