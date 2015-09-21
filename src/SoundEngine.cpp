#include "SoundEngine.h"

#include "Game.h"

#include "InputManager.h"

#include "Player.h"

extern Game *game;

SoundEngine::SoundEngine()
{
#if defined(_WIN32)
	try
	{
		if(!(mpSoundEngine = createIrrKlangDevice()))
			throw "Could not initialize IrrKlangDevice for sound engine!";
	}
	catch(const char *msg)
	{
		game->shout(msg);
	}

	mpMusic = NULL;
	mMusicVolume = 0.1f;
#endif
}

SoundEngine::~SoundEngine()
{
#if defined(_WIN32)
	if(mpSoundEngine)
		mpSoundEngine->drop();
#endif
}

//Sound* SoundEngine::play2DSoundKeep(std::string path, float volume, float speed)
//{
//#if defined(_WIN32)
	//ISound *sound = mpSoundEngine->play2D(path.c_str(),false,true,true,ESM_AUTO_DETECT,true);

	//if(!sound)
		//return NULL;

	

	//sound->setVolume(volume);
	//sound->setPlaybackSpeed(speed);
	//sound->setIsPaused(false);

	////Enable super awesome Reverb sound effect.
	//setFx(sound);

	//return sound;

	////fx->enableWavesReverbSoundEffect(0,0,99,0.001000000047F);

	////sound->drop();

	////sound->setIsLooped(true);

	//[>Sound *out = New Sound();

	//out->setSoundPtr(sound);*/
//#endif
//}

//Sound* SoundEngine::play2DSoundKeep(std::string *path, float volume, float speed)
//{
//#if defined(_WIN32)
	//ISound *sound = mpSoundEngine->play2D(path->c_str(),false,true,true,ESM_AUTO_DETECT,true);

	//if(!sound)
		//return NULL;

	
	//sound->setVolume(volume);
	//sound->setPlaybackSpeed(speed);
	//sound->setIsPaused(false);


	////Enable super awesome Reverb sound effect.
	//setFx(sound);

	//return sound;

	////fx->enableWavesReverbSoundEffect(0,0,99,0.001000000047F);

	////sound->drop();

	////sound->setIsLooped(true);

	//[>Sound *out = New Sound();

	//out->setSoundPtr(sound);*/
//#endif
//}

//Sound* SoundEngine::play2DSoundKeepLooped(std::string *path, float volume, float speed)
//{
//#if defined(_WIN32)
	//ISound *sound = mpSoundEngine->play2D(path->c_str(),true,true,true,ESM_AUTO_DETECT,true);

	//if(!sound)
		//return NULL;

	
	//sound->setVolume(volume);
	//sound->setPlaybackSpeed(speed);
	//sound->setIsPaused(false);


	////Enable super awesome Reverb sound effect.
	//setFx(sound);

	//return sound;

	////fx->enableWavesReverbSoundEffect(0,0,99,0.001000000047F);

	////sound->drop();

	////sound->setIsLooped(true);

	//[>Sound *out = New Sound();

	//out->setSoundPtr(sound);*/
//#endif
//}

void SoundEngine::play2DSound(std::string path, float volume, float speed)
{
#if defined(_WIN32)
	ISound *sound = mpSoundEngine->play2D(path.c_str(),false,true,true,ESM_AUTO_DETECT,true);

	if(!sound)
		return; //NULL;

	
	sound->setVolume(volume);
	sound->setPlaybackSpeed(speed);
	sound->setIsPaused(false);
	

	//Enable super awesome Reverb sound effect.
	setFx(sound);

	//fx->enableWavesReverbSoundEffect(0,0,99,0.001000000047F);

	sound->drop();

	//sound->setIsLooped(true);

	/*Sound *out = New Sound();

	out->setSoundPtr(sound);*/

	sound = NULL;

	return; //out;
#endif
}

void SoundEngine::play2DSound(std::string *path, float volume, float speed)
{
#if defined(_WIN32)
	ISound *sound = mpSoundEngine->play2D(path->c_str(),false,true,true,ESM_AUTO_DETECT,true);

	if(!sound)
		return; //NULL;

	
	sound->setVolume(volume);
	sound->setPlaybackSpeed(speed);
	sound->setIsPaused(false);

	//Enable super awesome Reverb sound effect.
	setFx(sound);

	//fx->enableWavesReverbSoundEffect(0,0,99,0.001000000047F);

	sound->drop();

	//sound->setIsLooped(true);

	/*Sound *out = New Sound();

	out->setSoundPtr(sound);*/

	sound = NULL;

	return; //out;
#endif
}

