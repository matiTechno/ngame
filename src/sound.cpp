#include <NGAME/sound.hpp>
#include <NGAME/del.hpp>
#include <SDL2/SDL_mixer.h>
#include <iostream>

Sample::~Sample() = default;
Music::~Music() = default;

Sample::Sample(const std::string& filename, int volume)
{
    chunk = Mix_LoadWAV(filename.c_str());
    if(!chunk)
        throw std::runtime_error(std::string("Mix_LoadWAV failed, filename = " + filename + ": " + Mix_GetError()));

    del = std::make_unique<Del>([this](){

        stop();

        Mix_FreeChunk(chunk);});

    Mix_VolumeChunk(chunk, volume);
}

Music::Music(const std::string& filename, int volume)
{
    music = Mix_LoadMUS(filename.c_str());

    if(!music)
        throw std::runtime_error(std::string("Mix_LoadMUS failed, filename = " + filename + ": " + Mix_GetError()));

    del = std::make_unique<Del>([this](){
        Mix_FreeMusic(music);
    });

    Mix_VolumeMusic(volume);
}

void Sample::play(int channel_volume)
{
    channel = Mix_PlayChannel(-1, chunk, 0);
    if(channel == -1)
    {
        Mix_AllocateChannels(Mix_AllocateChannels(-1) * 2);
        channel = Mix_PlayChannel(-1, chunk, 0);
        if(channel == -1)
        {
            std::cout << "Mix_PlayChannel failed: " << Mix_GetError() << std::endl;
            return;
        }
    }
    Mix_Volume(channel, channel_volume);
}

void Sample::pause()
{
    if(channel != -1 && Mix_GetChunk(channel) == chunk)
        Mix_Pause(channel);
}

void Sample::stop()
{
    if(channel != -1 && Mix_GetChunk(channel) == chunk)
        Mix_HaltChannel(channel);
}

int Sample::get_volume() const
{
    return Mix_VolumeChunk(chunk, -1);
}
void Sample::set_volume(int volume)
{
    Mix_VolumeChunk(chunk, volume);
}

void Sample::resume()
{
    if(channel != -1 && Mix_GetChunk(channel) == chunk)
        Mix_Resume(channel);
}


void Music::play(bool loop)
{
    int loops;
    if(loop)
        loops = -1;
    else
        loops = 1;

    auto error = Mix_PlayMusic(music, loops);
    if(error == -1)
        std::cout << "Mix_PlayMusic failed: " << Mix_GetError() << std::endl;
}

void Music::pause()
{
    if(Mix_PlayingMusic())
        Mix_PauseMusic();
}

void Music::stop()
{
    Mix_HaltMusic();
}

bool Music::is_playing()
{
    return Mix_PlayingMusic() && !Mix_PausedMusic();
}
bool Music::is_paused()
{
    return Mix_PlayingMusic() && Mix_PausedMusic();
}
bool Music::is_stopped()
{
    return !Mix_PlayingMusic();
}

int Music::get_volume() const
{
    return Mix_VolumeMusic(-1);
}
void Music::set_volume(int volume)
{
    Mix_VolumeMusic(volume);
}
