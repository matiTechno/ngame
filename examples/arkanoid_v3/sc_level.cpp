#include "sc_level.hpp"
#include <NGAME/common.hpp>
#include "sc_master.hpp"

// glm::operator is not constexpr
constexpr glm::vec2 Sc_level::vg_start;
// without this debug build fails
constexpr glm::vec2 Sc_level::vg_size;

Sc_level::Sc_level():
    tex_back("res/jupiter.jpg"),
    shader("res/wave.sh", false),
    tex_ball("res/ball.png"),
    tex_brick("res/bricks.png"),
    music("res/Blind Shift.mp3"),
    emitter(Sc_master::handle->rn_eng)
{
    is_opaque = false;
    init_proto();
    music.set_volume(mus_vol);
    music.play(true);
}

void Sc_level::init_proto()
{
    walls.resize(3);
    walls[0].pos = glm::vec2(0.f);
    walls[0].size = glm::vec2(vg_size.x, Wall::width);
    walls[1].pos = glm::vec2(0.f, Wall::width);
    walls[1].size = glm::vec2(Wall::width, vg_size.y - Wall::width);
    walls[2].pos = glm::vec2(vg_size.x - Wall::width, Wall::width);
    walls[2].size = glm::vec2(Wall::width, vg_size.y - Wall::width);

    paddle.size = glm::vec2(200.f, 40.f);
    paddle.pos.x = vg_size.x / 2.f - paddle.size.x / 2.f;
    paddle.pos.y = 700.f - paddle.size.y / 2.f;

    ball.texture = &tex_ball;
    ball.spawn(paddle, false);

    auto a_start = vg_start + Wall::width;
    glm::vec2 a_size(vg_size.x - 2 * Wall::width, 280.f);
    auto num_bricks_x = 8;
    auto num_bricks_y = 6;
    bricks.reserve(num_bricks_x * num_bricks_y);
    glm::vec2 b_size(a_size.x / num_bricks_x, a_size.y / num_bricks_y);
    auto b_pos = a_start;
    for(auto j = 0; j < num_bricks_y; ++j)
    {
        for(auto i = 0; i < num_bricks_x; ++i)
        {
            Brick brick;
            brick.size = b_size;
            brick.pos = b_pos;
            brick.texture = &tex_brick;
            brick.tex_coords = glm::ivec4(320, 0, 32, 32);
            bricks.push_back(brick);
            b_pos.x += b_size.x;
        }
        b_pos.x = a_start.x;
        b_pos.y += b_size.y;
    }

    emitter.spawn_time = 0.0006f;
    emitter.size_min = 2.f;
    emitter.size_max = 4.f;
    emitter.life_min = 2.f;
    emitter.life_max = 6.f;
    emitter.acc_min = glm::vec2(0.f, 3.f);
    emitter.acc_max = glm::vec2(0.f, 7.f);
    emitter.vel_min = glm::vec2(0.f, -20.f);
    emitter.vel_max = glm::vec2(0.f, -60.f);
    emitter.ang_vel_min = 0.f;
    emitter.ang_vel_max = 0.f;
    emitter.color_min = glm::vec4(1.f, 0.1f, 0.f, 0.2f);
    emitter.color_max = glm::vec4(1.f, 0.7f, 0.1f, 0.6f);
    // ...
    emitter.reserve(1.f / emitter.spawn_time * emitter.life_max);
}

void Sc_level::process_input()
{
    for(auto& event: io.events)
    {
        if(event.type == SDL_QUIT)
            Sc_master::handle->quit_and_save();
        else if(event.type == SDL_KEYDOWN)
        {
            if(event.key.keysym.sym == SDLK_g)
                show_debug = !show_debug;
        }
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
        //renderer2d.render(sprite);
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
            emitter.spawn_size = glm::vec2(proj_size.x, 20.f);
            emitter.spawn_pos = glm::vec2(proj_start.x, proj_size.y - emitter.spawn_size.y);
            emitter.update(io.frametime);
            emitter.render(renderer2d);

            for(auto& wall: walls)
                wall.render(renderer2d);

            paddle.render(renderer2d);

            ball.render(renderer2d);

            for(auto& brick: bricks)
                brick.render(renderer2d);

            life_bar.update(io.frametime);
            life_bar.pos = proj_start + glm::vec2(40.f);
            life_bar.render(renderer2d);
        }
    }

    renderer2d.flush();

    // debug
    if(show_debug)
    {
        ImGui::Begin("control");
        ImGui::Text("fb_size: %d x %d", io.w, io.h);
        ImGui::Spacing();
        ImGui::Text("frametime(ms): %.3f", io.av_frametime * 1000.f);
        ImGui::Spacing();
        if(ImGui::Button("vsync on/off"))
        {
            vsync = !vsync;
            SDL_GL_SetSwapInterval(vsync);
        }

        ImGui::Spacing();
        ImGui::SliderInt("mus_vol", &mus_vol, 0, 128);

        music.set_volume(mus_vol);

        ImGui::Spacing();
        if(ImGui::Button("fullscreen on/off"))
        {
            SDL_SetWindowFullscreen(io.win, SDL_WINDOW_FULLSCREEN_DESKTOP & SDL_GetWindowFlags(io.win)
                                    ? 0 : SDL_WINDOW_FULLSCREEN_DESKTOP);
        }
        ImGui::End();
    }
}
