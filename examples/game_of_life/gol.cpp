#include "gol.hpp"
#include <glm/vec4.hpp>
#include <NGAME/pp_unit.hpp>
#include <NGAME/sprite.hpp>
#include <NGAME/renderer2d.hpp>
#include <NGAME/app.hpp>
#include <NGAME/glad.h>
#include <fstream>

constexpr int Matrix::size;
constexpr float Matrix::grid_size;

Gol::Gol():
    Vspace(0, 0, matrix.size * matrix.grid_size, matrix.size * matrix.grid_size),
    texture("res/saturn.jpg"),
    sh_pp_pass("res/pp_pass.sh", false),
    sh_wave("res/wave.sh", false)
{
   SDL_ShowCursor(0);
}

void Gol::render()
{
    static constexpr float time_step = 0.1f;
    accumulator += io.frametime;
    while(accumulator >= time_step)
    {
        matrix.update();
        accumulator -= time_step;
    }

    render_background(texture);

    pp_unit.start();
    {
        Sprite sprite;
        sprite.pos = get_pstart();
        sprite.size = get_psize();
        sprite.color = glm::vec4(0.f, 1.f, 0.f, 0.7f);
        renderer2d.render(sprite);
        renderer2d.flush();
    }
    time += io.frametime;
    sh_wave.bind();
    glUniform1f(sh_wave.get_uni_location("time"), time);
    glUniform4i(sh_wave.get_uni_location("scene_coords"), 0, 0, size.x, size.y);
    pp_unit.render(true, sh_wave);

    matrix.render(renderer2d);

    pp_unit.start();
    render_grid(matrix.size, grid_width, glm::vec4(0.f, 0.f, 0.f, 0.3f));
    pp_unit.render(true, sh_pp_pass);
}

void Matrix::update()
{
    for(int i = 1; i < size - 1; ++i)
        for(int j = 1; j < size - 1; ++j)
        {
            auto val = probe(i, j);

            if(matrix[i][j].current)
            {
                if(val < 2)
                    matrix[i][j].next = false;
                else if(val < 4)
                    matrix[i][j].next = true;
                else
                    matrix[i][j].next = false;
            }
            else if(val == 3)
                matrix[i][j].next = true;
        }

    for(int i = 0; i < size; ++i)
        for(int j = 0; j < size; ++j)
            matrix[j][i].current = matrix[j][i].next;
}

void Matrix::render(const Renderer2d& renderer)
{
    glm::vec4 dum;
    glm::vec2 pos(0.f);
    glm::vec4 color;
    for(int i = 0; i < size; ++i)
    {
        for(int j = 0; j < size; ++j)
        {
            if(matrix[i][j].current)
                color = glm::vec4(0.f, 0.f, 0.f, 0.8f);
            else
                color = glm::vec4(0.f);

            renderer.render(pos, glm::vec2(grid_size), dum, nullptr, 0.f, dum, color);
            pos.x += grid_size;
        }
        pos.x = 0.f;
        pos.y += grid_size;
    }
    renderer.flush();
}

int Matrix::probe(int id_y, int id_x) const
{
    return matrix[id_y - 1][id_x - 1].current + matrix[id_y - 1][id_x].current + matrix[id_y - 1][id_x + 1].current +
            matrix[id_y + 1][id_x - 1].current + matrix[id_y + 1][id_x].current + matrix[id_y + 1][id_x + 1].current +
            matrix[id_y][id_x - 1].current + matrix[id_y][id_x + 1].current;
}

Matrix::Matrix()
{
    std::ifstream file;
    file.open("res/level");
    if(!file)
        throw std::runtime_error("could not load level file");

    int code;
    int x = 0, y = 0;
    while((code = file.get()) != std::ifstream::traits_type::eof())
    {
        if(code == '\n')
        {
            ++y;
            assert(y < size);
            x = 0;
            continue;
        }
        if(code == '1')
            matrix[y][x].current = 1;
        else
            matrix[y][x].current = 0;

        ++x;
        assert(x < size);
    }
}
