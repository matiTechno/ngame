#include <NGAME/app.hpp>
#include <iostream>
#include "animal_sim.hpp"

int main()
{
    try
    {
        App app(600, 600, "animal sim", true, 3, 3, SDL_WINDOW_FULLSCREEN_DESKTOP);
        app.start<Animal_sim>();
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
