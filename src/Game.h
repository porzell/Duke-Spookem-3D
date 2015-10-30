#ifndef _GAME_H
#define _GAME_H

#include "GLincludes.h"

//#define DEBUG_LOG_WINDOW

//#include "Timer.h"

#include "Trackable.h"

#include "ISaveWritable.h"

//#include "TextureManager.h"
//#include "AnimationManager.h"
//#include "EntityManager.h"
//#include "SpriteManager.h"
//#include "KeyManager.h"
//#include "SampleManager.h"
//#include "FontManager.h"
//#include "OverlayManager.h"
//#include "PathManager.h"
//#include "MenuStack.h"

//#include "EventSystem.h"
#include "EventListener.h"

#include "Header.h"
#include "VectorMath.h"
//#include <irrKlang.h>

class InputManager;
class AudioSystem;
class Display;
class Player;
class Map;
class SoundEngine;
class TextureManager;
class ShaderManager;
class SpriteManager;
class AnimationManager;
class EntityManager;
class FontManager;
class ResourceManager;
class MenuStack;

//Update variable for 60fps.	
//const double MS_PER_UPDATE = 1.0 / .06;

//Constant for FPS:
const double DEFAULT_FRAMES_PER_SECOND = 60.0;
const int DEFAULT_DISPLAY_WIDTH = 800;
const int DEFAULT_DISPLAY_HEIGHT = 600;

class Game : public EventListener, public ISaveWritable
{
	private:

		//Sample *mpMusic;

		//This timer starts when dancing begins.
		//Timer mDanceTimer;

		InputManager *mpInput;
        SoundEngine *mpSoundEngine;

		Player *mpPlayer;

		Vec2d mMousePosition;


		TextureManager *textureManager;
		SpriteManager *spriteManager;
		AnimationManager *animationManager;
		EntityManager *entityManager;
		FontManager *fontManager;

		ResourceManager *resourceManager;

		MenuStack *menuStack;

		Map **mpMap;
		
		unsigned current_map;

		float mHardness;

		/*TextureManager *bitmapManager;
		AnimationManager *animationManager;
		SpriteManager *spriteManager;
		EntityManager *entityManager;
		KeyManager *keyManager;
		SampleManager *sampleManager;
		FontManager *fontManager;
		OverlayManager *overlayManager;
		PathManager *pathManager;
		MenuStack *menuStack;*/

		AudioSystem *mpAudio;
		Display *mpDisplay;

        std::string asscover = "Pe-alpha footage. All assets are placeholders used under Fair Use, and are copyright to their respective owners";

		//Initializes the game resources.
		void initResources();

		//This determines whether the gameloop ends.
		bool mShouldEnd;

		//This determines whether draw calls are made.
		bool mShouldDraw;

		//This determines whether to allow giblets.
		bool mAllowGibs;

		//This shows whether the game has started.
		bool mGameStarted;

		//This holds the elapsed time in each successive gameloop iteration.
		double mElapsedTime;

		//void handleEvent( const Event& theEvent );

		bool mPaused;

		//Show wireframe bounding box mode.
		bool mWireframeBoundMode;

	public:
		Game();
		~Game();

#ifdef DEBUG_LOG_WINDOW
		DebugLogDisplay *mpLog;
#endif
		std::string getOpenFile();
		std::string getSaveFile();

		void save(std::string s = "");
		bool load();

		//Initializes members.
		void init();

		//Cleans up.
		void cleanup();

		//Displays a debug message.
		void shout(std::string out);

		//Runs game loop until stopped.
		void doLoop();

		void drawBillboard(std::string texture, Vec3d pos);

		void interact();

		void populateMap(int level);

		inline Display* getDisplay() { return mpDisplay; } 
		inline InputManager* getInput() { return mpInput; }
		inline Player* getPlayer() { return mpPlayer; }

		inline MenuStack* getMenuStack() { return menuStack; }

		inline SoundEngine* getSoundEngine() { return mpSoundEngine; }
		inline TextureManager* getTextureManager() { return textureManager; }
		inline Map* getCurrentMap() { return mpMap[current_map]; }
		inline AnimationManager* getAnimationManager() { return animationManager; }
		inline SpriteManager* getSpriteManager() { return spriteManager; }
		EntityManager* getEntityManager();
		inline ResourceManager* getResourceManager() { return resourceManager; }
		inline FontManager* getFontManager() { return fontManager; }

		float getHardness(){return mHardness;}
		void setHardness(float h){mHardness = h;}

		inline bool hasGameStarted() { return mGameStarted; }
		inline bool isEnding() { return mShouldEnd; }

		virtual void handleEvent( const Event& theEvent);

		std::string player_key;
		/*inline KeyManager* getKeyManager() { return keyManager; };
		inline SampleManager* getSampleManager() { return sampleManager; };
		inline FontManager* getFontManager() { return fontManager; };
		inline OverlayManager* getOverlayManager() { return overlayManager; };
		inline PathManager* getPathManager() { return pathManager; };
		inline MenuStack* getMenuStack() { return menuStack; };

		//Get a pointer to the dance timer.
		inline Timer* getDanceTimer() { return &mDanceTimer; };

		//Return a pointer to the current music.
		inline Sample *getMusic() { return mpMusic; };

		//Return whether music is allowed.
		inline bool allowMusic() { return mAllowMusic; };

		//Return whether debug mode is enabled.
		inline bool isDebugMode() { return mIsDebugMode; };
		

		//Toggle whether music is allowed.
		inline void toggleMusic() 
		{
			mAllowMusic = !mAllowMusic;

			//if(!mAllowMusic)
			//{
			//	if(mpMusic)
			//		mpMusic->stop();
			//}
			//else
			//{
			//	if(mpMusic)
			//		mpMusic->play();
			//}
		};

		//Toggle whether debug mode is enabled.
		inline void toggleDebugMode() { mIsDebugMode = !mIsDebugMode; };

		//Returns whether game is paused.
		inline bool isPaused() { return mPaused; };*/

		inline bool getAllowGibs() { return mAllowGibs; }
		inline void toggleAllowGibs() { mAllowGibs = !mAllowGibs; }

		inline bool getWireframeBoundingBoxMode() { return mWireframeBoundMode; }
		inline void setWireframeBoundingBoxMode(bool set) { mWireframeBoundMode = set; }

		inline void toggleWireframeBoundingBoxMode() { mWireframeBoundMode = !mWireframeBoundMode; }

		void cleanupMaps();
		void createNewGame();

		void drawHUD();

		void playMapMusic();

		virtual void writeSave(FILE* outfile, unsigned pos);
		virtual void readSave(FILE* infile, unsigned pos);

		friend class Input;
		friend class Display;
};

#endif
