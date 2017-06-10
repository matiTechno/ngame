#include <iostream>
#include <NGAME/app.hpp>
#include "snake.hpp"
#include <SDL2/SDL_mixer.h>

int main()
{
    try
    {
        App app(600, 600, "snake", true, 3, 3, SDL_WINDOW_RESIZABLE, MIX_INIT_MP3);
        app.start<Snake>();
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
