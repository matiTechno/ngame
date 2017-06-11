#include "snake.hpp"
#include <glm/vec4.hpp>
#include <NGAME/app.hpp>
#include "menu.hpp"
#include <NGAME/sprite.hpp>
#include <NGAME/renderer2d.hpp>
#include <NGAME/font_loader.hpp>

constexpr int Snake::num_grids;
constexpr float Snake::grid_size;

Snake::Snake():
    Vspace(0.f, 0.f, num_grids * grid_size, num_grids * grid_size),
    sample("laser3.mp3"),
    font(font_loader.load_font("ProggyCleanCE.ttf", 60))
{
    std::random_device rd;
    rn_eng.seed(rd());
    snake_nodes.reserve(num_grids * num_grids);
    init_level();
}

void Snake::process_input()
{
    for(auto& event: io.events)
    {
        if(event.type == SDL_KEYDOWN)
        {
            auto key = event.key.keysym.sym;
            if((key == SDLK_a || key == SDLK_LEFT) && !used_vel.x)
            {
                vel.x = -1;
                vel.y = 0;
            }
            else if((key == SDLK_d || key == SDLK_RIGHT) && !used_vel.x)
            {
                vel.x = 1;
                vel.y = 0;
            }
            else if((key == SDLK_w || key == SDLK_UP) && !used_vel.y)
            {
                vel.y = -1;
                vel.x = 0;
            }
            else if((key == SDLK_s || key == SDLK_DOWN) && !used_vel.y)
            {
                vel.y = 1;
                vel.x = 0;
            }
            else if(key == SDLK_ESCAPE)
                set_new_scene<Menu>(*this, true, score, font);
        }
    }
}

void Snake::update()
{
    food_time_left -= io.frametime;
    if(food_time_left <= 0.f)
        spawn_food();

    accumulator += io.frametime;
    while(accumulator >= dt)
    {
        accumulator -= dt;
        {
            used_vel = vel;
            for(int i = snake_nodes.size() - 1; i > 0; --i)
            {
                if(snake_nodes[i].delay == 0)
                    snake_nodes[i].pos = snake_nodes[i - 1].pos;
                else
                    --snake_nodes[i].delay;
            }
            auto& head = snake_nodes[0];
            head.pos += vel;
            if(head.pos.x < 0)
                head.pos.x = num_grids - 1;
            else if(head.pos.y < 0)
                head.pos.y = num_grids - 1;
            else if(head.pos.x == num_grids)
                head.pos.x = 0;
            else if(head.pos.y == num_grids)
                head.pos.y = 0;

            for(int i = 1; i < snake_nodes.size(); ++i)
                if(is_coll(head.pos, snake_nodes[i].pos))
                    set_new_scene<Menu>(*this, false, score, font);

            if(is_coll(head.pos, food_pos))
            {
                ++score;
                sample.play();
                spawn_food();
                snake_nodes.push_back(Node{head.pos, static_cast<int>(snake_nodes.size())});
            }
        }
    }
}

void Snake::render2()
{
    render_grid(num_grids, num_grids, 1.f, glm::vec4(1.f, 1.f, 1.f, 0.15f));
    glm::ivec4 dum1;
    glm::ivec2 dum2;
    glm::vec4 color;
    for(auto it = snake_nodes.rbegin(); it != snake_nodes.rend(); ++it)
    {
        auto& node = *it;

        if(node.delay > 0)
            color = food_color;
        else
            color = glm::vec4(0.f, 1.f, 0.f, 0.4f);

        renderer2d.render(glm::vec2(node.pos) * grid_size, glm::vec2(grid_size),
                          dum1, nullptr, 0.f, dum2, color);
    }
    renderer2d.render(glm::vec2(food_pos) * grid_size, glm::vec2(grid_size),
                      dum1, nullptr, 0.f, dum2, food_color);
    renderer2d.flush();
}

void Snake::spawn_food()
{
    std::uniform_int_distribution<int> dist(0, num_grids - 1);
again:
    food_pos.x = dist(rn_eng);
    food_pos.y = dist(rn_eng);
    for(auto& node: snake_nodes)
        if(is_coll(food_pos, node.pos))
            goto again;

    food_time_left = food_spawn_time;
}

void Snake::init_level()
{
    score = 0;
    accumulator = 0.f;
    snake_nodes.clear();
    std::uniform_int_distribution<int> dist(0, num_grids - 1);
    Node node{glm::ivec2(dist(rn_eng), dist(rn_eng)), 0};
    snake_nodes.push_back(node);
    vel = glm::ivec2(0, -1);
    used_vel = glm::ivec2(0);
    spawn_food();
}

bool Snake::is_coll(const glm::ivec2& pos1, const glm::ivec2& pos2) const
{
    return pos1.x == pos2.x && pos1.y == pos2.y;
}
