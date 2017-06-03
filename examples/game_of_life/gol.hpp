#pragma once

#include <NGAME/vspace.hpp>
#include <NGAME/gl/texture.hpp>
#include <NGAME/gl/shader.hpp>
class Renderer2d;

struct State
{
    bool current = false;
    bool next = false;
};

struct Matrix
{
    Matrix();
    static constexpr int size = 40;
    static constexpr float grid_size = 20;
    State matrix[size][size];

    void render(const Renderer2d& renderer);
    void update();

    int probe(int id_y, int id_x) const;
};

class Gol: public Vspace
{
public:
    Gol();

    void render() override;

    static constexpr float grid_width = 1.f;

private:
    Matrix matrix;
    Texture texture;
    Shader sh_pp_pass;
    Shader sh_wave;

    float time = 0.f;
    float accumulator = 0.f;
};
