#include <NGAME/app.hpp>
#include <iostream>
#include "sc_master.hpp"

int main()
{
    try
    {
        App app(800, 600, "arkanoid_v3", false);
        app.start<Sc_master>();
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
