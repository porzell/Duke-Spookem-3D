#include "Sound.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

SoundEffect::SoundEffect()
{
}

// TODO: Rewrite all of this
bool SoundEffect::load(const char* file, SoundFile format, bool relative)
{
    if(m_loaded)
        cleanup();
    alGenSources(1, &m_source);
    alGenBuffers(1, &m_buffer);
    alSourcef (m_source, AL_PITCH, 1);
    alSourcef (m_source, AL_GAIN, 1);
    alSource3f(m_source, AL_POSITION, 0, 0, 0);
    alSource3f(m_source, AL_VELOCITY, 0, 0, 0);
    alSourcei (m_source, AL_LOOPING, AL_FALSE);
    alSourcei (m_source, AL_SOURCE_RELATIVE, relative ? AL_TRUE : AL_FALSE);

    switch(format) {
        case SoundFile::WAV:
            {
            FILE* infile = fopen(file, "rb");
            if(!infile) {
                std::cerr << "IO ERROR: Could not open file.\n";
                return false;
            }
            
            char id[5];
            id[4] = 0;
            uint8_t* input_buffer;
            uint32_t file_size, format_size, sample_rate, uint8_ts_per_sec, data_size;
            short tag, channel_count, block_alignment, bits_per_sample;
            ALenum sound_format = AL_FORMAT_MONO8;
            
            fread(&id, sizeof(uint8_t), 4, infile);
            if(strcmp(id, "RIFF")) {
                std::cerr << "IO ERROR: File is not a RIFF file. (RIFF not found, got " << id << ")\n";
                fclose(infile);
                return false;
            }
            fread(&file_size, sizeof(uint32_t), 1, infile);
            fread(&id, sizeof(uint8_t), 4, infile);
            if(strcmp(id, "WAVE")) {
                std::cerr << "IO ERROR: File is not a WAV file. (WAVE not found, got " << id << ")\n";
                fclose(infile);
                return false;
            }
            
            fread(&id, sizeof(uint8_t), 4, infile);
            if(strcmp(id, "fmt ")) {
                std::cerr << "IO ERROR: File has unrecognizable formatting data. (fmt not found, got " << id << ")\n";
                fclose(infile);
                return false;
            }
            fread(&format_size, sizeof(uint32_t), 1, infile);
            if(format_size != 16) {
                std::cerr << "IO ERROR: File has unrecognizable formatting data. (nonstandard formatting length)\n";
                fclose(infile);
                return false;
            }
            fread(&tag, sizeof(short), 1, infile);
            fread(&channel_count, sizeof(short), 1, infile);
            fread(&sample_rate, sizeof(uint32_t), 1, infile);
            fread(&uint8_ts_per_sec, sizeof(uint32_t), 1, infile);
            fread(&block_alignment, sizeof(short), 1, infile);
            fread(&bits_per_sample, sizeof(short), 1, infile);
            
            fread(&id, sizeof(uint8_t), 4, infile);
            if(strcmp(id, "data")) {
                fclose(infile);
                return false;
            }
            fread(&data_size, sizeof(uint32_t), 1, infile);
            input_buffer = (uint8_t*) malloc(sizeof(uint8_t) * data_size);
            fread(input_buffer, sizeof(uint8_t), data_size, infile);
            fclose(infile);
            
            if(channel_count == 1) {
                if(bits_per_sample == 8) {
                    sound_format = AL_FORMAT_MONO8;
                } else if(bits_per_sample == 16) {
                    sound_format = AL_FORMAT_MONO16;
                }
            } else if(channel_count == 2) {
                if(bits_per_sample == 8) {
                    sound_format = AL_FORMAT_STEREO8;
                } else if(bits_per_sample == 16) {
                    sound_format = AL_FORMAT_STEREO16;
                }
            }
            
            alBufferData(m_buffer, sound_format, input_buffer, data_size, sample_rate);
            alSourcei(m_source, AL_BUFFER, m_buffer);
            
            free(input_buffer);

            m_loaded = true;
            }
        break;
        case SoundFile::OGG:
        {
            int result;
            FILE*           oggFile;       // file handle
            OggVorbis_File  oggStream;     // stream handle
            vorbis_info*    vorbisInfo;    // some formatting data
            int bufsize = STREAM_BUFFER_SIZE;
            char* input_buffer = (char*)malloc(bufsize * sizeof(uint8_t));
            ALenum sound_format = AL_FORMAT_MONO8;

            if(!(oggFile = fopen(file, "rb"))) {
                fprintf(stderr, "Error loading stream: File not found.");
                alDeleteSources(1, &m_source);
                alDeleteBuffers(1, &m_buffer);
                return false;
            }

            if((result = ov_open(oggFile, &oggStream, NULL, 0)) < 0)
            {
                fprintf(stderr, "Error loading the Ogg stream.");
                fclose(oggFile);
                alDeleteSources(1, &m_source);
                alDeleteBuffers(1, &m_buffer);
            }
            vorbisInfo = ov_info(&oggStream, -1);

            if(vorbisInfo->channels == 1)
                sound_format = AL_FORMAT_MONO16;
            else
                sound_format = AL_FORMAT_STEREO16;
            int  size = 0;
            int  section;
            do {
                while(size < bufsize)
                {
                    result = ov_read(&oggStream, input_buffer + size, bufsize - size, 0, 2, 1, & section);
                
                    if(result > 0)
                        size += result;
                    else
                        break;
                }
                if(size >= bufsize) {
                    input_buffer = (char*)realloc((void*)input_buffer, bufsize + STREAM_BUFFER_SIZE);
                    bufsize += STREAM_BUFFER_SIZE;
                }
            } while(result != 0);
            if(size == 0)
                return false;
         
            alBufferData(m_buffer, sound_format, input_buffer, size, vorbisInfo->rate);
            alSourcei(m_source, AL_BUFFER, m_buffer);
            free(input_buffer);

            m_loaded = true;
        }
        break;
        default:
            fprintf(stderr, "Error loading sound: Format is currently unsupported\n");
        break;
    }
    return true;
}

