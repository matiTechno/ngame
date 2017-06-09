#include <iostream>
#include <NGAME/app.hpp>
#include "snake.hpp"

int main()
{
    try
    {
        App app(600, 600, "snake", true, 3, 3, SDL_WINDOW_RESIZABLE);
        app.start<Snake>();
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
