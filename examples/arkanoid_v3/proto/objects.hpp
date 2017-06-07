#pragma once
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
class Texture;
class Renderer2d;
struct Paddle;
#include <glm/gtc/constants.hpp>
struct Collision;

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
    bool is_destroyed;
    Texture* texture;
    glm::ivec4 tex_coords;

    void render(const Renderer2d& renderer) const;
};

struct Life_bar
{
    static constexpr float quad_size = 35.f;
    static constexpr float spacing = 10.f;

    glm::vec2 pos;
    int lifes;
    glm::vec4 color{1.f, 1.f, 1.f, 1.f};
    float time;
    Texture* texture;

    void update(float dt);
    void render(const Renderer2d& renderer) const;
};

struct Ball
{
    static constexpr float radius = 20.f;
    static constexpr float max_immune = 0.3f;
    //static constexpr glm::vec2 init_vel{600.f, 0.f};
    static const glm::vec2 init_vel;

    glm::vec2 pos;
    glm::vec2 vel;
    Texture* texture;
    bool is_stuck;
    float immune_time;

    void spawn(const Paddle& paddle);
    void update(float dt);
    void render(const Renderer2d& renderer) const;
};

struct Paddle: public Bbox
{
    static constexpr float vel_len = 480.f;
    static constexpr float min_angle = glm::pi<float>() / 8.f;

    float vel;

    void update(float dt, Ball& ball);
    void render(const Renderer2d& renderer) const;
    void reflect(Ball& ball, const Collision& coll);
};
