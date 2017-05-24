#include "sc_level.hpp"
#include <NGAME/common.hpp>

// glm::operator is not constexpr
constexpr glm::vec2 Sc_level::vg_start;

Sc_level::Sc_level():
    tex_back("res/jupiter.jpg")
{
    is_opaque = false;
}

void Sc_level::process_input()
{
    for(auto& event: io.events)
    {
        if(event.type == SDL_QUIT)
            quit();
    }
}

void Sc_level::update()
{}

void Sc_level::set_coords()
{
    size.x = io.w;
    size.y = io.h;
}

void Sc_level::render()
{
    // render background texture
    renderer2d.set_projection(glm::vec2(0.f), size);
    {
        Sprite sprite;
        sprite.pos = glm::vec2(0.f);
        sprite.size = size;
        sprite.texture = &tex_back;

        auto tex_size = tex_back.get_size();
        if(tex_size.x >= size.x && tex_size.y >= size.y)
        {
            glm::ivec2 c_pos = tex_size / 2 - size / 2;
            sprite.tex_coords = glm::ivec4(c_pos, size);
        }
        else
        {
            auto tex_aspect = float(tex_size.x) / tex_size.y;
            glm::ivec4 tex_coords(0, 0, tex_size);

            if(io.aspect > tex_aspect)
            {
                tex_coords.w = tex_size.x / io.aspect;
                tex_coords.y = (tex_size.y - tex_coords.w) / 2;
            }
            else if(io.aspect < tex_aspect)
            {
                tex_coords.z = tex_size.y * io.aspect;
                tex_coords.x = (tex_size.x - tex_coords.z) / 2;
            }

            sprite.tex_coords = tex_coords;
        }
        renderer2d.render(sprite);
        renderer2d.flush();
    }

    // render game
    auto proj_size = vg_size;
    if(vg_aspect < io.aspect)
        proj_size.x = io.aspect * proj_size.y;

    else if(vg_aspect > io.aspect)
        proj_size.y = proj_size.x / io.aspect;

    auto proj_start = vg_start - (proj_size - vg_size) / 2.f;

    renderer2d.set_projection(proj_start, proj_size);

    // projection area
    {
        Sprite sprite;
        sprite.pos = proj_start;
        sprite.size = proj_size;
        sprite.color = glm::vec4(0.f, 1.f, 0.f, 0.2f);
        renderer2d.render(sprite);
    }
    // virtual game area
    {
        Sprite sprite;
        sprite.pos = vg_start;
        sprite.size = vg_size;
        sprite.color = glm::vec4(1.f, 0.f, 0.f, 0.5f);
        renderer2d.render(sprite);
    }
    renderer2d.flush();
}
