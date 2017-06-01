#pragma once
#include <NGAME/scene.hpp>
#include <random>
#include<NGAME/gl/texture.hpp>

class Sc_master: public Scene
{
public:
    Sc_master(const glm::ivec2& win_size);

    void start() override;

    void render() override;

private:
    friend class Sc_level;
    friend class Ball;
    static Sc_master* handle;
    std::mt19937 rn_eng;
    glm::ivec2 win_size;
    Texture tex_back;

    void quit_and_save();
};

struct Win_ini
{
    Win_ini();
    int flags = 0;
    glm::ivec2 size;
};

Win_ini load_win_ini();
