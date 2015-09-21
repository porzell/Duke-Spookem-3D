#include "Game.h"

//#include "Entity.h"
//#include "Animation.h"

#include "Display.h"
#include "InputManager.h"
#include "TextureManager.h"
#include "ShaderManager.h"
#include "SpriteManager.h"
#include "AnimationManager.h"
#include "EntityManager.h"
#include "FontManager.h"
#include "ResourceManager.h"

#include "MenuStack.h"

#include "Entity.h"

#include "Map.h"

#include "SoundEngine.h"

#include "Player.h"

#include "Timer.h"

#include "Monster.h"

#include "Minotaur.h"
#include "Zombie.h"
#include "SwampMonster.h"
#include "Cyborg.h"
#include "Nazi.h"
#include "Hitler.h"

#include "Utils.h"

#include <Windows.h>
#include <gl/GLU.h>

#include "EventSystem.h"

#include "EventQuit.h"

#include "MainMenu.h"
#include "StoryMenu.h"

#include "EventMenuPop.h"

#include <ctime>

const float DEFAULT_FRAME_TIME = (1.0f / 7.5f) * 1000.0f;

const double SIXTYFPS_FRAME_TIME = (1.0f / 60.0f) * 1000.0f;

Game::Game()
{
	mShouldEnd = false;
	mShouldDraw = false;
	mElapsedTime = 0.0;
	mHardness = 1;

	mAllowGibs = true;

	mpPlayer = NULL;
	mpMap = NULL;

	resourceManager = New ResourceManager();
	textureManager = New TextureManager();
	shaderManager = new cwc::glShaderManager();
	animationManager = New AnimationManager();
	spriteManager = New SpriteManager();
	fontManager = New FontManager();
	//entityManager = New EntityManager();
	/*overlayManager = New OverlayManager();
	keyManager = New KeyManager();
	sampleManager = New SampleManager();
	fontManager = New FontManager();
	pathManager = New PathManager();
	menuStack = New MenuStack();*/

	mWireframeBoundMode = false;

	gpEventSystem->addListener(EVENT_QUIT, this);
	gpEventSystem->addListener(EVENT_GAMELOSE, this);
	gpEventSystem->addListener(EVENT_LOAD, this);
	gpEventSystem->addListener(EVENT_SAVE, this);
	gpEventSystem->addListener(EVENT_NEW, this);

	mGameStarted = false;
}

Game::~Game()
{
	delete menuStack;

	delete textureManager;
	delete shaderManager;
	delete spriteManager;
	delete animationManager;
	delete fontManager;
	//delete entityManager;

	cleanupMaps();

	delete mpPlayer;

	delete resourceManager;
	/*delete fontManager;
	delete entityManager;
	delete overlayManager;
	delete keyManager;
	delete pathManager;
	delete sampleManager;
	delete menuStack;*/

	cleanup();
}

void Game::cleanup()
{
	//mpDisplay->cleanup();

	//delete mpAudio;
	delete mpDisplay;
	delete mpInput;
	delete mpSoundEngine;
}

void Game::init()
{
	try
	{
		//TODO: Init. stuff here!
	}
	catch(char * error)
	{
		shout(error);
	}

	//mpAudio = New AudioSystem();
	mpDisplay = New Display();

	//mpInput = New Input();

	//mpMusic = NULL;

	mPaused = false;

	mpDisplay->init(1366, 768, "hello", false);
	mpDisplay->setTitle("Duke Spookum 3D");

	//Trap the cursor.
	mpDisplay->shouldTrapCursor(true);


	mpInput = New InputManager();

	mpSoundEngine = New SoundEngine();

	menuStack = New MenuStack();

	initResources();
}

