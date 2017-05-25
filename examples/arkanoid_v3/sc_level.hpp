#pragma once
#include <NGAME/scene.hpp>
#include <glm/vec2.hpp>
#include <NGAME/gl/texture.hpp>
#include <NGAME/gl/shader.hpp>
#include <vector>
#include "proto/objects.hpp"
#include <NGAME/sound.hpp>
// temp (for emitter class)
#include "../test_scene/test_scene.hpp"
#include <set>

// most important thing todo:
// entity component system with metaprogramming
// (reflection && introspection with imgui)
// (no virtual functions && flat memory)
class Sc_level: public Scene
{
public:
    Sc_level();

    void process_input() override;

    void update() override;

    void set_coords() override;

    void render() override;

    // vg = virtual game area, minimal visible rect
    // this is optimal resolution
    static constexpr glm::vec2 vg_start{0.f};
    static constexpr glm::vec2 vg_size{1000.f, 800.f};
    static constexpr float vg_aspect = vg_size.x / vg_size.y;

private:
    Texture tex_back;
    bool vsync = true;
    std::set<int> pressed_keys;
    bool is_pressed(int key) const;
    std::set<int> pressed_keys2;
    bool was_pressed(int key) const;
    glm::vec2 proj_start, proj_size;

    // prototyping stuff
    void init_proto();

    Shader shader;
    float time = 0.f;
    std::vector<Wall> walls;
    Paddle paddle;
    Ball ball;
    Life_bar life_bar;
    std::vector<Brick> bricks;
    Texture tex_ball;
    Texture tex_brick;
    Music music;
    int mus_vol = 12;
    Emitter emitter;
    bool show_debug = false;
    Texture tex_heart;
};
