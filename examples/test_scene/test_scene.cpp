#include "test_scene.hpp"
#include <NGAME/common.hpp>

Test_scene::Test_scene():
    sample("res/laser1.mp3"),
    shader("res/pp_unit.sh", true),
    texture("res/example.png"),
    font(font_loader.load_font("res/Roboto-Medium.ttf", 100))
{}

void Test_scene::process_input()
{
    for(auto& event: io.events)
    {
        if(event.type == SDL_QUIT)
            quit();

        if(io.imgui_wants_input)
            return;

        if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
            quit();

        else if(event.type == SDL_KEYDOWN && !event.key.repeat && event.key.keysym.sym == SDLK_p)
            sample.play();
    }
}

void Test_scene::set_coords()
{
    size.x = io.w;
    size.y = io.h;
}

void Test_scene::render()
{
    ImGui::ShowTestWindow();
    ImGui::Begin("control");
    ImGui::Text("fb_size: %d x %d", io.w, io.h);
    ImGui::Spacing();
    ImGui::Text("frametime(ms): %.3f", io.av_frametime * 1000.f);
    ImGui::Separator();
    ImGui::Spacing();
    if(ImGui::Button("vsync on / off"))
    {
        vsync = !vsync;
        SDL_GL_SetSwapInterval(vsync);
    }
    ImGui::End();

    renderer2d.set_projection(glm::vec2(0.f), size);
    {
        pp_unit.start();
        Sprite sprite;
        sprite.pos = glm::vec2(0.f);
        sprite.size = size;
        renderer2d.render(sprite);
        renderer2d.flush();

        time += io.frametime;
        shader.bind();
        glUniform1f(shader.get_uni_location("time"), time);
        pp_unit.render(true, shader);
    }
    {
        Sprite sprite;
        sprite.pos = glm::vec2(20.f);
        sprite.size = glm::vec2(200.f);
        sprite.texture = &texture;
        sprite.tex_coords = glm::ivec4(0, 0, texture.get_size());
        renderer2d.render(sprite);
    }
    renderer2d.flush();
}
