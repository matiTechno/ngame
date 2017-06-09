#include "snake.hpp"
#include <glm/vec4.hpp>
#include <NGAME/app.hpp>
#include "menu.hpp"
#include <NGAME/sprite.hpp>
#include <NGAME/renderer2d.hpp>

constexpr int Snake::num_grids;
constexpr int Snake::grid_size;

Snake::Snake():
    Vspace(0.f, 0.f, num_grids * grid_size, num_grids * grid_size)
{}

void Snake::process_input()
{
    for(auto& event: io.events)
    {
        if(event.type == SDL_KEYDOWN)
        {
            auto key = event.key.keysym.sym;
            if(key == SDLK_a || key == SDLK_LEFT)
                vel.x = -1;
            else if(key == SDLK_d || key == SDLK_RIGHT)
                vel.x = 1;
            else if(key == SDLK_w || key == SDLK_UP)
                vel.y = -1;
            else if(key == SDLK_s || key == SDLK_DOWN)
                vel.y = 1;

            else if(key == SDLK_ESCAPE)
                set_new_scene<Menu>("game paused", score, false);
        }
    }
}

void Snake::update()
{
    accumulator += io.frametime;
    while(accumulator >= dt)
    {
        accumulator -= dt;
    }
}

void Snake::render2()
{
    render_grid(num_grids, num_grids, 2.f, glm::vec4(1.f, 0.f, 0.f, 1.f));
}
