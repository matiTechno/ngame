#include <NGAME/3d/renderer3d.hpp>
#include <NGAME/3d/model.hpp>

Renderer3d::Renderer3d():
    shader(
        #include "3d.sh"
        , "3d.sh"
        )
{}

void Renderer3d::render(const Model& model)
{
    (void)model;
}