void Game::initResources()
{
	resourceManager->load("resources.xml");

	textureManager->add("dukespookem", New Texture(*resourceManager->get("dukespookem")));

	textureManager->add("redbrick_texture", New Texture(*resourceManager->get("redbrick_texture")));
	textureManager->add("grass_texture", New Texture(*resourceManager->get("grass_texture")));
	textureManager->add("bluebrick_texture", New Texture(*resourceManager->get("bluebrick_texture")));
	textureManager->add("brownbrick_texture", New Texture(*resourceManager->get("brownbrick_texture")));
	textureManager->add("whitebrick_texture", New Texture(*resourceManager->get("whitebrick_texture")));
	textureManager->add("railwall_texture", New Texture(*resourceManager->get("railwall_texture")));
	textureManager->add("metalrivetwall_texture", New Texture(*resourceManager->get("metalrivetwall_texture")));
	textureManager->add("dirt_texture", New Texture(*resourceManager->get("dirt_texture")));

	textureManager->add("tile1_texture", New Texture(*resourceManager->get("tile1_texture")));
	textureManager->add("tile2_texture", New Texture(*resourceManager->get("tile2_texture")));
	textureManager->add("wolftile_texture", New Texture(*resourceManager->get("wolftile_texture")));

	textureManager->add("cobblestone_texture", New Texture(*resourceManager->get("cobblestone_texture")));

	textureManager->add("downstairs", New Texture(*resourceManager->get("stair_down_texture")));
	textureManager->add("upstairs", New Texture(*resourceManager->get("stair_up_texture")));

	textureManager->add("sunset_sky", New Texture(*resourceManager->get("sunset_sky")));
	textureManager->add("tech_sky", New Texture(*resourceManager->get("tech_sky")));

	textureManager->add("legs", New Texture(*resourceManager->get("legs_texture")));

	Texture *swampmonster = New Texture(*resourceManager->get("swampmonster_texture"));
	Texture *minotaur = New Texture(*resourceManager->get("minotaur_texture"));
	Texture *zombie = New Texture(*resourceManager->get("zombie_texture"));
	Texture *guard = New Texture(*resourceManager->get("guard_texture"));
	Texture *officer = New Texture(*resourceManager->get("officer_texture"));

	Texture *fireball = New Texture(*resourceManager->get("fireball_texture"));
	Texture *shot = New Texture(*resourceManager->get("shot_texture"));
	Texture *sunglasses = New Texture(*resourceManager->get("sunglasses_texture"));
	Texture *iceball = New Texture(*resourceManager->get("iceball_texture"));


	textureManager->add("swamp_monster", swampmonster);
	textureManager->add("minotaur", minotaur);
	textureManager->add("zombie", zombie);
	textureManager->add("guard", guard);
	textureManager->add("officer", officer);
	
	textureManager->add("fireball", fireball);
	textureManager->add("shot", shot);

	textureManager->add("sunglasses", sunglasses);
	textureManager->add("iceball", iceball);

	spriteManager->add("sunglasses", New Sprite(sunglasses, Vec2d(0,0), sunglasses->getSizeVec2d()));
	spriteManager->add("iceball", New Sprite(iceball, Vec2d(0,0), iceball->getSizeVec2d()));
	
	spriteManager->add("guard_walk1", New Sprite(guard, Vec2d(0,64), Vec2d(64,64)));
	spriteManager->add("guard_walk2", New Sprite(guard, Vec2d(0,128), Vec2d(64,64)));
	spriteManager->add("guard_walk3", New Sprite(guard, Vec2d(0,192), Vec2d(64,64)));
	spriteManager->add("guard_walk4", New Sprite(guard, Vec2d(64,0), Vec2d(64,64)));
	
	spriteManager->add("guard_shoot1", New Sprite(guard, Vec2d(64,192), Vec2d(64,64)));
	spriteManager->add("guard_shoot2", New Sprite(guard, Vec2d(192,0), Vec2d(64,64)));
	spriteManager->add("guard_shoot3", New Sprite(guard, Vec2d(64,64), Vec2d(64,64)));

	spriteManager->add("guard_pain1", New Sprite(guard, Vec2d(128,64), Vec2d(64,64)));
	spriteManager->add("guard_pain2", New Sprite(guard, Vec2d(128,0), Vec2d(64,64)));
	
	spriteManager->add("guard_die1", New Sprite(guard, Vec2d(0,0), Vec2d(64,64)));
	spriteManager->add("guard_die2", New Sprite(guard, Vec2d(64,128), Vec2d(64,64)));
	spriteManager->add("guard_die3", New Sprite(guard, Vec2d(128,192), Vec2d(64,64)));
	spriteManager->add("guard_die4", New Sprite(guard, Vec2d(128,128), Vec2d(64,64)));
	
	spriteManager->add("guard_head", New Sprite(guard, Vec2d(155,79), Vec2d(12,11)));
	animationManager->add("guard_head", New Animation(spriteManager->get("guard_head")));
	
	spriteManager->add("guard_torso", New Sprite(guard, Vec2d(152,89), Vec2d(17,16)));
	animationManager->add("guard_torso", New Animation(spriteManager->get("guard_torso")));
	
	spriteManager->add("guard_arm", New Sprite(guard, Vec2d(39,29), Vec2d(12,10)));
	animationManager->add("guard_arm", New Animation(spriteManager->get("guard_arm")));
	
	spriteManager->add("guard_leg", New Sprite(guard, Vec2d(31,36), Vec2d(13,22)));
	animationManager->add("guard_leg", New Animation(spriteManager->get("guard_leg")));

	spriteManager->add("off_walk1", New Sprite(officer, Vec2d(128,64), Vec2d(64,64)));
	spriteManager->add("off_walk2", New Sprite(officer, Vec2d(128,128), Vec2d(64,64)));
	spriteManager->add("off_walk3", New Sprite(officer, Vec2d(128,192), Vec2d(64,64)));
	spriteManager->add("off_walk4", New Sprite(officer, Vec2d(64,128), Vec2d(64,64)));
	
	spriteManager->add("off_shoot1", New Sprite(officer, Vec2d(0,128), Vec2d(64,64)));
	spriteManager->add("off_shoot2", New Sprite(officer, Vec2d(0,64), Vec2d(64,64)));
	spriteManager->add("off_shoot3", New Sprite(officer, Vec2d(0,0), Vec2d(64,64)));

	spriteManager->add("off_pain1", New Sprite(officer, Vec2d(64,0), Vec2d(64,64)));
	spriteManager->add("off_pain2", New Sprite(officer, Vec2d(0,192), Vec2d(64,64)));
	
	spriteManager->add("off_die1", New Sprite(officer, Vec2d(128,0), Vec2d(64,64)));
	spriteManager->add("off_die2", New Sprite(officer, Vec2d(64,192), Vec2d(64,64)));
	spriteManager->add("off_die3", New Sprite(officer, Vec2d(192,0), Vec2d(64,64)));
	spriteManager->add("off_die4", New Sprite(officer, Vec2d(64,64), Vec2d(64,64)));
	
	spriteManager->add("off_head", New Sprite(officer, Vec2d(92,15), Vec2d(10,11)));
	animationManager->add("off_head", New Animation(spriteManager->get("off_head")));
	
	spriteManager->add("off_torso", New Sprite(officer, Vec2d(89,26), Vec2d(15,14)));
	animationManager->add("off_torso", New Animation(spriteManager->get("off_torso")));
	
	spriteManager->add("off_arm", New Sprite(officer, Vec2d(166,29), Vec2d(13,9)));
	animationManager->add("off_arm", New Animation(spriteManager->get("off_arm")));
	
	spriteManager->add("off_leg", New Sprite(officer, Vec2d(32,106), Vec2d(13,22)));
	animationManager->add("off_leg", New Animation(spriteManager->get("off_leg")));
	
	Animation *guardWalkAnim = New Animation();
	
	guardWalkAnim->addFrameToEnd(spriteManager->get("guard_walk1"));
	guardWalkAnim->addFrameToEnd(spriteManager->get("guard_walk2"));
	guardWalkAnim->addFrameToEnd(spriteManager->get("guard_walk3"));
	guardWalkAnim->addFrameToEnd(spriteManager->get("guard_walk4"));
	
	guardWalkAnim->setFrameTime(25);
	
	animationManager->add("guardWalk", guardWalkAnim);
	
	Animation *guardShootAnim = New Animation();
	
	guardShootAnim->addFrameToEnd(spriteManager->get("guard_shoot1"));
	guardShootAnim->addFrameToEnd(spriteManager->get("guard_shoot2"));
	guardShootAnim->addFrameToEnd(spriteManager->get("guard_shoot3"));
	
	guardShootAnim->setFrameTime(1);

	animationManager->add("guardShoot", guardShootAnim);
	
	Animation *guardPainAnim = New Animation();
	
	guardPainAnim->addFrameToEnd(spriteManager->get("guard_pain1"));
	guardPainAnim->addFrameToEnd(spriteManager->get("guard_pain2"));
	
	animationManager->add("guardPain", guardPainAnim);
	
	Animation *offWalkAnim = New Animation();
	
	offWalkAnim->addFrameToEnd(spriteManager->get("off_walk1"));
	offWalkAnim->addFrameToEnd(spriteManager->get("off_walk2"));
	offWalkAnim->addFrameToEnd(spriteManager->get("off_walk3"));
	offWalkAnim->addFrameToEnd(spriteManager->get("off_walk4"));
	
	offWalkAnim->setFrameTime(25);
	
	animationManager->add("offWalk", offWalkAnim);

	Animation *offShootAnim = New Animation();
	
	offShootAnim->addFrameToEnd(spriteManager->get("off_shoot1"));
	offShootAnim->addFrameToEnd(spriteManager->get("off_shoot2"));
	offShootAnim->addFrameToEnd(spriteManager->get("off_shoot3"));
	
	animationManager->add("offShoot", offShootAnim);
	
	Animation *offPainAnim = New Animation();
	
	offPainAnim->addFrameToEnd(spriteManager->get("off_pain1"));
	offPainAnim->addFrameToEnd(spriteManager->get("off_pain2"));
	
	animationManager->add("offPain", offPainAnim);

	Animation *sunglassesAnim = New Animation();
	Animation *iceballAnim = New Animation();

	sunglassesAnim->addFrameToEnd(spriteManager->get("sunglasses"));
	iceballAnim->addFrameToEnd(spriteManager->get("iceball"));

	animationManager->add("sunglasses", sunglassesAnim);
	animationManager->add("iceball", iceballAnim);


	Texture *spellhand = New Texture(*resourceManager->get("spellhand_texture"));
	textureManager->add("spellhand", spellhand);
	spriteManager->add("spellhand", New Sprite(spellhand, Vec2d(0,0), Vec2d(160,160)));
	Animation *spellhandAnim = New Animation();
	spellhandAnim->addFrameToEnd(spriteManager->get("spellhand"));
	animationManager->add("spellhand", spellhandAnim);


	Texture *bomb = New Texture(*resourceManager->get("bomb_texture"));
	textureManager->add("bomb", bomb);
	spriteManager->add("bomb", New Sprite(bomb, Vec2d(0,0), Vec2d(32,32)));
	Animation *bombAnim = New Animation();
	bombAnim->addFrameToEnd(spriteManager->get("bomb"));
	animationManager->add("bomb",bombAnim);

	Texture *stick_grenade = New Texture(*resourceManager->get("stick_grenade_texture"));
	textureManager->add("stick_grenade", stick_grenade);
	spriteManager->add("stick_grenade", New Sprite(stick_grenade, Vec2d(0,0), Vec2d(61,343)));
	Animation *stickGrenadeAnim = New Animation();
	stickGrenadeAnim->addFrameToEnd(spriteManager->get("stick_grenade"));
	animationManager->add("stick_grenade",stickGrenadeAnim);

	Texture *vaporize = New Texture(*resourceManager->get("vaporize_texture"));
	textureManager->add("vaporize", vaporize);
	
	{
	int count = 0;

	for(int j = 0; j < 1024; j += 1024/3)
	{
		for(int i = 0; i < 2048; i += 2048/11)
		{
			spriteManager->add("vaporize" + std::to_string(count), New Sprite(vaporize, Vec2d(float(i),float(j)), Vec2d(2048/11,1024/3)));
			count++;
		}
		
		if(count > 25)
			break;
	}
	}
	
	Animation *vaporizeAnim = New Animation();
	
	for(int i = 0; i < 25; i++)
		vaporizeAnim->addFrameToEnd(spriteManager->get("vaporize" + std::to_string(i)));

	vaporizeAnim->setLoopable(false);

	animationManager->add("vaporize",vaporizeAnim);

	spriteManager->add("swamp_monster0", New Sprite(swampmonster, Vec2d(0,0), Vec2d(66,66)));
	spriteManager->add("swamp_monster1", New Sprite(swampmonster, Vec2d(0,66), Vec2d(66,66)));
	spriteManager->add("swamp_monster2", New Sprite(swampmonster, Vec2d(0,132), Vec2d(66,66)));
	spriteManager->add("swamp_monster3", New Sprite(swampmonster, Vec2d(0,198), Vec2d(66,66)));

	spriteManager->add("minotaur0", New Sprite(minotaur, Vec2d(0,0), Vec2d(102,118)));
	spriteManager->add("minotaur1", New Sprite(minotaur, Vec2d(102,0), Vec2d(102,118)));
	spriteManager->add("minotaur2", New Sprite(minotaur, Vec2d(204,0), Vec2d(102,118)));
	spriteManager->add("minotaur3", New Sprite(minotaur, Vec2d(306,0), Vec2d(102,118)));

	spriteManager->add("zombie0", New Sprite(zombie, Vec2d(0,0), Vec2d(40,55)));
	spriteManager->add("zombie1", New Sprite(zombie, Vec2d(40,0), Vec2d(40,55)));
	spriteManager->add("zombie2", New Sprite(zombie, Vec2d(80,0), Vec2d(40,55)));
	spriteManager->add("zombie3", New Sprite(zombie, Vec2d(120,0), Vec2d(40,55)));

	spriteManager->add("shot", New Sprite(shot, Vec2d(128,128), Vec2d(128,128)));

	{
		unsigned int count = 0;
		unsigned int dimension = 80;
		for(int y = 0; y < 5; ++y)
		{
			for(int x = 0; x < 5; ++x)
			{
				if(count > 22)
					break;
				spriteManager->add("fireball" + std::to_string(count), New Sprite(fireball, Vec2d(x*64.f,y*64.f), Vec2d(64.f,64.f)));
				count++;
			}
		}
	}

	Animation *fireballAnim = New Animation();

	for(int i = 0; i < 23; ++i)
		fireballAnim->addFrameToEnd(spriteManager->get("fireball"+std::to_string(i)));

	animationManager->add("fireball",fireballAnim);

	fireballAnim->setFrameTime(1);

	fireballAnim->advance();

	
	Animation *shotAnim = New Animation();

	shotAnim->addFrameToEnd(spriteManager->get("shot"));

	animationManager->add("shot", shotAnim);

	Animation *swampmonsterAnim = New Animation();

	swampmonsterAnim->addFrameToEnd(spriteManager->get("swamp_monster0"));
	swampmonsterAnim->addFrameToEnd(spriteManager->get("swamp_monster1"));
	swampmonsterAnim->addFrameToEnd(spriteManager->get("swamp_monster2"));
	swampmonsterAnim->addFrameToEnd(spriteManager->get("swamp_monster3"));

	swampmonsterAnim->advance();

	swampmonsterAnim->setFrameTime(25);

	Animation *minotaurAnim = New Animation();

	minotaurAnim->addFrameToEnd(spriteManager->get("minotaur0"));
	minotaurAnim->addFrameToEnd(spriteManager->get("minotaur1"));
	minotaurAnim->addFrameToEnd(spriteManager->get("minotaur2"));
	minotaurAnim->addFrameToEnd(spriteManager->get("minotaur3"));

	minotaurAnim->advance();

	minotaurAnim->setFrameTime(25);

	Animation *zombieAnim = New Animation();

	zombieAnim->addFrameToEnd(spriteManager->get("zombie0"));
	zombieAnim->addFrameToEnd(spriteManager->get("zombie1"));
	zombieAnim->addFrameToEnd(spriteManager->get("zombie2"));
	zombieAnim->addFrameToEnd(spriteManager->get("zombie3"));

	zombieAnim->advance();

	zombieAnim->setFrameTime(25);

	animationManager->add("minotaurWalk",minotaurAnim);
	animationManager->add("swampmonsterWalk",swampmonsterAnim);
	animationManager->add("zombieWalk",zombieAnim);

	Texture *minotaurHead = New Texture(*resourceManager->get("minotaur_head_texture"));
	Texture *minotaurTorso = New Texture(*resourceManager->get("minotaur_torso_texture"));
	Texture *minotaurArm = New Texture(*resourceManager->get("minotaur_arm_texture"));
	Texture *minotaurLeg = New Texture(*resourceManager->get("minotaur_leg_texture"));

	Texture *swampmonsterHead = New Texture(*resourceManager->get("swampmonster_head_texture"));
	Texture *swampmonsterArm = New Texture(*resourceManager->get("swampmonster_arm_texture"));
	Texture *swampmonsterLeg = New Texture(*resourceManager->get("swampmonster_leg_texture"));

	Texture *zombieHead = New Texture(*resourceManager->get("zombie_head_texture"));
	Texture *zombieArm = New Texture(*resourceManager->get("zombie_arm_texture"));
	Texture *zombieLeg = New Texture(*resourceManager->get("zombie_leg_texture"));

	textureManager->add("minotaur_head", minotaurHead);
	textureManager->add("minotaur_torso", minotaurTorso);
	textureManager->add("minotaur_arm", minotaurArm);
	textureManager->add("minotaur_leg", minotaurLeg);

	textureManager->add("swampmonster_head", swampmonsterHead);
	textureManager->add("swampmonster_arm", swampmonsterArm);
	textureManager->add("swampmonster_leg", swampmonsterLeg);

	textureManager->add("zombie_head", zombieHead);
	textureManager->add("zombie_arm", zombieArm);
	textureManager->add("zombie_leg", zombieLeg);

	spriteManager->add("minotaur_head", New Sprite(minotaurHead, Vec2d(0,0), minotaurHead->getSizeVec2d()));
	spriteManager->add("minotaur_torso", New Sprite(minotaurTorso, Vec2d(0,0), minotaurHead->getSizeVec2d()));
	spriteManager->add("minotaur_arm", New Sprite(minotaurArm, Vec2d(0,0), minotaurArm->getSizeVec2d()));
	spriteManager->add("minotaur_leg", New Sprite(minotaurLeg, Vec2d(0,0), minotaurLeg->getSizeVec2d()));

	spriteManager->add("swampmonster_head", New Sprite(swampmonsterHead, Vec2d(0,0), swampmonsterHead->getSizeVec2d()));
	spriteManager->add("swampmonster_arm", New Sprite(swampmonsterArm, Vec2d(0,0), swampmonsterArm->getSizeVec2d()));
	spriteManager->add("swampmonster_leg", New Sprite(swampmonsterLeg, Vec2d(0,0), swampmonsterLeg->getSizeVec2d()));

	spriteManager->add("zombie_head", New Sprite(zombieHead, Vec2d(0,0), zombieHead->getSizeVec2d()));
	spriteManager->add("zombie_arm", New Sprite(zombieArm, Vec2d(0,0), zombieArm->getSizeVec2d()));
	spriteManager->add("zombie_leg", New Sprite(zombieLeg, Vec2d(0,0), zombieLeg->getSizeVec2d()));

	Animation *minotaurHeadAnim = New Animation();
	minotaurHeadAnim->addFrameToEnd(spriteManager->get("minotaur_head"));
	
	Animation *minotaurTorsoAnim = New Animation();
	minotaurTorsoAnim->addFrameToEnd(spriteManager->get("minotaur_torso"));

	Animation *minotaurArmAnim = New Animation();
	minotaurArmAnim->addFrameToEnd(spriteManager->get("minotaur_arm"));

	Animation *minotaurLegAnim = New Animation();
	minotaurLegAnim->addFrameToEnd(spriteManager->get("minotaur_leg"));

	Animation *swampmonsterHeadAnim = New Animation();
	swampmonsterHeadAnim->addFrameToEnd(spriteManager->get("swampmonster_head"));

	Animation *swampmonsterArmAnim = New Animation();
	swampmonsterArmAnim->addFrameToEnd(spriteManager->get("swampmonster_arm"));

	Animation *swampmonsterLegAnim = New Animation();
	swampmonsterLegAnim->addFrameToEnd(spriteManager->get("swampmonster_leg"));

	Animation *zombieHeadAnim = New Animation();
	zombieHeadAnim->addFrameToEnd(spriteManager->get("zombie_head"));

	Animation *zombieArmAnim = New Animation();
	zombieArmAnim->addFrameToEnd(spriteManager->get("zombie_arm"));

	Animation *zombieLegAnim = New Animation();
	zombieLegAnim->addFrameToEnd(spriteManager->get("zombie_leg"));

	animationManager->add("minotaur_head", minotaurHeadAnim);
	animationManager->add("minotaur_torso", minotaurTorsoAnim);
	animationManager->add("minotaur_arm", minotaurArmAnim);
	animationManager->add("minotaur_leg", minotaurLegAnim);

	animationManager->add("swampmonster_head", swampmonsterHeadAnim);
	animationManager->add("swampmonster_torso", New Animation(spriteManager->add("swampmonster_torso", New Sprite(swampmonster,Vec2d(19,19),Vec2d(24,20), Vec2d(2.0,2.0)))));
	animationManager->add("swampmonster_arm", swampmonsterArmAnim);
	animationManager->add("swampmonster_leg", swampmonsterLegAnim);

	animationManager->add("zombie_head", zombieHeadAnim);
	animationManager->add("zombie_torso", New Animation(spriteManager->add("zombie_torso", New Sprite(zombie,Vec2d(12,21),Vec2d(17,17)))));
	animationManager->add("zombie_arm", zombieArmAnim);
	animationManager->add("zombie_leg", zombieLegAnim);

	Texture *kick = New Texture(*resourceManager->get("kick_texture"));

	textureManager->add("kick", kick);

	spriteManager->add("kick1", New Sprite(kick, Vec2d(0,0), kick->getSizeVec2d()/2));
	spriteManager->add("kick2", New Sprite(kick, kick->getSizeVec2d()/2, kick->getSizeVec2d()/2));

	Animation *kickAnim = New Animation();

	kickAnim->addFrameToEnd(spriteManager->get("kick1"));
	kickAnim->addFrameToEnd(spriteManager->get("kick2"));

	kickAnim->setFrameTime(400);
	kickAnim->setLoopable(false);

	animationManager->add("kick", kickAnim);

	
	Texture *healthPotion = New Texture(*resourceManager->get("health_potion_texture"));

	textureManager->add("healthPotion", healthPotion);

	spriteManager->add("healthPotion", New Sprite(healthPotion, Vec2d(0,0), healthPotion->getSizeVec2d()));

	Animation *healthPotionAnim = New Animation();

	healthPotionAnim->addFrameToEnd(spriteManager->get("healthPotion"));

	animationManager->add("healthPotion", healthPotionAnim);


	Texture *selfie = New Texture(*resourceManager->get("cell_phone_texture"));

	textureManager->add("selfie", selfie);

	spriteManager->add("selfie", New Sprite(selfie, Vec2d(0,0), selfie->getSizeVec2d()));

	Animation *selfieAnim = New Animation();

	selfieAnim->addFrameToEnd(spriteManager->get("selfie"));

	animationManager->add("selfie", selfieAnim);

	Texture *century_gothic = New Texture(*resourceManager->get("century_gothic"));

	textureManager->add("century_gothic", century_gothic);

	//fontManager->add("font", New Font(shot, 12, 12));
	fontManager->add("century_gothic", New Font(century_gothic, game->getResourceManager()->get("century_gothic_metrics")));

	textureManager->add("grid", New Texture(*game->getResourceManager()->get("grid")));
	//textureManager->add("city", New Texture(*game->getResourceManager()->get("city")));
	textureManager->add("circuit", New Texture(*game->getResourceManager()->get("circuit")));

	Texture *cyborg1 = textureManager->add("cyborg1", New Texture(*game->getResourceManager()->get("cyborg1_texture")));

	Sprite *cyborg1_w1 = spriteManager->add("cyborg1_w1", New Sprite(cyborg1, Vec2d(144,140), Vec2d(39,135)));
	Sprite *cyborg1_w2 = spriteManager->add("cyborg1_w2", New Sprite(cyborg1, Vec2d(183,140), Vec2d(39,135)));
	Sprite *cyborg1_w3 = spriteManager->add("cyborg1_w3", New Sprite(cyborg1, Vec2d(222,140), Vec2d(39,135)));
	Sprite *cyborg1_w4 = spriteManager->add("cyborg1_w4", New Sprite(cyborg1, Vec2d(261,140), Vec2d(39,135)));

	Sprite *cyborg1_s1 = spriteManager->add("cyborg1_s1", New Sprite(cyborg1, Vec2d(201,0), Vec2d(44,137)));
	Sprite *cyborg1_s2 = spriteManager->add("cyborg1_s2", New Sprite(cyborg1, Vec2d(459,0), Vec2d(44,137)));

	Animation *cyborg1Walk = New Animation();

	cyborg1Walk->addFrameToEnd(cyborg1_w1);
	cyborg1Walk->addFrameToEnd(cyborg1_w2);
	cyborg1Walk->addFrameToEnd(cyborg1_w3);
	cyborg1Walk->addFrameToEnd(cyborg1_w4);

	cyborg1Walk->setFrameTime(25);

	Animation *cyborg1Shoot = New Animation();

	cyborg1Shoot->addFrameToEnd(cyborg1_s1);
	cyborg1Shoot->addFrameToEnd(cyborg1_s2);

	cyborg1Shoot->setFrameTime(25);


	Texture *circuit_board = textureManager->add("circuit_board", New Texture(*game->getResourceManager()->get("circuit_board")));

	Sprite *circuit_board_sprite = spriteManager->add("circuit_board", New Sprite(circuit_board, Vec2d(0,0), circuit_board->getSizeVec2d()));

	Animation *circuitBoardAnim = animationManager->add("circuit_board", New Animation(circuit_board_sprite));

	circuitBoardAnim->setScale(Vec2d(0.1f,0.1f));

	Animation *cyborg1_leg = animationManager->add("cyborg1_leg", New Animation(spriteManager->add("cyborg1_leg", New Sprite(cyborg1, Vec2d(107,607), Vec2d(25,66)))));
	Animation *cyborg1_head = animationManager->add("cyborg1_head", New Animation(spriteManager->add("cyborg1_head", New Sprite(cyborg1, Vec2d(548,537), Vec2d(18,16)))));
	Animation *cyborg1_torso = animationManager->add("cyborg1_torso", New Animation(spriteManager->add("cyborg1_torso", New Sprite(cyborg1, Vec2d(123,21), Vec2d(26,43)))));
	Animation *cyborg1_arm = animationManager->add("cyborg1_arm", New Animation(spriteManager->add("cyborg1_arm", New Sprite(cyborg1, Vec2d(584,11), Vec2d(43,26)))));


	animationManager->add("cyborg1Walk", cyborg1Walk);
	animationManager->add("cyborg1Shoot", cyborg1Shoot);


	Texture *hitler = textureManager->add("hitler", New Texture(*game->getResourceManager()->get("hitler_texture")));

	Sprite *hitler_w1 = spriteManager->add("hitler_w1", New Sprite(hitler, Vec2d(384,64), Vec2d(64,64)));
	Sprite *hitler_w2 = spriteManager->add("hitler_w2", New Sprite(hitler, Vec2d(0,128), Vec2d(64,64)));
	Sprite *hitler_w3 = spriteManager->add("hitler_w3", New Sprite(hitler, Vec2d(64,128), Vec2d(64,64)));
	Sprite *hitler_w4 = spriteManager->add("hitler_w4", New Sprite(hitler, Vec2d(128,128), Vec2d(64,64)));

	Sprite *hitler_s1 = spriteManager->add("hitler_s1", New Sprite(hitler, Vec2d(192,128), Vec2d(64,64)));
	Sprite *hitler_s2 = spriteManager->add("hitler_s2", New Sprite(hitler, Vec2d(256,128), Vec2d(64,64)));
	Sprite *hitler_s3 = spriteManager->add("hitler_s3", New Sprite(hitler, Vec2d(320,128), Vec2d(64,64)));

	Animation *hitlerWalk = New Animation();

	hitlerWalk->addFrameToEnd(hitler_w1);
	hitlerWalk->addFrameToEnd(hitler_w2);
	hitlerWalk->addFrameToEnd(hitler_w3);
	hitlerWalk->addFrameToEnd(hitler_w4);

	hitlerWalk->setFrameTime(2);

	Animation *hitlerShoot = New Animation();

	hitlerShoot->addFrameToEnd(hitler_s1);
	hitlerShoot->addFrameToEnd(hitler_s2);
	hitlerShoot->addFrameToEnd(hitler_s3);

	hitlerShoot->setFrameTime(0);

	animationManager->add("hitler_walk", hitlerWalk);
	animationManager->add("hitler_shoot", hitlerShoot);


	Texture *shellcasing = textureManager->add("shellcasing", New Texture(*game->getResourceManager()->get("shell_casing")));

	Sprite *shellcasing_sprite = spriteManager->add("shellcasing", New Sprite(shellcasing, Vec2d(0,0), Vec2d(20,18)));

	Animation *shellcasingAnim = animationManager->add("brass_shell", New Animation(shellcasing_sprite));

	shellcasingAnim->setScale(Vec2d(0.05f,0.05f));

	textureManager->add("normal_texture", new Texture(*game->getResourceManager()->get("normal_texture")));
	textureManager->add("height_texture", new Texture(*game->getResourceManager()->get("height_texture")));

	//shaderManager->add("shader", New Shader("vertexshader.txt","fragmentshader.txt"));

	//cwc::glShader *shader = shaderManager->loadfromFile("SimpleLightShader.vsh", "SimpleLightShader.fsh");
	cwc::glShader *shader = shaderManager->loadfromFile("vertexshader.txt","fragmentshader.txt");


	Texture *puddle = textureManager->add("pee_puddle", New Texture(*game->getResourceManager()->get("puddle_texture")));

	Sprite *puddle_sprite = spriteManager->add("pee_puddle", New Sprite(puddle, Vec2d(0,0), Vec2d(41,17)));

	Animation *puddleAnim = animationManager->add("pee_puddle", New Animation(puddle_sprite));

	//puddleAnim->setScale(Vec2d(1.0f,1.0f));

}

