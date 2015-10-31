#include "SoundEngine.h"

#include "Game.h"

#include "InputManager.h"

#include "Player.h"

extern Game *game;

SoundEngine::SoundEngine()
{
    mpDevice = alcOpenDevice(NULL);
    mpContext = alcCreateContext(mpDevice, NULL);
    alcMakeContextCurrent(mpContext);
    alListener3f(AL_POSITION, 0, 0, 0);
    alListener3f(AL_VELOCITY, 0, 0, 0);
    alListener3f(AL_ORIENTATION, 0, 0, -1);
	mpMusic = NULL;
	mMusicVolume = 0.1f;
}

SoundEngine::~SoundEngine()
{
    alcMakeContextCurrent(NULL);
    if(mpContext)
        alcDestroyContext(mpContext);
    if(mpDevice)
        alcCloseDevice(mpDevice);
}

ISound* SoundEngine::play2DSoundKeep(std::string path, float volume, float speed)
{
    ISound *sound = new SoundEffect();

    if(!sound)
        return NULL;

    sound->load(path.c_str(), SoundFile::OGG, true);

    

    sound->setVolume(volume);
    sound->play();
    //sound->setPlaybackSpeed(speed);
    //sound->setIsPaused(false);

    //Enable super awesome Reverb sound effect.
    //setFx(sound);

    return sound;

    //fx->enableWavesReverbSoundEffect(0,0,99,0.001000000047F);

    //sound->drop();

    //sound->setIsLooped(true);

    //[>Sound *out = New Sound();

    //out->setSoundPtr(sound);*/
}

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
/*#if defined(_WIN32)
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

	Sound *out = New Sound();

	out->setSoundPtr(sound);

	sound = NULL;

	return; //out;
#endif*/
}

void SoundEngine::play2DSound(std::string *path, float volume, float speed)
{
/*#if defined(_WIN32)
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

	Sound *out = New Sound();

	out->setSoundPtr(sound);

	sound = NULL;

	return; //out;
#endif*/
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

void SoundEngine::playMusic(std::string path, bool loop)
{
    if(!mpMusic)
        mpMusic = new SoundStream();
    mpMusic->load(path.c_str(), SoundFile::OGG, true);

    if(!mpMusic)
        return;

    mpMusic->setVolume(mMusicVolume);
    mpMusic->play();

    return;
}

void SoundEngine::think()
{
	//Update sound viewpoint.
	if(game->getPlayer())
	{
		Vec3d mAngle = game->getPlayer()->getAngle();
		Vec3d mPosition = game->getPlayer()->getPosition();
		Vec3d mVelocity = game->getPlayer()->getVelocity();

		mAngle.x = sin((mAngle.x * PI) / 180);
		mAngle.y = -sin((mAngle.y * PI) / 180);
		mAngle.z = -cos((mAngle.x * PI) / 180);
        alListener3f(AL_POSITION, mPosition.x, mPosition.y, mPosition.z);
        alListener3f(AL_ORIENTATION, -mAngle.x, -mAngle.y, -mAngle.z);
        alListener3f(AL_VELOCITY, mVelocity.x, mVelocity.y, mVelocity.z);
	}

    if(mpMusic)
        mpMusic->update(1.0f); // TODO: dt is unset
} 


//void SoundEngine::setFx(Sound *sound)
//{
	////ISoundEffectControl *fx = sound->getSoundEffectControl();

	////fx->enableWavesReverbSoundEffect(0.0F, 0.f,2000,0.4);
//}
