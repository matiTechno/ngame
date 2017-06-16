#pragma once

#include <NGAME/vspace.hpp>
#include "render_ext.hpp"

class Animal_sim: public Vspace
{
public:
    Animal_sim();

private:
    Render_ext render_ext;
};