void Game::handleEvent( const Event& theEvent )
{
	switch(theEvent.getType())
	{
		case EVENT_QUIT:
			game->getDisplay()->setShouldClose(true);

			if(mGameStarted)
				game->save("autosave.sav");

			break;
		case EVENT_GAMELOSE:
			mpDisplay->setShouldClose(true);

			MessageBox(mpDisplay->getHwnd(),"You died!", "Oh no!", MB_ICONEXCLAMATION);

			break;
		case EVENT_LOAD:
			if(load())
			{
				mGameStarted = true;
				gpEventSystem->fireEvent(EventMenuPop());
			}
			break;
		case EVENT_SAVE:
				save();
				gpEventSystem->fireEvent(EventMenuPop());
			break;
		case EVENT_NEW:
			cleanupMaps();
			createNewGame();
			gpEventSystem->fireEvent(EventMenuPop());
			break;
		default:
			break;
	}
}

std::string Game::getOpenFile()
{
	const int BUFSIZE = 1024;
    
	char buffer[BUFSIZE] = {0};

    OPENFILENAME ofns = {0};
    ofns.lStructSize = sizeof( ofns );
    ofns.lpstrFile = buffer;
    ofns.nMaxFile = BUFSIZE;
	ofns.lpstrFilter = "Duke Spookem 3D Save (.sav)\0*.sav\0";
    ofns.lpstrTitle = "Open Savegame";
	ofns.lpstrDefExt = "sav";
	ofns.Flags = OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST;
    GetSaveFileName( & ofns );

    return buffer;
}

