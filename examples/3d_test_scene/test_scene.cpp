#include "test_scene.hpp"
#include <NGAME/3d/camera.hpp>
#include <NGAME/renderer2d.hpp>
#include <NGAME/text.hpp>
#include <NGAME/font_loader.hpp>
//#include <NGAME/imgui.h>
#include <NGAME/app.hpp>
#include <glm/trigonometric.hpp>

Test_scene::Test_scene():
    monkey_mod("monkey.obj"),
    cube_mod("cube.obj"),
    font(font_loader.load_font("Xolonium-Bold.ttf", 22))
{
    monkey.model = &monkey_mod;
    monkey.mat = Material(Material::test);
    cube.model = &cube_mod;
    cube.mat = Material(Material::gold);
    cube.pos = glm::vec3(-2.f, -2.5f, -3.f);
    lights.reserve(20);
    lights.push_back({{-1.6f, 0.6f, 0.2f}, {1.f, 1.f, 1.f}});
    lights.push_back({{4.f, -5.f, -5.f}, {0.f, 1.f, 0.f}});
}

void Test_scene::render()
{
    time += io.frametime;

    Camera camera(size.x, size.y);
    renderer3d.set_camera(camera);

    for(auto& light: lights)
        renderer3d.set_light(light);

    monkey.rotation.y += io.frametime / 4.f;

    renderer3d.render(monkey);
    renderer3d.render(cube);
    renderer3d.render_lights();

    renderer2d.set_projection(glm::vec2(0.f), size);
    Text text(font);
    text.text = "working on 3d api";
    text.pos = glm::vec2(10.f, 10.f + font.get_ascent());
    renderer2d.render(text);
    renderer2d.flush();

    //    ImGui::Begin("control");
    //    ImGui::Text("frametime(ms): %.3f", io.av_frametime * 1000.f);
    //    if(ImGui::Button("vsync on/off"))
    //    {
    //        vsync = !vsync;
    //        SDL_GL_SetSwapInterval(vsync);
    //    }
    //    ImGui::End();
}
