#pragma once

#include <NGAME/vspace.hpp>
#include <random>
#include <vector>

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
    static constexpr int grid_size = 10;
    const float dt = 0.15f;
    float accumulator = 0.f;
    std::random_device rn_eng;
    glm::ivec2 food_pos;
    static constexpr float food_spawn_time = 4.f;
    float food_time_left;
    std::vector<Node> snake_nodes;
    glm::ivec2 vel;
    int score = 0;

    void render2() override;
};