std::string Game::getSaveFile()
{
	const int BUFSIZE = 1024;
    
	char buffer[BUFSIZE] = {0};
    
	OPENFILENAME ofns = {0};
    ofns.lStructSize = sizeof( ofns );
    ofns.lpstrFile = buffer;
    ofns.nMaxFile = BUFSIZE;
	ofns.lpstrFilter = "Duke Spookem 3D Save (.sav)\0*.sav\0";
    ofns.lpstrTitle = "Open Savegame";
	ofns.lpstrDefExt = "sav";
	ofns.Flags = OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST;
    GetSaveFileName( & ofns );
    
	return buffer;
}

void Game::save(std::string s){

	FILE* save = NULL;
	
	//while(!save)
	//{

	std::string savefile;

	if(s == "")
		savefile = getSaveFile();
	else
		savefile = s;

	save = fopen(savefile.c_str(), "wb");
	//}

	if(!save)
		return;

	writeSave(save, 0);

	mpPlayer->writeSave(save, 0);

	for(int i = 0; i < 10; ++i){
		mpMap[i]->writeSave(save, 0);
	}
}

bool Game::load(){

	if(mpMap && mpMap[current_map] && getEntityManager())
	{
		getEntityManager()->quiet();
	}

	FILE* save = fopen(getOpenFile().c_str(), "rb");
	
	if(!save)
		return false;

	cleanupMaps();

	readSave(save, 0);

	mpPlayer = New Player();

	mpPlayer->readSave(save, 0);

	mpMap = new Map*[10];

	for(int i = 0; i < 10; ++i){
		mpMap[i] = New Map();
		mpMap[i]->readSave(save, 0);

		if(!mpMap[i]->getCleared())
			populateMap(i);
	}

	playMapMusic();

	return true;
}

