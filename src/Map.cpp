#include "Map.h"

#include "Game.h"

#include "Texture.h"

#include "TextureManager.h"

#include "ResourceManager.h"

#include "Display.h"

extern Game *game;

Map::Map()
{
	entityManager = New EntityManager();

	_mpWidth = _mpHeight = _upsx = _upsy = _dnsx = _dnsy = 0;

	_theme = THEME_DUNGEON;

	_level = 0;

	//_skyTexture = _wallTexture = _ceilingTexture = _floorTexture = NULL;

	_skyTexture = game->getTextureManager()->get("redbrick_texture");
	_wallTexture = game->getTextureManager()->get("redbrick_texture");
	_ceilingTexture = game->getTextureManager()->get("redbrick_texture");
	_floorTexture = game->getTextureManager()->get("redbrick_texture");

	_isCleared = false;
	_isVisited = false;
}

Map::Map(generation_types gen, unsigned width, unsigned height, int startx, int starty, int level, map_themes theme) : _mpWidth(width), _mpHeight(height), _level(level), _theme(theme)
{
	entityManager = New EntityManager();
	//init the tilemap
	_mpTiles = new Tile*[width];
	for(unsigned i = 0; i < width; ++i)
		_mpTiles[i] = New Tile[height];
	for(unsigned i = 0; i < width; ++i)
		for(unsigned j = 0; j < height; ++j)
			_mpTiles[i][j].type = TYPE_WALL;

	unsigned seed_x = startx == -1 ? rand() % (width  - 4) + 2 : startx;
	unsigned seed_y = startx == -1 ? rand() % (height - 4) + 2 : starty;
	_mpTiles[seed_x][seed_y].type = TYPE_STAIRS_UP;
	_upsx = seed_x;
	_upsy = seed_y;

	switch(gen)
	{
	case GENERATION_EMPTY:
		for(unsigned i = 1; i < width - 1; ++i)
			for(unsigned j = 1; j < height - 1; ++j)
				if(_mpTiles[i][j].type == TYPE_WALL)
					_mpTiles[i][j].type = TYPE_FLOOR;
		break;
	case GENERATION_DRUNKARD:
		{
		unsigned dig_count = width * height / 3;
		
		while(dig_count > 0){
			unsigned direction = rand() % 4;
			if(direction == 0 && seed_x > 2)
				seed_x--;
			else if(direction == 1 && seed_x < (width - 3))
				seed_x++;
			else if(direction == 2 && seed_y > 2)
				seed_y--;
			else if(direction == 3 && seed_y < (height - 3))
				seed_y++;

			if(_mpTiles[seed_x][seed_y].type == TYPE_WALL){
				_mpTiles[seed_x][seed_y].type = TYPE_FLOOR;
				--dig_count;
			}
		}
		}
		break;
	case GENERATION_BSP:
		break;
	}
	unsigned x = rand() % width;
	unsigned y = rand() % height;

	while(_mpTiles[x][y].type != TYPE_FLOOR)
	{
		x = rand() % width;
		y = rand() % height;
	}

	_mpTiles[x][y].type = TYPE_STAIRS_DOWN;
	_dnsx = x;
	_dnsy = y;

	_isCleared = false;
	_isVisited = false;

	switch(theme)
	{
		case THEME_GRAVEYARD:
		{
			_skyTexture = game->getTextureManager()->get("sunset_sky");
			_floorTexture = game->getTextureManager()->get("grass_texture");
			_ceilingTexture = game->getTextureManager()->get("brownbrick_texture");
			_wallTexture = game->getTextureManager()->get("redbrick_texture");
			break;
		}
		case THEME_DUNGEON:
		{
			_skyTexture = game->getTextureManager()->get("sunset_sky");

			switch(rand() % 6)
			{
			case 0:
				_floorTexture = game->getTextureManager()->get("brownbrick_texture");
				break;
			case 1:
				_floorTexture = game->getTextureManager()->get("dirt_texture");
				break;
			case 2:
				_floorTexture = game->getTextureManager()->get("tile1_texture");
				break;
			case 3:
				_floorTexture = game->getTextureManager()->get("tile2_texture");
				break;
			case 4:
				_floorTexture = game->getTextureManager()->get("cobblestone_texture");
				break;
			default:
				_floorTexture = game->getTextureManager()->get("tile2_texture");
				break;
			}

			switch(rand() % 5)
			{
			case 0:
				_ceilingTexture = game->getTextureManager()->get("brownbrick_texture");
				break;
			case 1:
				_ceilingTexture = game->getTextureManager()->get("dirt_texture");
				break;
			case 2:
				_ceilingTexture = game->getTextureManager()->get("tile1_texture");
				break;
			case 3:
				_ceilingTexture = game->getTextureManager()->get("tile2_texture");
				break;
			case 4:
				_ceilingTexture = game->getTextureManager()->get("cobblestone_texture");
				break;
			default:
				_ceilingTexture = game->getTextureManager()->get("tile2_texture");
				break;
			}

			switch(rand() % 6)
			{
			case 0:
				_wallTexture = game->getTextureManager()->get("brownbrick_texture");
				break;
			case 1:
				_wallTexture = game->getTextureManager()->get("dirt_texture");
				break;
			case 2:
				_wallTexture = game->getTextureManager()->get("tile1_texture");
				break;
			case 3:
				_wallTexture = game->getTextureManager()->get("tile2_texture");
				break;
			case 4:
				_wallTexture = game->getTextureManager()->get("cobblestone_texture");
				break;
			case 5:
				_wallTexture = game->getTextureManager()->get("redbrick_texture");
				break;
			default:
				_wallTexture = game->getTextureManager()->get("tile2_texture");
				break;
			}
			break;
		}

		case THEME_NAZI:
		{
			_skyTexture = game->getTextureManager()->get("sunset_sky");

			switch(rand() % 5)
			{
			case 0:
				_floorTexture = game->getTextureManager()->get("brownbrick_texture");
				break;
			case 1:
				_floorTexture = game->getTextureManager()->get("wolftile_texture");
				break;
			case 2:
				_floorTexture = game->getTextureManager()->get("metalrivetwall_texture");
				break;
			case 3:
				_floorTexture = game->getTextureManager()->get("whitebrick_texture");
				break;
			case 4:
				_floorTexture = game->getTextureManager()->get("cobblestone_texture");
				break;
			default:
				_floorTexture = game->getTextureManager()->get("cobblestone_texture");
				break;
			}

			switch(rand() % 5)
			{
			case 0:
				_ceilingTexture = game->getTextureManager()->get("brownbrick_texture");
				break;
			case 1:
				_ceilingTexture = game->getTextureManager()->get("dirt_texture");
				break;
			case 2:
				_ceilingTexture = game->getTextureManager()->get("tile1_texture");
				break;
			case 3:
				_ceilingTexture = game->getTextureManager()->get("tile2_texture");
				break;
			case 4:
				_ceilingTexture = game->getTextureManager()->get("cobblestone_texture");
				break;
			default:
				_ceilingTexture = game->getTextureManager()->get("tile2_texture");
				break;
			}

			switch(rand() % 5)
			{
			case 0:
				_wallTexture = game->getTextureManager()->get("brownbrick_texture");
				break;
			case 1:
				_wallTexture = game->getTextureManager()->get("bluebrick_texture");
				break;
			case 2:
				_wallTexture = game->getTextureManager()->get("railwall_texture");
				break;
			case 3:
				_wallTexture = game->getTextureManager()->get("metalrivetwall_texture");
				break;
			case 4:
				_wallTexture = game->getTextureManager()->get("cobblestone_texture");
				break;
			default:
				_wallTexture = game->getTextureManager()->get("metalrivetwall_texture");
				break;
			}
			break;
		}

		case THEME_TECH:
		{
			_skyTexture = game->getTextureManager()->get("tech_sky");

			_wallTexture = game->getTextureManager()->get("grid");
			_ceilingTexture = _wallTexture;
			_floorTexture = _wallTexture;

			break;
		}

		default:
		{
			_skyTexture = game->getTextureManager()->get("tech_sky");

			_wallTexture = game->getTextureManager()->get("grid");
			_ceilingTexture = _wallTexture;
			_floorTexture = _wallTexture;

			break;
		}
	}
}