void SoundEffect::cleanup()
{
    alDeleteSources(1, &m_source);
    alDeleteBuffers(1, &m_buffer);

    m_loaded = false;
}

void SoundEffect::play()
{
    alSourcePlay(m_source);
}

void SoundEffect::pause()
{
    alSourcePause(m_source);
}

void SoundEffect::stop()
{
    alSourceStop(m_source);
}

bool SoundStream::load(const char* file, SoundFile format, bool relative)
{
    if(m_loaded)
        cleanup();
	alGenBuffers(2, m_buffers);
    alGenSources(1, &m_source);
    alSource3f(m_source, AL_POSITION,        0.0, 0.0, 0.0);
    alSource3f(m_source, AL_VELOCITY,        0.0, 0.0, 0.0);
    alSource3f(m_source, AL_DIRECTION,       0.0, 0.0, 0.0);
    alSourcef (m_source, AL_ROLLOFF_FACTOR,  0.0          );
    alSourcei (m_source, AL_SOURCE_RELATIVE, relative ? AL_TRUE : AL_FALSE);

    int result;
    
    if(!(oggFile = fopen(file, "rb"))) {
        fprintf(stderr, "Error loading stream: File not found.");
        alDeleteSources(1, &m_source);
        alDeleteBuffers(2, m_buffers);
        return false;
    }
 
    if((result = ov_open(oggFile, &oggStream, NULL, 0)) < 0)
    {
        fprintf(stderr, "Error loading the Ogg stream.");
        fclose(oggFile);
        alDeleteSources(1, &m_source);
        alDeleteBuffers(2, m_buffers);
    }
    vorbisInfo = ov_info(&oggStream, -1);
    vorbisComment = ov_comment(&oggStream, -1);
 
    if(vorbisInfo->channels == 1)
        m_format = AL_FORMAT_MONO16;
    else
        m_format = AL_FORMAT_STEREO16;

    m_loaded = true;
    
    return true;
}

void SoundStream::cleanup()
{
    alSourceStop(m_source);
    alDeleteSources(1, &m_source);
    alDeleteBuffers(2, m_buffers);
 
    ov_clear(&oggStream);

    m_loaded = false;
}

void SoundStream::play()
{
    ALenum state;
    alGetSourcei(m_source, AL_SOURCE_STATE, &state);
    if(state == AL_PLAYING)
        return;
        
    if(!stream(m_buffers[0]))
        return;
 
    if(!stream(m_buffers[1]))
        return;
    
    alSourceQueueBuffers(m_source, 2, m_buffers);
    alSourcePlay(m_source);
}

void SoundStream::pause()
{
    fprintf(stderr, "SoundStream::play is unimplemented!");
}

void SoundStream::stop()
{
    fprintf(stderr, "SoundStream::play is unimplemented!");
}

void SoundStream::update(float dt)
{
    if(!m_loaded)
        return;
    ALenum state;
    alGetSourcei(m_source, AL_SOURCE_STATE, &state);
    if(state != AL_PLAYING)
        return;
    int processed;
    //bool active = true;
 
    alGetSourcei(m_source, AL_BUFFERS_PROCESSED, &processed);
 
    while(processed--)
    {
        ALuint buffer;
        
        alSourceUnqueueBuffers(m_source, 1, &buffer);
 
        /*active = */stream(buffer);
 
        alSourceQueueBuffers(m_source, 1, &buffer);
    }
}

bool SoundStream::stream(ALuint buffer)
{
    char data[STREAM_BUFFER_SIZE];
    int  size = 0;
    int  section;
    int  result;
 
    while(size < STREAM_BUFFER_SIZE)
    {
        result = ov_read(&oggStream, data + size, STREAM_BUFFER_SIZE - size, 0, 2, 1, & section);
    
        if(result > 0)
            size += result;
        else
            if(result < 0)
                throw "";
            else
                break;
    }
    
    if(size == 0)
        return false;
 
    alBufferData(buffer, m_format, data, size, vorbisInfo->rate);
 
    return true;
}
