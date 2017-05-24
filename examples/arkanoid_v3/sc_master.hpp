#pragma once
#include <NGAME/scene.hpp>

class Sc_master: public Scene
{
public:
    Sc_master();

private:
    friend class Sc_level;
    static const Sc_master* handle;
};