Map::~Map()
{
	if(entityManager)
		delete entityManager;

	if(_mpTiles)
	{
		for(unsigned i = 0; i < _mpWidth; ++i)
		{
			delete _mpTiles[i];
			_mpTiles[i] = NULL;
		}
		delete [] _mpTiles;
	}
}

void Map::diffuseGoals()
{

	for(unsigned i = 0; i < _mpWidth; ++i)
	{
		for(unsigned j = 1; j < _mpHeight - 1; ++j)
		{
			_mpTiles[i][j].clearGoals();
			if(_mpTiles[i][j].type == TYPE_STAIRS_DOWN)
				_mpTiles[i][j].goodCreatureGoal = 50;
		}
	}
	Vec3d position = game->getPlayer()->getPosition();
	_mpTiles[int(position.X + 0.5)][int(position.Z + 0.5)].goodCreatureGoal = game->getPlayer()->getSneaking() ? 60 : 100;

	for(unsigned i = 1; i < _mpWidth - 1; ++i)
	{
		for(unsigned j = 1; j < _mpHeight - 1; ++j)
		{
			if(_mpTiles[i][j].goodCreatureLock == false)
				for(int k = -1; k < 2; ++k){
					for(int l = -1; l < 2; ++l){
						if(_mpTiles[i + k][j + l].goodCreatureGoal > _mpTiles[i][j].goodCreatureGoal + 2 && _mpTiles[i][j].type != TYPE_WALL){
							_mpTiles[i][j].goodCreatureGoal = _mpTiles[i + k][j + l].goodCreatureGoal - (_mpTiles[i + k][j + l].goodCreatureGoal/20 + 1);
						}
					}
				}
			else
				_mpTiles[i][j].goodCreatureLock = false;
		}
	}
}

