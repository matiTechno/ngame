#include "test_scene.hpp"
#include <NGAME/3d/renderer3d.hpp>
#include <NGAME/3d/camera.hpp>
#include <NGAME/renderer2d.hpp>
#include <NGAME/text.hpp>
#include <NGAME/font_loader.hpp>
#include <NGAME/imgui.h>
#include <NGAME/app.hpp>

Test_scene::Test_scene():
    model("monkey.obj"),
    font(font_loader.load_font("ProggyCleanCE.ttf", 22))
{
    inst1.model = &model;
    inst1.color = glm::vec3(1.f, 0.2f, 0.f);
    renderer3d.set_light({{-1.f, 1.f, 1.f}, {1.f, 0.5f, 1.f}});
}

void Test_scene::render()
{
    Camera camera(size.x, size.y);
    renderer3d.set_camera(camera);

    renderer3d.render(inst1);
    renderer3d.render_lights();

    renderer2d.set_projection(glm::vec2(0.f), size);
    Text text(font);
    text.text = "working on 3d api";
    text.pos = glm::vec2(10.f, 10.f + font.get_ascent());
    renderer2d.render(text);
    renderer2d.flush();

    ImGui::Begin("control");
    ImGui::Text("frametime(ms): %.3f", io.frametime * 1000.f);
    if(ImGui::Button("vsync on/off"))
    {
        vsync = !vsync;
        SDL_GL_SetSwapInterval(vsync);
    }
    ImGui::End();
}