void Game::playMapMusic()
{
	switch(mpMap[current_map]->getTheme())
	{
		case THEME_GRAVEYARD:
			mpSoundEngine->playMusic(*resourceManager->get("music_graveyard"));
			break;
		case THEME_DUNGEON:
			mpSoundEngine->playMusic(*resourceManager->get("music_dungeon"));
			break;
		case THEME_NAZI:
			if(rand() % 2)
				mpSoundEngine->playMusic(*resourceManager->get("music_nazi1"));
			else
				mpSoundEngine->playMusic(*resourceManager->get("music_nazi2"));
			break;
		case THEME_TECH:
			if(rand() % 2)
				mpSoundEngine->playMusic(*resourceManager->get("music_tech1"));
			else
				mpSoundEngine->playMusic(*resourceManager->get("music_tech2"));
			break;
	}
}

void Game::writeSave(FILE *outfile, unsigned pos)
{
	fwrite(&current_map, sizeof(int), 1, outfile);
	fwrite(&mHardness, sizeof(float), 1, outfile);
}

void Game::readSave(FILE *infile, unsigned pos)
{
	fread(&current_map, sizeof(int), 1, infile);
	fread(&mHardness, sizeof(float), 1, infile);
}

void Game::interact()
{
	switch(mpMap[current_map]->getTile((int)mpPlayer->getPosition().X, (int)mpPlayer->getPosition().Z).type)
	{
	case TYPE_STAIRS_DOWN:
		if(current_map < 9)
		{
			mpMap[current_map]->getEntityManager()->remove(player_key, false);

			//If we haven't cleared the current map, leave monsters behind quote.
			if(!mpMap[current_map]->getCleared())
				mpPlayer->speak(mpPlayer->getQuipLeaveBehind());

			mpMap[current_map]->getEntityManager()->quiet();

			++current_map;
			player_key = mpMap[current_map]->getEntityManager()->add(mpPlayer);
			mpMap[current_map]->diffuseGoals();

			if(!mpMap[current_map]->getCleared())
			{
				if(!mpMap[current_map]->getVisited())
				{
					if(mpMap[current_map]->getLevel() == 0)
					{
						playMapMusic();
					}

					if(mpMap[current_map]->getTheme() == THEME_NAZI)
						mpPlayer->speak(mpPlayer->getQuipEnterNazi(), true);
					else if(mpMap[current_map]->getTheme() == THEME_TECH && current_map > 0 && mpMap[current_map-1]->getTheme() != THEME_TECH)
						mpPlayer->speak(mpPlayer->getQuipEnterTech(), true);
					else
						mpPlayer->speak(mpPlayer->getQuipNewLevel());
				}
				else
					mpPlayer->speak(mpPlayer->getQuipReturnLevel());
			}

			mpMap[current_map]->getEntityManager()->unQuiet();

			mpMap[current_map]->setVisited(true);
		}
		break;
	case TYPE_STAIRS_UP:
		if(current_map > 0)
		{
			//bool oldMapCleared = mpMap[current_map]->getCleared();

			mpMap[current_map]->getEntityManager()->remove(player_key, false);

			mpMap[current_map]->getEntityManager()->quiet();

			--current_map;
			player_key = mpMap[current_map]->getEntityManager()->add(mpPlayer);
			mpMap[current_map]->diffuseGoals();

			if(mpMap[current_map]->getCleared())
				mpPlayer->speak(mpPlayer->getQuipLevelAlreadyCleared());
			else
				mpPlayer->speak(mpPlayer->getQuipReturnLevel());

			/*mpPlayer->speak(mpPlayer->getQuipLeaveBehind());

			if(mpMap[current_map]->getCleared())
				mpPlayer->speak(mpPlayer->getQuipLevelAlreadyCleared());*/

			/*if(!oldMapCleared)
				mpPlayer->speak(mpPlayer->getQuipReturnLevel());
			else
				mpPlayer->speak(mpPlayer->getQuipLeaveBehind());*/

			mpMap[current_map]->getEntityManager()->unQuiet();
		}
		break;
	}
}

