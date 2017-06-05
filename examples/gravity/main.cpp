#include <NGAME/app.hpp>
#include "gravity.hpp"
#include <iostream>

int main()
{
    try
    {
        App app(800, 800, "gravity", true, 4, 3, SDL_WINDOW_MAXIMIZED | SDL_WINDOW_RESIZABLE);
        app.start<Gravity>();
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
