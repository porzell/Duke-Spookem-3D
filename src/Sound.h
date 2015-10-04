#include <AL/al.h>
#include <AL/alc.h>
#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisenc.h>
#include <vorbis/vorbisfile.h>

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
    virtual void pause() = 0;
    virtual void stop() = 0;
};

class SoundEffect : public ISound
{
public:
    SoundEffect();
    virtual ~SoundEffect() {}
    virtual bool load(const char* file, SoundFile format);
    virtual void cleanup();
    virtual void play();
    virtual void pause();
    virtual void stop();
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
    virtual void pause();
    virtual void stop();
protected:
    ALuint m_buffers[2]; // front and back buffers
    ALuint m_source;     // audio source
    ALenum m_format;     // internal format

    FILE*           oggFile;       // file handle
    OggVorbis_File  oggStream;     // stream handle
    vorbis_info*    vorbisInfo;    // some formatting data
    vorbis_comment* vorbisComment; // user comments
};