void Game::populateMap(int level)
{
	static bool hitlerSpawned = false;

	unsigned int count = 0;

	bool shouldBreak = false;

	srand(time(NULL));

	for(unsigned x = 0; x < mpMap[level]->getWidth(); ++x)
	{
		for(unsigned y = 0; y < mpMap[level]->getHeight(); ++y)
		{
			if(mpMap[level]->getTile(x, y).type == TYPE_FLOOR)
			{
					//drawBillboard("driver", Vec3d(x, -0.5, y));
					//mpPlayer->setPosition(Vec3d(x, 0, y));
					if(rand() % 4)
					{
						count++;

						switch(mpMap[level]->getTheme())
						{
							case THEME_GRAVEYARD:
								if(rand() % 4 == 0)
									mpMap[level]->getEntityManager()->add(New SwampMonster(Vec3d(float(x) + 0.5f,-0.5,float(y) + 0.5f)));
								else
									mpMap[level]->getEntityManager()->add(New Zombie(Vec3d(float(x) + 0.5f,-0.5,float(y) + 0.5f)));
								break;
							case THEME_DUNGEON:
								switch(rand() % 3)
								{
									case 0:
										mpMap[level]->getEntityManager()->add(New Minotaur(Vec3d(float(x) + 0.5f,-0.5,float(y) + 0.5f)));
										break;
									case 1:
										mpMap[level]->getEntityManager()->add(New Zombie(Vec3d(float(x) + 0.5f,-0.5,float(y) + 0.5f)));
										break;
									case 2:
										mpMap[level]->getEntityManager()->add(New SwampMonster(Vec3d(float(x) + 0.5f,-0.5,float(y) + 0.5f)));
										break;
								}
								break;
							case THEME_NAZI:
								if(rand() % 3 && !hitlerSpawned)
								{
									mpMap[level]->getEntityManager()->add(New Hitler(Vec3d(float(x) + 0.5f,-0.5,float(y) + 0.5f)));
									hitlerSpawned = true;
								}
								mpMap[level]->getEntityManager()->add(New Nazi(Vec3d(float(x) + 0.5f,-0.5,float(y) + 0.5f)));
								break;
							case THEME_TECH:
								mpMap[level]->getEntityManager()->add(New Cyborg(Vec3d(float(x) + 0.5f,-0.5,float(y) + 0.5f)));
								break;
							default:
								break;
						}
					}

				//mpMap[level]->getEntityManager()->add(New Hitler(Vec3d(float(x) + 0.5f,-0.5,float(y) + 0.5f)));
				//shouldBreak = true;
				break;
			}

			if(shouldBreak)
				break;
		}

		if(shouldBreak)
			break;	
	}

	if(mpMap[level]->getTheme() == THEME_NAZI && mpMap[level]->getLevel() == 2)
	{
		int x = 0, y = 0;
		while(true)
		{
			x = rand() % mpMap[level]->getWidth();
			y = rand() % mpMap[level]->getHeight();

			if(mpMap[level]->getTile(x, y).type == TYPE_FLOOR)
			{
				mpMap[level]->getEntityManager()->add(New Hitler(Vec3d(float(x) + 0.5f,-0.5,float(y) + 0.5f)));
				break;
			}
		}
	}

	std::cout << count << " monsters added!\n";
}

