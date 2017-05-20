#pragma once
#include <NGAME/scene.hpp>
#include <NGAME/sound.hpp>
#include <NGAME/gl/shader.hpp>
#include <NGAME/gl/texture.hpp>
#include <NGAME/font.hpp>
#include <random>

#include <vector>
#include <NGAME/sprite.hpp>
#include <random>

class Emitter
{
public:
    friend class Test_scene;
    Emitter(std::mt19937& rn_eng);

    void update(float dt);
    void render(const Renderer2d& renderer) const;

    // spawn properties
    glm::vec2 spawn_pos;
    glm::vec2 spawn_size;
    float spawn_time;

    // particles properties
    float size_min, size_max;
    float life_min, life_max;
    glm::vec2 acc_min, acc_max;
    glm::vec2 vel_min, vel_max;
    float ang_vel_min, ang_vel_max; // angular velocity
    glm::vec4 color_min, color_max;

private:
    class Particle: public Sprite
    {
    public:
        float life;
        glm::vec2 vel;
        glm::vec2 acc;
        float ang_vel;
    };

    std::vector<Particle> particles;
    int last_active = -1;
    float accumulator = 0.f;
    std::mt19937& rn_eng;

    void spawn();
    void kill(int index);
};

class Test_scene: public Scene
{
public:
    Test_scene();

    void process_input() override;

    void set_coords() override;

    void render() override;

private:
    Sample sample;
    bool vsync = true;
    Shader shader;
    Texture texture;
    float time = 0.f;
    Font font;
    std::mt19937 rn_eng;
    Emitter emitter;
};
