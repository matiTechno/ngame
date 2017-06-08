#include "mandelbrot.hpp"
#include <NGAME/glad.h>
#include <NGAME/app.hpp>
#include <NGAME/pp_unit.hpp>
#include <NGAME/imgui.h>
#include <glm/exponential.hpp>

constexpr float mid_x = -1.283918f, mid_y = 0.427249f,
min_range_x = 0.0001f,
range_x = 3.5f, range_y = 2.f,
start_x = mid_x - range_x / 2.f, start_y = mid_y - range_y / 2.f;

Mandelbrot::Mandelbrot():
    Vspace(start_x, start_y, range_x, range_y),
    shader("compute.sh", false)
{}

void Mandelbrot::process_input()
{
    float scroll = 0.f;
    int add_to_it = 0;
    for(auto& event: io.events)
    {
        if(event.type == SDL_KEYDOWN)
        {
            auto key = event.key.keysym.sym;
            if(key == SDLK_g)
                imgui = !imgui;
            else if(key == SDLK_KP_PLUS)
                ++add_to_it;
            else if(key == SDLK_KP_MINUS)
                --add_to_it;
        }
        else if(event.type == SDL_MOUSEWHEEL)
            scroll += event.wheel.y;
        else if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_RIGHT)
            is_rmb = true;
        else if(event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_RIGHT)
            is_rmb = false;
    }
    auto hide = pilot && !imgui;
    SDL_ShowCursor(!hide);
    if(hide || io.imgui_wants_input)
    {
        is_rmb = false;
        return;
    }
    float times = glm::pow(zoom_coeff, scroll);
    if(is_rmb)
        zoom_to_cursor(times, io.cursor_pos);
    else
        zoom_to_center(times);

    if(is_rmb)
        move(io.cursor_pos, prev_cursor_pos);

    iterations += add_to_it;
}

void Mandelbrot::render()
{
    if(pilot)
    {
        auto vrange = get_vrange();
        static float accumulator = 0.f;
        accumulator += io.frametime;
        while(accumulator >= 0.01666f)
        {
            zoom_to_center(sign * 1.005f);
            accumulator -= 0.01666f;
        }
        if(vrange.x < min_range_x)
            sign = -1;
        else if(vrange.x > range_x)
                sign = 1;
    }

    shader.bind();
    auto pstart = get_pstart();
    glUniform2f(shader.get_uni_location("start"), pstart.x, pstart.y);
    auto prange = get_prange();
    glUniform2f(shader.get_uni_location("range"), prange.x, prange.y);
    glUniform2i(shader.get_uni_location("fb_size"), io.w, io.h);
    glUniform1i(shader.get_uni_location("iterations"), iterations);

    pp_unit.start();
    pp_unit.render(true, shader);

    if(imgui)
    {
        ImGui::Begin("control panel");
        ImGui::Text("frametime(ms): %.3f", io.frametime * 1000.f);
        if(ImGui::Button("vsync on/off"))
        {
            vsync = !vsync;
            SDL_GL_SetSwapInterval(vsync);
        }
        if(ImGui::Button("pilot on/off"))
            pilot = !pilot;
        ImGui::Text("iterations: %d", iterations);
        ImGui::Text("rmb - move around\n"
                    "scroll - zoom in/out\n"
                    "+/-(num) - change iterations");
        ImGui::InputInt("iterations", &iterations);
        auto vstart = get_vstart();
        auto vrange = get_vrange();
        ImGui::Text("mid-point: %f, %f", vstart.x + vrange.x / 2.f, vstart.y + vrange.y / 2.f);
        ImGui::Text("range: %f, %f", vrange.x, vrange.y);
        ImGui::End();
    }
    prev_cursor_pos = io.cursor_pos;
}
