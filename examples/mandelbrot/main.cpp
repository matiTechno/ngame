#include <NGAME/app.hpp>
#include "mandelbrot.hpp"
#include <iostream>

int main()
{
    try
    {
        App app(800, 600, "mandelbrot", true, 3, 3, SDL_WINDOW_FULLSCREEN_DESKTOP);
        app.start<Mandelbrot>();
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