//void SoundEngine::play3DSound(std::string path, Vec3d &vector, float volume, float speed)
//{
//#if defined(_WIN32)
	//ISound *sound = mpSoundEngine->play3D(path.c_str(),vector,false,true,true,ESM_AUTO_DETECT,true);

	////Something went wrong...
	//if(!sound)
		//return; //NULL;

	
	//sound->setVolume(volume);
	//sound->setPlaybackSpeed(speed);
	//sound->setIsPaused(false);
	

	////Enable super awesome Reverb sound effect.
	//setFx(sound);

	////fx->enableWavesReverbSoundEffect(0,0,99,0.001000000047F);

	////Pretty much similar to delete only it cleans itself up somehow.
	//sound->drop();

	//[>Sound *out = New Sound();

	//out->setSoundPtr(sound);*/

	//sound = NULL;

	//return; //out;
//#endif
//}

//void SoundEngine::play3DSound(std::string *path, Vec3d &vector, float volume, float speed)
//{
//#if defined(_WIN32)
	//ISound *sound = mpSoundEngine->play3D(path->c_str(),vector,false,true,true,ESM_AUTO_DETECT,true);

	////Something went wrong...
	//if(!sound)
		//return; //NULL;

	
	//sound->setVolume(volume);
	//sound->setPlaybackSpeed(speed);
	//sound->setIsPaused(false);
	

	////Enable super awesome Reverb sound effect.
	//setFx(sound);

	////fx->enableWavesReverbSoundEffect(0,0,99,0.001000000047F);

	////Pretty much similar to delete only it cleans itself up somehow.
	//sound->drop();

	//[>Sound *out = New Sound();

	//out->setSoundPtr(sound);*/

	//sound = NULL;

	//return; //out;
//#endif
//}

//Sound* SoundEngine::play3DSoundKeep(std::string *path, Vec3d &pos, float volume, float speed)
//{
//#if defined(_WIN32)
	//ISound *sound = mpSoundEngine->play3D(path->c_str(), pos, false,true,true,ESM_AUTO_DETECT,true);

	//if(!sound)
		//return NULL;

	
	//sound->setVolume(volume);
	//sound->setPlaybackSpeed(speed);
	//sound->setIsPaused(false);
	

	////Enable super awesome Reverb sound effect.
	//setFx(sound);

	////fx->enableWavesReverbSoundEffect(0,0,99,0.001000000047F);

	//return sound;

	////sound->drop();

	////sound->setIsLooped(true);

	//[>Sound *out = New Sound();

	//out->setSoundPtr(sound);*/
//#endif
//}

//Sound* SoundEngine::play3DSoundKeep(std::string path, Vec3d &pos, float volume, float speed)
//{
//#if defined(_WIN32)
	//ISound *sound = mpSoundEngine->play3D(path.c_str(), pos, false,true,true,ESM_AUTO_DETECT,true);

	//if(!sound)
		//return NULL;

	
	//sound->setVolume(volume);
	//sound->setPlaybackSpeed(speed);
	//sound->setIsPaused(false);

	////Enable super awesome Reverb sound effect.
	//setFx(sound);

	////fx->enableWavesReverbSoundEffect(0,0,99,0.001000000047F);

	//return sound;

	////sound->drop();

	////sound->setIsLooped(true);

	//[>Sound *out = New Sound();

	//out->setSoundPtr(sound);*/
//#endif
//}

//void SoundEngine::playMusic(std::string path, bool loop)
//{

	//if(mpMusic)
	//{
		//mpMusic->stop();
		//mpMusic->drop();
	//}

	//mpMusic = mpSoundEngine->play2D(path.c_str(),loop,false,true,ESM_AUTO_DETECT,true);

	//if(!mpMusic)
		//return;

	//mpMusic->setVolume(mMusicVolume);

	////SoundEffectControl* fx = 0;

	//return;
//}

void SoundEngine::think()
{
#if defined(_WIN32)
	//Update sound viewpoint.
	//mpSoundEngine->setListenerPosition(game->getPlayer()->getPosition(), game->getPlayer()->getViewAngle(), game->getPlayer()->getVelocity());

	//Vec3d pos = Vec3d(game->getInput()->getMouseX(),game->getInput()->getMouseY(),0)/100;

	if(game->getPlayer())
	{
		Vec3d mAngle = game->getPlayer()->getAngle();
		Vec3d angle;

		angle.X = sin((mAngle.X * PI) / 180);
		angle.Y = -sin((mAngle.Y * PI) / 180);
		angle.Z = -cos((mAngle.X * PI) / 180);
		mpSoundEngine->setListenerPosition(game->getPlayer()->getPosition(),-angle);
	}
#endif
} 


//void SoundEngine::setFx(Sound *sound)
//{
	////ISoundEffectControl *fx = sound->getSoundEffectControl();

	////fx->enableWavesReverbSoundEffect(0.0F, 0.f,2000,0.4);
//}
