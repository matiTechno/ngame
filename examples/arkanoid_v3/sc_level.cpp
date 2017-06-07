#include "sc_level.hpp"
#include <NGAME/common.hpp>
#include "sc_master.hpp"
#include "proto/collisions.hpp"

//constexpr glm::vec2 Sc_level::vg_start;
//constexpr glm::vec2 Sc_level::vg_size;
const glm::vec2 Sc_level::vg_start{0.f};
const glm::vec2 Sc_level::vg_size{1000.f, 800.f};
const float Sc_level::vg_aspect = vg_size.x / vg_size.y;

Sc_level::Sc_level():
    shader("res/wave.sh", false),
    tex_ball("res/ball.png"),
    tex_brick("res/bricks.png"),
    music("res/Blind Shift.mp3"),
    emitter(Sc_master::handle->rn_eng),
    tex_heart("res/heart.png")
{
    is_opaque = false;
    music.set_volume(mus_vol);
    music.play(true);

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
    emitter.reserve(1.f / emitter.spawn_time * emitter.life_max);

    walls.resize(3);
    walls[0].pos = glm::vec2(0.f);
    walls[0].size = glm::vec2(vg_size.x, Wall::width);
    walls[1].pos = glm::vec2(0.f, Wall::width);
    walls[1].size = glm::vec2(Wall::width, vg_size.y - Wall::width);
    walls[2].pos = glm::vec2(vg_size.x - Wall::width, Wall::width);
    walls[2].size = glm::vec2(Wall::width, vg_size.y - Wall::width);

    life_bar.texture = &tex_heart;

    ball.texture = &tex_ball;

    paddle.size = glm::vec2(150.f, 40.f);

    set_level();
}

void Sc_level::set_level()
{
    for(auto& brick: bricks)
        brick.is_destroyed = false;

    life_bar.lifes = 3;

    paddle.pos.x = vg_size.x / 2.f - paddle.size.x / 2.f;
    paddle.pos.y = 700.f - paddle.size.y / 2.f;

    ball.vel = Ball::init_vel;
    ball.spawn(paddle);
    ball.immune_time = 0.f;
}

void Sc_level::start()
{
    proj_size = vg_size;
    if(vg_aspect < io.aspect)
        proj_size.x = io.aspect * proj_size.y;

    else if(vg_aspect > io.aspect)
        proj_size.y = proj_size.x / io.aspect;

    proj_start = vg_start - (proj_size - vg_size) / 2.f;
}

void Sc_level::process_input()
{
    pressed_keys2.clear();

    for(auto& event: io.events)
    {
        auto key = event.key.keysym.sym;

        if(event.type == SDL_QUIT)
            Sc_master::handle->quit_and_save();
        else if(event.type == SDL_KEYDOWN)
        {
            pressed_keys.emplace(key);
            pressed_keys2.emplace(key);

        }
        else if(event.type == SDL_KEYUP)
            pressed_keys.erase(key);
    }

    paddle.vel = 0.f;

    if(io.imgui_wants_input)
        return;

    if(was_pressed(SDLK_g))
        show_debug = !show_debug;

    if(is_pressed(SDLK_a) || is_pressed(SDLK_LEFT) || was_pressed(SDLK_a) || was_pressed(SDLK_LEFT))
        paddle.vel -= Paddle::vel_len;

    if(is_pressed(SDLK_d) || is_pressed(SDLK_RIGHT) || was_pressed(SDLK_d) || was_pressed(SDLK_RIGHT))
        paddle.vel += Paddle::vel_len;

    if(was_pressed(SDLK_SPACE))
        ball.is_stuck = false;
}

void Sc_level::update()
{
    auto dt = io.frametime > 0.020f ? 0.020f : io.frametime;

    paddle.update(dt, ball);
    ball.update(dt);

    glm::vec2 ball_correct(0.f);

    for(auto& wall: walls)
    {
        auto coll = get_collision(paddle, wall);
        if(coll.is)
        {
            paddle.pos -= coll.pene_vec;
            if(ball.is_stuck)
                ball.pos -= coll.pene_vec;
        }
        auto coll2 = get_collision(ball, wall);
        if(coll2.is)
        {
            reflect_vel(ball, coll2);
            if(glm::abs(coll2.pene_vec.x) > glm::abs(ball_correct.x))
                ball_correct.x -= coll2.pene_vec.x;
            if(glm::abs(coll2.pene_vec.y) > glm::abs(ball_correct.y))
                ball_correct.y -= coll2.pene_vec.y;
        }
    }
    Bbox ball_bb;
    ball_bb.pos = ball.pos;
    ball_bb.size = glm::vec2(ball.radius * 2.f);
    for(auto& brick: bricks)
    {
        if(brick.is_destroyed)
            continue;

        if(is_collision(ball_bb, brick) == 0)
            continue;

        auto coll = get_collision(ball, brick);
        if(coll.is)
        {
            brick.is_destroyed = true;

            reflect_vel(ball, coll);
            if(glm::abs(coll.pene_vec.x) > glm::abs(ball_correct.x))
                ball_correct.x -= coll.pene_vec.x;
            if(glm::abs(coll.pene_vec.y) > glm::abs(ball_correct.y))
                ball_correct.y -= coll.pene_vec.y;
        }
    }
    if(ball.immune_time <= 0.f)
    {
        auto coll = get_collision(ball, paddle);
        if(coll.is)
        {
            paddle.reflect(ball, coll);
            ball.pos -= coll.pene_vec;
            ball.immune_time = Ball::max_immune;
        }
    }

    ball.pos += ball_correct;

    if(ball.pos.y > proj_start.y + proj_size.y)
    {
        life_bar.lifes -= 1;
        if(life_bar.lifes <= 0)
        {}
        ball.spawn(paddle);
    }

    for(auto& brick: bricks)
    {
        if(!brick.is_destroyed)
            return;
    }
}

void Sc_level::set_coords()
{
    size.x = io.w;
    size.y = io.h;
}

void Sc_level::render()
{
    if((io.win_flags & SDL_WINDOW_FULLSCREEN_DESKTOP) && !show_debug)
        SDL_ShowCursor(0);
    else
        SDL_ShowCursor(1);

    // render game
    renderer2d.set_projection(proj_start, proj_size);
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
    // game objects
    {
        {
            emitter.spawn_size = glm::vec2(proj_size.x, 20.f);
            emitter.spawn_pos = glm::vec2(proj_start.x, proj_start.y + proj_size.y - emitter.spawn_size.y);
            emitter.update(io.frametime);
            emitter.render(renderer2d);

            for(auto& wall: walls)
                wall.render(renderer2d);

            paddle.render(renderer2d);

            ball.render(renderer2d);

            for(auto& brick: bricks)
            {
                if(brick.is_destroyed == 0)
                    brick.render(renderer2d);
            }

            life_bar.update(io.frametime);
            life_bar.pos = proj_start + glm::vec2(40.f);
            life_bar.render(renderer2d);
        }
    }

    renderer2d.flush();

    // imgui
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
        ImGui::Spacing();
        if(ImGui::Button("quit"))
            Sc_master::handle->quit_and_save();
        ImGui::End();
    }
}

bool Sc_level::is_pressed(int key) const
{
    auto it = pressed_keys.find(key);
    if(it == pressed_keys.end())
        return false;
    return true;
}

bool Sc_level::was_pressed(int key) const
{
    auto it = pressed_keys2.find(key);
    if(it == pressed_keys2.end())
        return false;
    return true;
}
