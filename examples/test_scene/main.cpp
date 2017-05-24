#include <NGAME/app.hpp>
#include <iostream>
#include "test_scene.hpp"
#include <SDL2/SDL_mixer.h>

int main() {
    try
    {
        App app(960, 720, "test", true, SDL_WINDOW_MAXIMIZED, MIX_INIT_MP3);
        app.start<Test_scene>();
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
