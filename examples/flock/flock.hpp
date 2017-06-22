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

struct Obstacle
{
    glm::vec2 pos;
    float radius;
    void render(guru::Guru& guru);
};

struct Boid
{
    glm::vec2 vel;
    glm::vec2 pos; /*center*/
    float radius;
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
