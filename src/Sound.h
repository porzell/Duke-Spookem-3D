#ifndef SOUND_H
#define SOUND_H
#include <AL/al.h>
#include <AL/alc.h>
#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisenc.h>
#include <vorbis/vorbisfile.h>
#define STREAM_BUFFER_SIZE (4096 * 8)

enum class SoundFile
{
    UNRECOGNIZED,
    WAV,
    OGG
};

class ISound
{
public:
    virtual ~ISound() {}
    virtual bool load(const char* file, SoundFile format, bool relative = false) = 0;
    virtual bool getRelative() = 0;
    virtual void setRelative(bool relative) = 0;
    virtual bool getLoaded() = 0;
    virtual void setVolume(float volume) = 0;
    virtual float getVolume() = 0;
    virtual bool isFinished() = 0;
    //SoundEffectControl* fx = 0;
    virtual void cleanup() = 0;
    virtual void play() = 0;
    virtual void pause() = 0;
    virtual void stop() = 0;
    virtual void update(float dt) = 0;
};

class SoundEffect : public ISound
{
public:
    SoundEffect();
    virtual ~SoundEffect() {}
    virtual bool load(const char* file, SoundFile format, bool relative = false);
    virtual bool getRelative() { return m_relative; }
    virtual void setRelative(bool relative) { m_relative = relative; alSourcei (m_source, AL_SOURCE_RELATIVE, relative ? AL_TRUE : AL_FALSE);}
    virtual bool getLoaded() { return m_loaded; }
    virtual void setVolume(float volume) { m_volume = volume; alSourcef(m_source, AL_GAIN, volume); }
    virtual float getVolume() { return m_volume; }
    virtual bool isFinished() { ALenum state; alGetSourcei(m_source, AL_SOURCE_STATE, &state); return state != AL_PLAYING; }
    virtual void cleanup();
    virtual void play();
    virtual void pause();
    virtual void stop();
    virtual void update(float dt) {}
protected:
    ALuint m_source;
    ALuint m_buffer;
    bool m_relative = false;
    bool m_loaded = false;
    float m_volume = 1.0f;
};

class SoundStream : public ISound
{
public:
    virtual ~SoundStream() {}
    virtual bool load(const char* file, SoundFile format, bool relative = false);
    virtual bool getRelative() { return m_relative; }
    virtual void setRelative(bool relative) { m_relative = relative; alSourcei (m_source, AL_SOURCE_RELATIVE, relative ? AL_TRUE : AL_FALSE);}
    virtual bool getLoaded() { return m_loaded; }
    virtual void setVolume(float volume) { m_volume = volume; alSourcef(m_source, AL_GAIN, volume); }
    virtual float getVolume() { return m_volume; }
    virtual bool isFinished() { ALenum state; alGetSourcei(m_source, AL_SOURCE_STATE, &state); return state != AL_PLAYING; }
    virtual void cleanup();
    virtual void play();
    virtual void pause();
    virtual void stop();
    virtual void update(float dt);
private:
    bool stream(ALuint buffer);
protected:
    ALuint m_buffers[2]; // front and back buffers
    ALuint m_source;     // audio source
    ALenum m_format;     // internal format
    bool m_relative = false;
    bool m_loaded = false;
    float m_volume = 1.0f;

    FILE*           oggFile;       // file handle
    OggVorbis_File  oggStream;     // stream handle
    vorbis_info*    vorbisInfo;    // some formatting data
    vorbis_comment* vorbisComment; // user comments
};
#endif
