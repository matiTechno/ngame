#include <iostream>
#include <NGAME/app.hpp>
#include "gol.hpp"

int main()
{
    try
    {
        App app(800, 600, "game of life", true, 3, 3, SDL_WINDOW_FULLSCREEN_DESKTOP);
        app.start<Gol>();
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
