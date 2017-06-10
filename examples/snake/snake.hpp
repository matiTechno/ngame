#pragma once

#include <NGAME/vspace.hpp>
#include <random>
#include <vector>
#include <NGAME/sound.hpp>
#include <glm/vec4.hpp>
#include <NGAME/font.hpp>

struct Node
{
    glm::ivec2 pos;
    int delay;
};

class Snake: public Vspace
{
public:
    Snake();

    void process_input() override;

    void update() override;

private:
    static constexpr int num_grids = 15;
    static constexpr float grid_size = 10;
    const float food_spawn_time = 6.f;
    const float dt = 0.2f;
    const glm::vec4 food_color{1.f, 1.f, 0.f, 0.6f};

    Sample sample;
    Font font;
    int score;
    float accumulator;
    std::mt19937 rn_eng;
    glm::ivec2 food_pos;
    float food_time_left;
    std::vector<Node> snake_nodes;
    glm::ivec2 vel;
    glm::ivec2 used_vel;

    void render2() override;

    void spawn_food();

    friend class Menu;
    void init_level();

    bool is_coll(const glm::ivec2& pos1, const glm::ivec2& pos2) const;
};