void Game::cleanupMaps()
{
	mShouldEnd = true;

	if(mpMap)
	{
		for(int i = 0; i < 10; ++i)
		{
			delete mpMap[i];
			mpMap[i] = NULL;
		}
		
		delete mpMap;
		mpMap = NULL;
	}

	delete mpPlayer;
	mpPlayer = NULL;

	mShouldEnd = false;
}

void Game::drawHUD()
{
	glDisable( GL_DEPTH_TEST );

	//glClear(GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	Vec2d bounds = game->getDisplay()->getDimensions();

	gluOrtho2D(0,bounds.X(),0,bounds.Y());

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	if(mpPlayer)
	{
		mpPlayer->drawHUD();
	}

	menuStack->draw();

	glEnable(GL_TEXTURE_2D);
	glEnable( GL_DEPTH_TEST );

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void Game::createNewGame()
{
	if(mpMap && mpMap[current_map] && getEntityManager())
	{
		getEntityManager()->quiet();
	}

	cleanupMaps();

	mGameStarted = true;

	mpPlayer = New Player();

	mpMap = new Map*[10];
	mpMap[0] = New Map(GENERATION_DRUNKARD, 25, 25, -1, -1, 0, THEME_GRAVEYARD);
	
	player_key = mpMap[0]->getEntityManager()->add(mpPlayer);
	populateMap(0);

	for(int i = 1; i < 4; ++i)
	{
		mpMap[i] = New Map(GENERATION_DRUNKARD, 35+i, 35+i, mpMap[i-1]->getDownStairsX(), mpMap[i-1]->getDownStairsY(), i-1, THEME_DUNGEON);
		populateMap(i);
	}

	for(int i = 4; i < 7; ++i)
	{
		mpMap[i] = New Map(GENERATION_DRUNKARD, 35+i, 35+i, mpMap[i-1]->getDownStairsX(), mpMap[i-1]->getDownStairsY(), i - 4, THEME_NAZI);
		populateMap(i);
	}

	for(int i = 7; i < 10; ++i)
	{
		mpMap[i] = New Map(GENERATION_DRUNKARD, 35+i, 35+i, mpMap[i-1]->getDownStairsX(), mpMap[i-1]->getDownStairsY(), i - 7, THEME_TECH);
		populateMap(i);
	}

	current_map = 0;

	unsigned w = mpMap[0]->getWidth();
	unsigned h = mpMap[0]->getHeight();

	game->getPlayer()->speak(*resourceManager->get("player_intro"));

	mpSoundEngine->playMusic(*resourceManager->get("music_dungeon"));

	mpPlayer->setPosition(Vec3d(mpMap[0]->getUpStairsX() + 0.5f, 0, mpMap[0]->getUpStairsY() + 0.5f));
	if(mpMap[0]->getTile(mpMap[0]->getUpStairsX(), mpMap[0]->getUpStairsY()-1).type == TYPE_WALL)
	{
		if(mpMap[0]->getTile(mpMap[0]->getUpStairsX()-1, mpMap[0]->getUpStairsY()).type != TYPE_WALL)
			mpPlayer->setAngle(vec3df(90, 0, 0));
		else if(mpMap[0]->getTile(mpMap[0]->getUpStairsX(), mpMap[0]->getUpStairsY()+1).type != TYPE_WALL)
			mpPlayer->setAngle(vec3df(180, 0, 0));
		else
			mpPlayer->setAngle(vec3df(270, 0, 0));
	}

	
}

void Game::doLoop()
{
#if defined(_WIN32)
	Vec3d vec = Vec3d(4,3,0);
#endif

	//createNewGame();
	menuStack->push(New MainMenu());

	//******************************************
	//*This section enables geometry smoothing.*
	//******************************************

	//Enable alpha blending.
	glEnable( GL_BLEND );

	//Enable depth testing.
    glEnable( GL_DEPTH_TEST );

	//Enable automatic backface culling.
	glEnable(GL_CULL_FACE);
	glDepthMask(GL_TRUE);

	//For some reason smoothing causes diagonal lines in quads.
	/*glHint (GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint (GL_POINT_SMOOTH_HINT,GL_NICEST);

	glEnable(GL_POLYGON_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POINT_SMOOTH);*/

	glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_LINE_SMOOTH);

	glEnable(GL_MULTISAMPLE);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_FOG);

	//Color set to match environmental blue sky.
	float FogCol[3]={0,0,0};
	float EnvCol[3]={0.29019607843f,0.34509803921f,0.49019607843f};
	
	//Set the fog color.
	glFogfv(GL_FOG_COLOR,FogCol);

	glFogi(GL_FOG_MODE, GL_LINEAR); // Note the 'i' after glFog - the GL_LINEAR constant is an integer.
	glFogf(GL_FOG_START, 5.f);
	glFogf(GL_FOG_END, 10.f);

	//glFogf(GL_FOG_DENSITY, 0.5f);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glAlphaFunc(GL_GREATER, 0.8f);
	glEnable(GL_ALPHA_TEST);


	//populateMap();

	//entityManager->add(New Entity(bjAnim, Vec3d(0,-0.5,0)));

	//******************************************

	Timer gameTimer, drawTimer;

	drawTimer.start();

	//gameTimer.start();

	unsigned long long lastTick = GetTickCount64();

	while(!mShouldEnd)
	{
		//if(drawTimer.getElapsedTime() >= 40)
			mShouldDraw = true;

		gameTimer.start();

		while (GetTickCount64() - lastTick < 8);

		lastTick = GetTickCount64();

		/*if(!menuStack->empty())
		{
			mPaused = true;
		}
		else
			mPaused = false;
		*/
		if(!mPaused)
		{

		}

		//These two should think even if game is paused.
		//mpInput->think();
		//menuStack->think();

		//Get window to update.
		mpDisplay->think();

		//Have the sound engine update.
		mpSoundEngine->think();

		//Menu stack think.
		menuStack->think();

		if(!menuStack->empty())
			mpDisplay->shouldTrapCursor(false);
		else
		{
			mpDisplay->shouldTrapCursor(true);
		}

		//Update player.
		if(mpPlayer)
			mpPlayer->think();

		//This will close window if ALT-F4 or close button are pressed.
		if(mpDisplay->shouldClose())
			mShouldEnd = true;

		//Sample graphics code.
		{
			float ratio;
			int width, height;
			width = mpDisplay->getWidth();
			height = mpDisplay->getHeight();
			ratio = width / (float) height;
			glViewport(0, 0, width, height);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();

			gluPerspective(60.0,ratio,0.25,1000000);

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			if(mpPlayer)
			{
				//Set player camera.
				mpPlayer->draw();

				mpMap[current_map]->draw();

				mpMap[current_map]->diffuseGoals();

				mpMap[current_map]->getEntityManager()->think(gameTimer.getElapsedTime());
				mpMap[current_map]->getEntityManager()->runCollisions();

				gameTimer.stop();

				//if(seeThruEnabled)
					//glDisable(GL_DEPTH_TEST);

				mpMap[current_map]->getEntityManager()->draw();

				if(game->getWireframeBoundingBoxMode())
				{
					glDisable(GL_TEXTURE_2D);

					glLineWidth(10);

					glBegin(GL_LINES);
					// Front Face

					//Set tint.
					glColor3f(1.0f, 0.0f, 0.0f);

					glVertex3f(mpPlayer->getPosition().X, mpPlayer->getPosition().Y, mpPlayer->getPosition().Z);              // Top Left
					glVertex3f(mpMap[current_map]->getDownStairsX(), -1, mpMap[current_map]->getDownStairsY());

					glEnd();

					glEnable(GL_TEXTURE_2D);
				}

				if(mpPlayer->isPeeing())
				{
					Vec3d point = mpPlayer->getPosition();

					point.Y -= 0.45;

					Vec3d mAngle = mpPlayer->getAngle();

					Vec3d shotVelocity;

					shotVelocity.X = sin((mAngle.X * PI) / 180);
					shotVelocity.Y = -sin((mAngle.Y * PI) / 180);
	
					//Use this line if dealing with a non physics-enabled projectile.
					//shotVelocity.Y = 0;
	
					shotVelocity.Z = -cos((mAngle.X * PI) / 180);

					shotVelocity /= 20;

					//glDisable( GL_DEPTH_TEST );

					glBegin(GL_LINES);

					for(int i = 0; i < 30; ++i)
					{
						glColor3f(0.75f, 0.75f, 0);
						glVertex3f(point.X, point.Y, point.Z);

						point += shotVelocity + Vec3d(randomFloat(0.0f,0.001f), randomFloat(0.0f,0.001f), randomFloat(0.0f,0.001f));


						glColor3f(0.75f, 0.75f, 0);
						glVertex3f(point.X, point.Y, point.Z);

						shotVelocity.Y -= 0.0005;
					}

					glEnd();
				}

			}

			game->drawHUD();

			glBindTexture( GL_TEXTURE_2D, NULL);
		}

		if(mShouldDraw)
		{
			//Draw menu stack over absolutely everything!
			//menuStack->draw(mpDisplay->getBackBuffer());


			//Flip the backbuffer.
			mpDisplay->flip();

			mShouldDraw = false;
			drawTimer.start();
		}
	}
}

