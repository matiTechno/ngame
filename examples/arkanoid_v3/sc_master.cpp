#include "sc_master.hpp"
#include "sc_level.hpp"
#include <NGAME/app.hpp>
#include <string>
#include <fstream>

Sc_master* Sc_master::handle = nullptr;

Sc_master::Sc_master(const glm::ivec2& win_size):
    win_size(win_size)
{
    assert(handle == nullptr);
    handle = this;
    set_new_scene<Sc_level>();

    std::random_device rd;
    rn_eng.seed(rd());

    update_when_not_top = true;
}

void Sc_master::update()
{
    if(SDL_GetWindowFlags(io.win) & (SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_MAXIMIZED))
        return;
    SDL_GetWindowSize(io.win, &win_size.x, &win_size.y);
}

void Sc_master::quit_and_save()
{
    std::ofstream file("arkanoid_v3.ini");
    if(!file)
    {
        quit();
        return;
    }

    auto flags = SDL_GetWindowFlags(io.win);
    std::string mode;
    if(flags & SDL_WINDOW_FULLSCREEN_DESKTOP)
        mode = "fullscreen";
    else if(flags & SDL_WINDOW_MAXIMIZED)
        mode = "maximized";
    else
        mode = "windowed";

    file << mode << '\n' << win_size.x << ' ' << win_size.y;

    quit();
}

Win_ini::Win_ini():
    //size(Sc_level::vg_size)
    size(800, 600)
{}

Win_ini load_win_ini()
{
    Win_ini win_ini;
    std::ifstream file("arkanoid_v3.ini");
    if(!file)
        return win_ini;

    std::string str;
    file >> str;
    file >> win_ini.size.x;
    file >> win_ini.size.y;

    if(str == "fullscreen")
        win_ini.flags = SDL_WINDOW_FULLSCREEN_DESKTOP;
    else if(str == "maximized")
        win_ini.flags = SDL_WINDOW_MAXIMIZED;

    return win_ini;
}
