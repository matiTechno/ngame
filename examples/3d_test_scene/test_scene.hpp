#pragma once

#include <NGAME/scene.hpp>
#include <NGAME/3d/model.hpp>
#include <NGAME/3d/inst3d.hpp>

class Test_scene: public Scene
{
public:
    Test_scene();

    void render() override;

private:
    Model model;
    Inst3d inst1;
};
