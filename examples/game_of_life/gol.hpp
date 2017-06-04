#pragma once

#include <NGAME/vspace.hpp>
#include <NGAME/gl/texture.hpp>
#include <NGAME/gl/shader.hpp>
class Renderer2d;

struct Matrix
{
    Matrix();
    static constexpr int size = 38;
    static constexpr float grid_size = 20;

    void render(const Renderer2d& renderer);
    void update();

private:
    struct State
    {
        bool current = false;
        bool next = false;
    };

    State matrix[size][size];

    int probe(int id_y, int id_x) const;
};

class Gol: public Vspace
{
public:
    Gol();

    void render() override;

    static constexpr float grid_width = 1.f;
    static constexpr float timestep = 0.1f;

private:
    Matrix matrix;
    Texture texture;
    Shader sh_wave;
    float time = 0.f;
    float accumulator = 0.f;
};