void Map::writeSave(FILE* outfile, unsigned pos)
{
   /* To save:
   * Level, uint DONE
   * Type, uint DONE
	* Width, uint DONE
	* Height, uint DONE
	* Up x/y, 2 uints DONE
	* Down x/y, 2 uints DONE
	* Sky texture ID, uint DONE
	* Wall texture ID, uint DONE
	* Floor texture ID, uint DONE
	* Ceiling texture ID, uint DONE
	* Tiles, Width*Height uchars  DONE
	* EntityManager, special  DONE
	*/

	int sky = _skyTexture->getTextureID();
	int wall = _wallTexture->getTextureID();
	int floor = _floorTexture->getTextureID();
	int ceiling = _ceilingTexture->getTextureID();

	fwrite(&_level, sizeof(int), 1, outfile);
	fwrite(&_theme, sizeof(map_themes), 1, outfile);

	fwrite(&_isCleared, sizeof(bool), 1, outfile);
	fwrite(&_isVisited, sizeof(bool), 1, outfile);

	fwrite(&_mpWidth, sizeof(int), 1, outfile);
	fwrite(&_mpHeight, sizeof(int), 1, outfile);
	fwrite(&_upsx, sizeof(int), 1, outfile);
	fwrite(&_upsy, sizeof(int), 1, outfile);
	fwrite(&_dnsx, sizeof(int), 1, outfile);
	fwrite(&_dnsy, sizeof(int), 1, outfile);

	fwrite(&sky, sizeof(int), 1, outfile);
	fwrite(&wall, sizeof(int), 1, outfile);
	fwrite(&floor, sizeof(int), 1, outfile);
	fwrite(&ceiling, sizeof(int), 1, outfile);

	//fputc(_level, outfile);

	for(unsigned i = 0; i < _mpWidth; ++i)
		for(unsigned j = 0; j < _mpHeight; ++j)
			fwrite(&_mpTiles[i][j].type, sizeof(int), 1, outfile);
	entityManager->writeSave(outfile, pos);
}

void Map::readSave(FILE* infile, unsigned pos)
{
	fread(&_level, sizeof(int), 1, infile);
	fread(&_theme, sizeof(map_themes), 1, infile);

	fread(&_isCleared, sizeof(bool), 1, infile);
	fread(&_isVisited, sizeof(bool), 1, infile);

	fread(&_mpWidth, sizeof(int), 1, infile);
	fread(&_mpHeight, sizeof(int), 1, infile);
	fread(&_upsx, sizeof(int), 1, infile);
	fread(&_upsy, sizeof(int), 1, infile);
	fread(&_dnsx, sizeof(int), 1, infile);
	fread(&_dnsy, sizeof(int), 1, infile);

	int sky, wall, floor, ceiling;
	fread(&sky, sizeof(int), 1, infile);
	fread(&wall, sizeof(int), 1, infile);
	fread(&floor, sizeof(int), 1, infile);
	fread(&ceiling, sizeof(int), 1, infile);

	_skyTexture = game->getTextureManager()->findTextureByGLuint(sky);

	_wallTexture = game->getTextureManager()->findTextureByGLuint(wall);
	_floorTexture = game->getTextureManager()->findTextureByGLuint(floor);
	_ceilingTexture = game->getTextureManager()->findTextureByGLuint(ceiling);

	//_level = fgetc(infile);

	_mpTiles = new Tile*[_mpWidth];
	for(unsigned i = 0; i < _mpWidth; ++i){
		_mpTiles[i] = New Tile[_mpHeight];
		for(unsigned j = 0; j < _mpHeight; ++j)
			fread(&_mpTiles[i][j].type, sizeof(int), 1, infile);
	}

	if(entityManager)
	{
		delete entityManager;
	}
	entityManager = New EntityManager();
	entityManager->readSave(infile, pos);
	//entityManager->readSave(infile, pos);
}

