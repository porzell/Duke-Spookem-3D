#include <AL/al.h>
#include <AL/alc.h>

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
    virtual bool load(const char* file, SoundFile format) = 0;
    virtual void cleanup() = 0;
    virtual void play() = 0;
};

class SoundEffect : public ISound
{
public:
    SoundEffect();
    virtual ~SoundEffect() {}
    virtual bool load(const char* file, SoundFile format);
    virtual void cleanup();
    virtual void play();
protected:
    ALuint m_source;
    ALuint m_buffer;
};

class SoundStream : public ISound
{
public:
    virtual ~SoundStream() {}
    virtual bool load(const char* file, SoundFile format);
    virtual void cleanup();
    virtual void play();
protected:
};
