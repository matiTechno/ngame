#include "sc_level.hpp"
#include <NGAME/common.hpp>

// glm::operator is not constexpr
constexpr glm::vec2 Sc_level::vg_start;

Sc_level::Sc_level():
    tex_back("res/jupiter.jpg"),
    shader("res/wave.sh", false)
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
        renderer2d.flush();
        pp_unit.start();

        Sprite sprite;
        sprite.pos = vg_start;
        sprite.size = vg_size;
        sprite.color = glm::vec4(1.f, 0.f, 0.f, 0.8f);
        renderer2d.render(sprite);

        time += io.frametime;
        shader.bind();
        glUniform1f(shader.get_uni_location("time"), time);
        glm::ivec4 pp_scene_coords(pos.x, get_gl_y(), size);
        glUniform4iv(shader.get_uni_location("scene_coords"), 1, &pp_scene_coords.x);

        renderer2d.flush();
        pp_unit.render(true, shader);
    }
    // prototyping stuff
    {
        {
            Sprite sprite;
            sprite.color = glm::vec4(0.f, 1.f, 0.f, 0.4f);
            sprite.pos = glm::vec2(0.f);
            sprite.size = glm::vec2(vg_size.x, wall_width);
            renderer2d.render(sprite);

            sprite.pos = glm::vec2(0.f, wall_width);
            sprite.size = glm::vec2(wall_width, vg_size.y - wall_width);
            renderer2d.render(sprite);

            sprite.pos = glm::vec2(vg_size.x - wall_width, wall_width);
            sprite.size = glm::vec2(wall_width, vg_size.y - wall_width);
            renderer2d.render(sprite);

            sprite.color = glm::vec4(1.f, 1.f, 1.f, 0.6f);
            sprite.size = paddle_size;
            sprite.pos.x = vg_size.x / 2.f - paddle_size.x / 2.f;
            sprite.pos.y = paddle_pos_y - paddle_size.y / 2.f;
            renderer2d.render(sprite);
        }
    }

    renderer2d.flush();

    // debug
    {
        ImGui::Begin("control");
        ImGui::Text("fb_size: %d x %d", io.w, io.h);
        ImGui::Spacing();
        ImGui::Text("frametime(ms): %.3f", io.av_frametime * 1000.f);
        ImGui::Spacing();
        if(ImGui::Button("vsync on / off"))
        {
            vsync = !vsync;
            SDL_GL_SetSwapInterval(vsync);
        }
        ImGui::End();
    }
}
