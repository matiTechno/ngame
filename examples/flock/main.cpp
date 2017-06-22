#include <iostream>
#include <NGAME/app.hpp>
#include "flock.hpp"

int main()
{
    try
    {
        App app(600, 600, "flock", true, 3, 3, SDL_WINDOW_FULLSCREEN_DESKTOP);
        app.start<SceneF>();
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
