#pragma once

#include <NGAME/scene.hpp>
#include <NGAME/3d/model.hpp>
#include <NGAME/3d/inst3d.hpp>
#include <NGAME/font.hpp>
#include <NGAME/3d/renderer3d.hpp>
#include <vector>

class Test_scene: public Scene
{
public:
    Test_scene();

    void render() override;

private:
    Model monkey_mod, cube_mod;
    Inst3d cube;
    Inst3d monkey;
    Font font;
    bool vsync = true;
    std::vector<Light> lights;
    float time = 0.f;
};