void Map::drawSky()
{
	static float scroll = 0.0f;

	glDisable( GL_CULL_FACE );

	glDisable( GL_DEPTH_TEST );

	//*********************
	//Draw Sky:
	//*********************

	//shaderManager->get("shader")->useShader();

	_skyTexture->setAsActiveTexture();

	//glClear(GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	Vec2d bounds = game->getDisplay()->getDimensions();

	gluOrtho2D(0,1,-1,1);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	//glTranslatef(-1.0f,0,-1.0f);

	glBegin(GL_QUADS);

	glColor4f(1,1,1,1);


	float newScroll = scroll + game->getPlayer()->getAngle().X/360;
			
	//GLfloat angle = mpPlayer->getVelocity().X;

	glTexCoord2f(0 + newScroll,1);
	//glTexCoord2f(0 + angle,1);
	glVertex3f(0.0f, 1.0f, 0.0f);              // Top Left

	glTexCoord2f(1.0f/_skyTexture->getRatio() + newScroll,1);
	//glTexCoord2f(1 + angle,1);
	glVertex3f(1.0f, 1.0f, 0.0f);              // Top Right

	glTexCoord2f(1.0f/_skyTexture->getRatio() + newScroll,0);
	//glTexCoord2f(1 + angle,0);
	glVertex3f(1.0f,-1.0f, 0.0f);              // Bottom Right

	glTexCoord2f(0 + newScroll,0);
	//glTexCoord2f(0 + angle,0);
	glVertex3f(0.0f,-1.0f, 0.0f);              // Bottom Left

	glEnd();

	glClear(GL_DEPTH_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D);
	glEnable( GL_DEPTH_TEST );

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glEnable( GL_CULL_FACE );

	scroll += 0.0005f;
}

