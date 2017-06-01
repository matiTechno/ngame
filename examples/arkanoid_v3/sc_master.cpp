#include "sc_master.hpp"
#include "sc_level.hpp"
#include <NGAME/app.hpp>
#include <string>
#include <fstream>
#include <NGAME/renderer2d.hpp>

Sc_master* Sc_master::handle = nullptr;

Sc_master::Sc_master(const glm::ivec2& win_size):
    win_size(win_size),
    tex_back("res/jupiter.jpg")
{
    assert(handle == nullptr);
    handle = this;

    std::random_device rd;
    rn_eng.seed(rd());

    set_new_scene<Sc_level>();
}

void Sc_master::start()
{
    size.x = io.w;
    size.y = io.h;

    if(io.win_flags & (SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_MAXIMIZED))
        return;
    win_size.x = io.w;
    win_size.y = io.h;
}

void Sc_master::render()
{
    // render background texture
    renderer2d.set_projection(glm::vec2(0.f), size);
    {
        Sprite sprite;
        sprite.pos = glm::vec2(0.f);
        sprite.size = size;
        sprite.texture = &tex_back;

        auto tex_size = tex_back.get_size();
        if(tex_size.x >= size.x && tex_size.y >= size.y)
        {
            glm::ivec2 c_pos = tex_size / 2 - size / 2;
            sprite.tex_coords = glm::ivec4(c_pos, size);
        }
        else
        {
            auto tex_aspect = float(tex_size.x) / tex_size.y;
            glm::ivec4 tex_coords(0, 0, tex_size);

            if(io.aspect > tex_aspect)
            {
                tex_coords.w = tex_size.x / io.aspect;
                tex_coords.y = (tex_size.y - tex_coords.w) / 2;
            }
            else if(io.aspect < tex_aspect)
            {
                tex_coords.z = tex_size.y * io.aspect;
                tex_coords.x = (tex_size.x - tex_coords.z) / 2;
            }

            sprite.tex_coords = tex_coords;
        }
        renderer2d.render(sprite);
        renderer2d.flush();
    }
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
