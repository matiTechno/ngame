#pragma once
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
class Texture;
class Renderer2d;
struct Paddle;
#include <glm/gtc/constants.hpp>

struct Bbox
{
    glm::vec2 pos;
    glm::vec2 size;
};

struct Wall: public Bbox
{
    static constexpr float width = 50.f;
    void render(const Renderer2d& renderer) const;
};

struct Brick: public Bbox
{
    bool is_destroyed = false;
    Texture* texture;
    glm::ivec4 tex_coords;

    void render(const Renderer2d& renderer) const;
};

struct Life_bar
{
    void init(){lifes = 3;}
    int lifes;
    const float quad_size = 35.f;
    const float spacing = 10.f;
    glm::vec2 pos;
    glm::vec4 color{1.f, 1.f, 1.f, 1.f};
    float time = 0.f;
    Texture* texture;

    void update(float dt);
    void render(const Renderer2d& renderer) const;
};

struct Ball
{
    const float radius = 25.f;
    glm::vec2 pos;
    glm::vec2 vel;
    glm::vec2 init_vel{100.f, -200.f};
    Texture* texture;
    bool is_stuck;
    float immune_time = 0.f;
    float immune_coeff = 0.5f;

    void spawn(const Paddle& paddle);
    void update(float dt);
    void render(const Renderer2d& renderer) const;
};

struct Paddle: public Bbox
{
    float vel = 0.f;
    float vel_coeff = 480.f;
    float max_angle = glm::pi<float>() / 8.f;

    void update(float dt, Ball& ball);
    void render(const Renderer2d& renderer) const;
    void reflect(Ball* ball);
};