void Map::draw()
{

	if(_level == 0)
		drawSky();

	//lpos.clear();

	static float scroll = 0.0f;
	static float alpha = 1.0f;

	static bool alphaUp = false;

	unsigned w = getWidth();
	unsigned h = getHeight();

	/*Texture *floorTex = textureManager->get("floor");
	Texture *downstairsTex = textureManager->get("downstairs");
	Texture *upstairsTex = textureManager->get("upstairs");
	Texture *ceilingTex = textureManager->get("dirty");
	Texture *wallTex = textureManager->get("wall");*/

	Texture *downstairsTex = game->getTextureManager()->get("downstairs");
	Texture *upstairsTex = game->getTextureManager()->get("upstairs");

	Texture *normalTex = game->getTextureManager()->get("normal_texture");
	Texture *heightTex = game->getTextureManager()->get("height_texture");

	scroll += 0.0025f;

	if(alpha >= 1.0f)
		alphaUp = false;
	else if(alpha <= 0.0)
		alphaUp = true;

	if(alphaUp)
		alpha += 0.005f;
	else
		alpha -= 0.005f;

	if(_theme != THEME_TECH)
		scroll = 0;

	for(unsigned i = 0; i < w; ++i)
	{
		for(unsigned j = 0; j < h; ++j){
			//glColor4f(1.0f - float(map->getTile(i, j).goodCreatureGoal)/150.0f, 1.0f - float(map->getTile(i, j).goodCreatureGoal)/150.0f, 1.0f - float(map->getTile(i, j).goodCreatureGoal)/150.0f, 1);
			glPushMatrix();
			glTranslatef((float)i, 0, (float)j);

			//cwc::glShader *shader = (*game->getShaderManager())[0];

			//shader->begin();

			/*shader->setUniform1i("toggletexture", 0);
			shader->setUniform1i("togglebump", 1);
			shader->setUniform1f("parallaxheight", -0.5f);*/

			//normalTex->setAsActiveTexture();
			//shader->setUniform1i("normalMap", 0);
			//shader->setUniform1i("my_color_texture[1]", 0);
			/*heightTex->setAsActiveTexture();
			shader->setUniform1i("my_color_texture[2]", heightTex->getTextureID());
			shader->setUniform1i("my_color_texture[0]", 0);*/

			switch(getTile(i, j).type)
			{
			case TYPE_STAIRS_UP:
			case TYPE_STAIRS_DOWN:
			case TYPE_FLOOR:
			{
				if(getTile(i, j).type == TYPE_FLOOR)
					_floorTexture->setAsActiveTexture();
				else if(getTile(i, j).type == TYPE_STAIRS_DOWN)
					downstairsTex->setAsActiveTexture();
				else if(getTile(i, j).type == TYPE_STAIRS_UP)
					upstairsTex->setAsActiveTexture();

				//Floor
				glBegin(GL_QUADS);
				if(_theme == THEME_TECH)
					glColor3f(alpha,0.0f,1.0f-alpha);
				glTexCoord2f(0 + scroll, 0 + scroll);
				glVertex3f(0, -1, 0);
				glTexCoord2f(1 + scroll, 0 + scroll);
				glVertex3f(0, -1, 1);
				glTexCoord2f(1 + scroll, 1 + scroll);
				glVertex3f(1, -1, 1);
				glTexCoord2f(0 + scroll, 1 + scroll);
				glVertex3f(1, -1, 0);
				glEnd();
				
				glColor3f(1,1,1);

				//Ceiling

				if(_level != 0  && getTile(i, j).type != TYPE_STAIRS_UP)
				{
					//glBindTexture(GL_TEXTURE_2D, textureManager->get("floor")->getTextureID());
					_ceilingTexture->setAsActiveTexture();
					glBegin(GL_QUADS);
					if(_theme == THEME_TECH)
						glColor3f(alpha,0.0f,1.0f-alpha);
					glTexCoord2f(0, 0 + scroll);
					glVertex3f(0, 1, 0);
					glTexCoord2f(1, 0 + scroll);
					glVertex3f(1, 1, 0);
					glTexCoord2f(1, 1 + scroll);
					glVertex3f(1, 1, 1);
					glTexCoord2f(0, 1 + scroll);
					glVertex3f(0, 1, 1);
					glEnd();
				}
			}
				break;
			case TYPE_WALL:
				_wallTexture->setAsActiveTexture();

				glBegin(GL_QUADS);

				if(_theme == THEME_TECH)
					glColor3f(1.0f-alpha,0.0f,alpha);

				if(getTile(i, j - 1).type != TYPE_WALL)
				{
					glTexCoord2f(0, 0 + scroll);
					glVertex3f(0, -1, 0);
					glTexCoord2f(0, 1 + scroll);
					glVertex3f(0, 1, 0);
					glTexCoord2f(1, 1 + scroll);
					glVertex3f(1, 1, 0);
					glTexCoord2f(1, 0 + scroll);
					glVertex3f(1, -1, 0);
				}
				if(getTile(i - 1, j).type != TYPE_WALL)
				{
					glTexCoord2f(0, 0 + scroll);
					glVertex3f(0, -1, 0);
					glTexCoord2f(1, 0 + scroll);
					glVertex3f(0, -1, 1);
					glTexCoord2f(1, 1 + scroll);
					glVertex3f(0, 1, 1);
					glTexCoord2f(0, 1 + scroll);
					glVertex3f(0, 1, 0);
				}
				if(getTile(i, j + 1).type != TYPE_WALL)
				{
					glTexCoord2f(0, 0 + scroll);
					glVertex3f(0, -1, 1);
					glTexCoord2f(1, 0 + scroll);
					glVertex3f(1, -1, 1);
					glTexCoord2f(1, 1 + scroll);
					glVertex3f(1, 1, 1);
					glTexCoord2f(0, 1 + scroll);
					glVertex3f(0, 1, 1);
				}
				if(getTile(i + 1, j).type != TYPE_WALL)
				{
					glTexCoord2f(0, 0 + scroll);
					glVertex3f(1, -1, 0);
					glTexCoord2f(0, 1 + scroll);
					glVertex3f(1, 1, 0);
					glTexCoord2f(1, 1 + scroll);
					glVertex3f(1, 1, 1);
					glTexCoord2f(1, 0 + scroll);
					glVertex3f(1, -1, 1);
				}
				glEnd();
				
				break;
			}

			//shader->setUniform1i("colorMap", 0);
			//shader->setUniform1i("invRadius", 0.5);
			//shader->end();

			glPopMatrix();
		}
	}

	/*for(auto it = lpos.begin(); it != lpos.end(); ++it)
	{
		//drawBillboard("ladder",(*it));
		textureManager->get("ladder")->setAsActiveTexture();
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex3f(0, -1, 0);
		glTexCoord2f(1, 0);
		glVertex3f(0, 1, 0);
		glTexCoord2f(1, 1);
		glVertex3f(1, 1, 0);
		glTexCoord2f(0, 1);
		glVertex3f(1, -1, 0);
		glEnd();
	}*/
}