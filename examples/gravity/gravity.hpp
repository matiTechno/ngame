#pragma once

#include <NGAME/vspace.hpp>
#include <NGAME/gl/shader.hpp>
#include <NGAME/gl/buffers.hpp>
#include <NGAME/font.hpp>

class Gravity: public Vspace
{
public:
    Gravity();

    void process_input() override;

    void update() override;

private:
    Shader sh_points, sh_gravity;
    VAO vao;
    BO bo_pos, bo_vel;
    static constexpr int num_particles = 1000000;
    bool vsync = true;
    bool imgui = false;
    bool pilot = true;
    bool is_lmb = false;
    bool is_rmb = false;
    float zoom_coeff = 1.05f;
    glm::ivec2 prev_cursor_pos;
    Font font;

    void render2() override;
};
