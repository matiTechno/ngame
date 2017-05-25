#include <NGAME/app.hpp>
#include <iostream>
#include "sc_master.hpp"
#include <SDL2/SDL_mixer.h>

int main()
{
    try
    {
        auto win_ini = load_win_ini();
        App app(win_ini.size.x, win_ini.size.y, "arkanoid_v3", false, 3, 3,
                SDL_WINDOW_RESIZABLE | win_ini.flags, MIX_INIT_MP3);

        app.start<Sc_master>(win_ini.size);
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
