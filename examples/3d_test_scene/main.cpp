#include <iostream>
#include <NGAME/app.hpp>
#include "test_scene.hpp"

int main()
{
    try
    {
        App app(600, 600, "3d", true, 3, 3, SDL_WINDOW_FULLSCREEN_DESKTOP);
        app.start<Test_scene>();
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
