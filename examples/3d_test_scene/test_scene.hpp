#pragma once

#include <NGAME/scene.hpp>
#include <NGAME/3d/model.hpp>
#include <NGAME/3d/inst3d.hpp>
#include <NGAME/font.hpp>
#include <NGAME/3d/renderer3d.hpp>
#include <array>
#include <random>
#include <memory>

struct Particle: public Light
{
    glm::vec3 vel;
    float life;
};

// not implemented with efficiency in mind
// good for small numbers
// ALL values are fixed
class Emitter
{
public:
    Emitter(const glm::vec3& position, std::mt19937& rn_eng);

    void set_lights(const Renderer3d& renderer) const;

    void update(float dt);

private:
    static constexpr int num = 20;
    std::array<Particle, num> particles;
    glm::vec3 position;
    std::mt19937 rn_eng;
    const float life_min = 2.f, life_max = 6.f;

    void spawn(Particle& particle);
};

class Test_scene: public Scene
{
public:
    Test_scene();

    void render() override;

private:
    Model monkey_mod, cube_mod;
    Inst3d cube;
    Inst3d monkey;
    Font font;
    bool vsync = true;
    std::vector<Light> lights;
    float time = 0.f;
    std::mt19937 rn_eng;
    // rn_eng needs to be seeded first
    std::unique_ptr<Emitter> emitter;
    bool imgui = false;
};
