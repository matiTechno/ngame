#pragma once
class Del;
#include <memory>
#include <string>
struct Mix_Chunk;
typedef struct _Mix_Music Mix_Music;

class Sample
{
public:
    Sample(const std::string& filename, int volume = 128);
    ~Sample();

    void play(int channel_volume = 128);
    void pause();
    void stop();
    void resume();

    int get_volume() const;
    void set_volume(int volume);

private:
    std::unique_ptr<Del> del;
    Mix_Chunk* chunk;
    int channel = -1;
};

class Music
{
public:
    Music(const std::string& filename, int volume = 128);
    ~Music();

    void play(bool loop);

    static void pause();
    static void stop();
    static void resume();

    static bool is_playing();
    static bool is_paused();
    static bool is_stopped();

    int get_volume() const;
    void set_volume(int volume);

private:
    std::unique_ptr<Del> del;
    Mix_Music* music;
};
