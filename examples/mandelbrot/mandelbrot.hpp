#pragma once

#include <NGAME/gl/shader.hpp>
#include <NGAME/vspace.hpp>

// todo: better colors
class Mandelbrot: public Vspace
{
public:
    Mandelbrot();

    void process_input() override;

private:
    Shader shader;
    int iterations = 150;
    bool imgui = false;
    bool pilot = true;
    bool vsync = true;
    bool is_rmb = false;
    float zoom_coeff = 1.1f;
    glm::ivec2 prev_cursor_pos;
    int sign = 1;

    void render2() override;
};
