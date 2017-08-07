#pragma once
#include <NGAME/guruscene.hpp>
class Renderer2d;
namespace guru {class Guru;}
#include <vector>
#include <glm/vec4.hpp>

struct Rect
{
    glm::vec2 pos;
    glm::vec2 size;
};

struct Circle
{
    glm::vec2 pos; /*center*/
    float radius;
};

struct Wall: public Rect
{
    glm::vec4 color;
    void render(const Renderer2d& renderer);
};

struct Obstacle: public Circle
{
    glm::vec4 color;
    void render(guru::Guru& guru);
};

struct Boid: public Circle
{
    glm::vec2 vel;
    glm::vec2 acc{0.f, 0.f};
    glm::vec4 color;
    static constexpr float sight = 12.f;
    static constexpr float targetVel = 10.f;
    static constexpr float maxVel = 20.f;
    static constexpr float maxAcc = 30.f;
    void render(guru::Guru& guru);
};

class SceneF: public guru::GuruScene
{
public:
    SceneF();

private:
    void worldRender() override;
    void intUpdate();

    Wall walls[4];
    std::vector<Obstacle> obstacles;
    std::vector<Boid> boids;
};
