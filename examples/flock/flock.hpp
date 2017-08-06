#pragma once
#include <NGAME/guruscene.hpp>
class Renderer2d;
namespace guru {class Guru;}
#include <vector>

struct Wall
{
    glm::vec2 pos;
    glm::vec2 size;
    void render(const Renderer2d& renderer);
};

struct Circle
{
    glm::vec2 pos; /*center*/
    float radius;
};

struct Obstacle: Circle
{
    void render(guru::Guru& guru);
};

struct Boid: Circle
{
    glm::vec2 vel;
    glm::vec2 acc;

    static constexpr float maxVel = 50.f;
    static constexpr float maxAcc = 40.f;
    static constexpr float maxTurnRate = 6.28f;

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