//Peter: Used great billboard drawing trick from http://www.lighthouse3d.com/opengl/billboarding/index.php?billCheat.
void Game::drawBillboard(std::string texture, Vec3d pos)
{
	Texture *tex = textureManager->get(texture);

	glPushMatrix();

	glTranslatef(pos.X, pos.Y, pos.Z);

	//glTranslatef(tex->getRatio()*0.5f,0,0);  

	glPushMatrix();

	tex->setAsActiveTexture();

	float modelview[16];
	int i,j;

	// save the current modelview matrix
	glPushMatrix();

	// get the current modelview matrix
	glGetFloatv(GL_MODELVIEW_MATRIX , modelview);

	// undo all rotations
	// beware all scaling is lost as well 
	for( i=0; i<3; i++ ) 
		for( j=0; j<3; j++ ) 
		{
			if ( i==j )
				modelview[i*4+j] = 1.0;
			else
				modelview[i*4+j] = 0.0;
		}

// set the modelview with no rotations and scaling
glLoadMatrixf(modelview);

	glBegin(GL_QUADS);
	// Front Face
	glColor3f(1,1,1);

	glTexCoord2f(0,0);
	glVertex3f(0.0f, -0.5f, 0.0f);              // Top Left

	glTexCoord2f(1,0);
	glVertex3f( tex->getRatio()*1.0f, -0.5f, 0.0f);              // Top Right

	glTexCoord2f(1,1);
	glVertex3f( tex->getRatio()*1.0f,0.5f, 0.0f);              // Bottom Right

	glTexCoord2f(0,1);
	glVertex3f(0.0f,0.5f, 0.0f);              // Bottom Left

	glEnd();

	glPopMatrix();

	// restores the modelview matrix
glPopMatrix();

	glPopMatrix();
}

//std::vector<Vec3d> lpos;

void Game::shout(std::string out)
{
#ifdef DEBUG_LOG_WINDOW

	if(mpLog != NULL)
	{
		mpLog->append(out);
		mpLog->append("\n");
	}

#endif

	//std::perror(out.c_str());

	std::fprintf(stderr,out.c_str());
}

EntityManager* Game::getEntityManager() { return mpMap[current_map]->getEntityManager(); }