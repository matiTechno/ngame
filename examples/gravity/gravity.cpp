#include "gravity.hpp"
#include <random>
#include <NGAME/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <NGAME/app.hpp>
#include <NGAME/imgui.h>
#include <glm/trigonometric.hpp>
#include <NGAME/blend.hpp>
#include <NGAME/font_loader.hpp>
#include <NGAME/renderer2d.hpp>
#include <NGAME/text.hpp>
#include <NGAME/sprite.hpp>

constexpr int Gravity::num_particles;

Gravity::Gravity():
    Vspace(0.f, 0.f, 100.f, 100.f),
    sh_points("points.sh", false),
    sh_gravity("gravity.sh", false),
    font(font_loader.load_font("ProggyCleanCE.ttf", 20))
{
    // because local group in compute shader == 1000
    assert(num_particles % 1000 == 0);

    std::random_device rd;
    std::mt19937 rn_eng(rd());
    std::uniform_real_distribution<float> dist_x(get_vstart().x, get_vstart().x + get_vrange().x);
    std::uniform_real_distribution<float> dist_y(get_vstart().y, get_vstart().y + get_vrange().y);

    // vectors because stack overflow
    std::vector<glm::vec2> poss(num_particles);
    std::vector<glm::vec2> vels(num_particles);
    for(int i = 0; i < num_particles; ++i)
    {
        poss[i] = glm::vec2(dist_x(rn_eng), dist_y(rn_eng));
        // to avoid dividing by 0 when normalizing vector
        vels[i] = glm::vec2(0.000001f);
    }

    bo_pos.bind(GL_ARRAY_BUFFER);
    glBufferData(GL_ARRAY_BUFFER, num_particles * sizeof(glm::vec2), poss.data(), GL_DYNAMIC_DRAW);

    vao.bind();
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    bo_pos.bind(GL_SHADER_STORAGE_BUFFER);

    bo_vel.bind(GL_SHADER_STORAGE_BUFFER);
    glBufferData(GL_SHADER_STORAGE_BUFFER, num_particles * sizeof(glm::vec2), vels.data(), GL_DYNAMIC_DRAW);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, bo_pos.id);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, bo_vel.id);

    Blend::set(GL_SRC_ALPHA, GL_ONE);
}

void Gravity::process_input()
{
    float scroll = 0.f;

    for(auto& event: io.events)
    {
        auto mb = event.button;
        if(event.type == SDL_MOUSEBUTTONDOWN)
        {
            if(mb.button == SDL_BUTTON_LEFT)
                is_lmb = true;

            else if(mb.button == SDL_BUTTON_RIGHT)
                is_rmb = true;
        }
        else if(event.type == SDL_MOUSEBUTTONUP)
        {
            if(mb.button == SDL_BUTTON_LEFT)
                is_lmb = false;

            else if(mb.button == SDL_BUTTON_RIGHT)
                is_rmb = false;
        }
        else if(event.type == SDL_MOUSEWHEEL)
            scroll += event.wheel.y;

        else if(event.type == SDL_KEYDOWN)
        {
            auto key = event.key.keysym.sym;
            if(key == SDLK_g)
                imgui = !imgui;
        }
    }

    auto hide_cursor = pilot && !imgui;
    SDL_ShowCursor(!hide_cursor);

    if(io.imgui_wants_input || hide_cursor)
    {
        is_lmb = false;
        is_rmb = false;
        return;
    }

    if(is_rmb)
        move(io.cursor_pos, prev_cursor_pos);

    if(scroll)
    {
        float times = glm::pow(zoom_coeff, scroll);
        if(is_rmb || is_lmb)
            zoom_to_cursor(times, io.cursor_pos);
        else
            zoom_to_center(times);
    }
}

void Gravity::update()
{
    sh_gravity.bind();
    glUniform1f(sh_gravity.get_uni_location("dt"), io.frametime);

    if(pilot)
    {
        glm::vec2 pos(50.f);
        static float time = 0.f;
        time += io.frametime / 2.f;
        pos.x += 40.f * glm::sin(time * 2.f);
        pos.y += 40.f * glm::cos(time * 2.f);
        glUniform2f(sh_gravity.get_uni_location("gpos"), pos.x, pos.y);
        glUniform1i(sh_gravity.get_uni_location("is_active"), 1);
    }
    else
    {
        auto pos = get_cursor_vs(io.cursor_pos);
        glUniform2f(sh_gravity.get_uni_location("gpos"), pos.x, pos.y);
        glUniform1i(sh_gravity.get_uni_location("is_active"), is_lmb);
    }
    glDispatchCompute(num_particles / 1000, 1, 1);
    glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
}

void Gravity::render()
{
    sh_points.bind();
    auto proj = glm::ortho(get_pstart().x, get_pstart().x + get_prange().x,
                           get_pstart().y + get_prange().y, get_pstart().y);

    glUniformMatrix4fv(sh_points.get_uni_location("projection"), 1, GL_FALSE, &proj[0][0]);
    vao.bind();
    glDrawArrays(GL_POINTS, 0, num_particles);

    if(imgui)
    {
        ImGui::Begin("control");
        ImGui::Text("frametime(ms): %.3f", io.av_frametime * 1000.f);
        if(ImGui::Button("vsync on/off"))
        {
            vsync = !vsync;
            SDL_GL_SetSwapInterval(vsync);
        }
        if(ImGui::Button("pilot on/off"))
            pilot = !pilot;

        ImGui::Text("lmb - create gravity\n"
                    "rmb - move around\n"
                    "scroll - zoom in/out");

        ImGui::End();
    }
    prev_cursor_pos = io.cursor_pos;

    Blend::set_default();
    renderer2d.set_projection(pos, size);
    Text text(font);
    text.pos = glm::vec2(50.f);
    text.text = "made by   m a t i T e c h n o\n"
                "press g to open control panel";
    Sprite sprite(text);
    sprite.color = glm::vec4(0.f, 0.f, 0.f, 0.5f);
    renderer2d.render(sprite);
    renderer2d.render(text);
    renderer2d.flush();
    Blend::set(GL_SRC_ALPHA, GL_ONE);
}
