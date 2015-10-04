#ifndef _SOUND_ENGINE_H
#define _SOUND_ENGINE_H

#include "Trackable.h"
#include <string>
#include "Sound.h"

#include <AL/al.h>
#include <AL/alc.h>

#include "VectorMath.h"

class SoundEngine : public Trackable
{
	private:
        ISound *mpMusic;

		float mMusicVolume;
        ALCdevice* mpDevice;
        ALCcontext* mpContext;
	public:
		SoundEngine();
		~SoundEngine();
		
		//Plays a 2D Sound based on path.
		void play2DSound(std::string path, float volume = 0.5f, float speed = 1.0f);
		void play2DSound(std::string *path, float volume = 0.5f, float speed = 1.0f);

		//Plays a 2D Sound based on path.
		//Sound* play2DSoundKeep(std::string path, float volume = 0.5f, float speed = 1.0f);
		//Sound* play2DSoundKeep(std::string *path, float volume = 0.5f, float speed = 1.0f);

		//Sound* play2DSoundKeepLooped(std::string *path, float volume, float speed = 1.0f);

		//Plays a 3D Sound based on path.
		//void play3DSound(std::string path, Vec3d &vector, float volume = 0.5f, float speed = 1.0f);
		//void play3DSound(std::string *path, Vec3d &vector, float volume = 0.5f, float speed = 1.0f);

		//Plays a 3D Sound based on path.
		//Sound* play3DSoundKeep(std::string path, Vec3d &vector, float volume = 0.5f, float speed = 1.0f);
		//Sound* play3DSoundKeep(std::string *path, Vec3d &vector, float volume = 0.5f, float speed = 1.0f);

		//Play music.
		void playMusic(std::string path, bool loop = true);

		//Update player position and things.
		void think();

		//Set global effects.
		//void setFx(Sound *sound);
		
		//inline float getMusicVolume() { if(mpMusic) return mpMusic->getVolume(); else return mMusicVolume; }
		//inline void setMusicVolume(float volume) { mMusicVolume = volume; if(mpMusic) mpMusic->setVolume(mMusicVolume); }
};

#endif
