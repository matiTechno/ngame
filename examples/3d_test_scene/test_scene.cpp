#include "test_scene.hpp"
#include <NGAME/3d/camera.hpp>
#include <NGAME/renderer2d.hpp>
#include <NGAME/text.hpp>
#include <NGAME/font_loader.hpp>
#include <NGAME/imgui.h>
#include <NGAME/app.hpp>
#include <glm/trigonometric.hpp>

Test_scene::Test_scene():
    monkey_mod("monkey.obj"),
    cube_mod("cube.obj"),
    font(font_loader.load_font("Xolonium-Bold.ttf", 30))
{
    std::random_device rd;
    rn_eng.seed(rd());

    emitter = std::make_unique<Emitter>(glm::vec3(0.8f, -0.5f, 1.f), rn_eng);

    monkey.model = &monkey_mod;
    monkey.mat = Material(Material::test);
    cube.model = &cube_mod;
    cube.mat = Material(Material::gold);
    cube.pos = glm::vec3(-2.f, -2.5f, -3.f);
    lights.reserve(20);
    lights.push_back({{-1.6f, 0.6f, 0.2f}});
    lights.push_back({{4.f, -5.f, -5.f}, {0.f, 1.f, 0.f}});
}

void Test_scene::render()
{
    for(auto& event: io.events)
        if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_g)
            imgui = !imgui;

    SDL_ShowCursor(imgui);

    time += io.frametime;

    emitter->update(io.frametime);

    Camera camera(size.x, size.y);
    renderer3d.set_camera(camera);

    for(auto& light: lights)
        renderer3d.set_light(light);
    emitter->set_lights(renderer3d);

    monkey.rotation.y += io.frametime / 8.f;

    renderer3d.render(monkey);
    renderer3d.render(cube);
    renderer3d.render_lights();

    renderer2d.set_projection(glm::vec2(0.f), size);
    Text text(font);
    text.text = "working on 3d api";
    text.pos = glm::vec2(10.f, 10.f + font.get_ascent());
    renderer2d.render(text);
    renderer2d.flush();

    if(!imgui)
        return;

    ImGui::Begin("control");
    ImGui::Text("frametime(ms): %.3f", io.av_frametime * 1000.f);
    if(ImGui::Button("vsync on/off"))
    {
        vsync = !vsync;
        SDL_GL_SetSwapInterval(vsync);
    }
    ImGui::End();
}

// E M I T T E R

Emitter::Emitter(const glm::vec3& position, std::mt19937& rn_eng):
    position(position),
    rn_eng(rn_eng)
{
    std::uniform_real_distribution<float> life(life_min, life_max);
    for(auto& particle: particles)
    {
        particle.vel = glm::vec3(0.f);
        particle.pos = glm::vec3(0.f);
        particle.life = life(rn_eng);
        particle.scale = 0.003f;
        particle.color = glm::vec3(0.f);
        particle.attenuation = 15.f;
    }
}

void Emitter::set_lights(const Renderer3d& renderer) const
{
    for(auto& particle: particles)
        renderer.set_light(particle);
}

void Emitter::update(float dt)
{
    for(auto& particle: particles)
    {
        particle.pos += particle.vel * dt;
        particle.life -= dt;
        if(particle.life <= 0.f)
            spawn(particle);
    }
}

void Emitter::spawn(Particle& particle)
{
    std::uniform_real_distribution<float> life(life_min, life_max);
    particle.life = life(rn_eng);

    std::uniform_real_distribution<float> spawn_radius(-0.1f, 0.1f);
    particle.pos = position + glm::vec3(spawn_radius(rn_eng), spawn_radius(rn_eng), spawn_radius(rn_eng));

    std::uniform_real_distribution<float> r(1.f, 1.f);
    std::uniform_real_distribution<float> g(0.2f, 0.7f);
    std::uniform_real_distribution<float> b(0.f, 0.1f);
    particle.color = glm::vec3(r(rn_eng), g(rn_eng), b(rn_eng));

    std::uniform_real_distribution<float> vel_y(0.5f, 1.5f);
    particle.vel = glm::vec3(0.f, vel_y(rn_eng), 0.f);
}
