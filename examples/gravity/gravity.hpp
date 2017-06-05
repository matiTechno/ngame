#pragma once

#include <NGAME/vspace.hpp>
#include <NGAME/gl/shader.hpp>
#include <NGAME/gl/buffers.hpp>

class Gravity: public Vspace
{
public:
    Gravity();

    void update() override;

    void render() override;

private:
    Shader sh_points, sh_gravity;
    VAO vao;
    BO bo_pos, bo_vel;
    static constexpr int num_particles = 1000000;
    bool vsync = true;
};
