#include <iostream>
#include <NGAME/app.hpp>
#include "editor.hpp"

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        std::cout << "cxxedit <filename>" << std::endl;
        return 0;
    }
    try
    {
        App app(600, 600, ("matiTechno cxxedit: " + std::string(argv[1])).c_str(),
                true, 3, 3, SDL_WINDOW_MAXIMIZED | SDL_WINDOW_RESIZABLE);
        app.start<Editor>(argv[1]);
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}
