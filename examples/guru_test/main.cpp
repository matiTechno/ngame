#include <iostream>
#include <NGAME/app.hpp>
#include "testscene.hpp"

int main()
{
    try
    {
        App app(600, 600, "guru test", true, 3, 3, SDL_WINDOW_FULLSCREEN_DESKTOP);
        app.start<TestScene>();
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
