#pragma once
#include <NGAME/scene.hpp>
#include <glm/vec2.hpp>
#include <NGAME/gl/texture.hpp>

class Sc_level: public Scene
{
public:
    Sc_level();

    void process_input() override;

    void update() override;

    void set_coords() override;

    void render() override;

private:
    // vg = virtual game area
    static constexpr glm::vec2 vg_start{0.f};
    static constexpr glm::vec2 vg_size{1000.f, 800.f};
    static constexpr float vg_aspect = vg_size.x / vg_size.y;

    Texture tex_back;
};
