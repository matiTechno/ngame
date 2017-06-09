#include "test_scene.hpp"
#include <NGAME/common.hpp>
#include <glm/gtc/constants.hpp>
#include "menu.hpp"

Test_scene::Test_scene():
    sample("res/laser1.mp3"),
    shader("res/pp_unit.sh", true),
    texture("res/example.png"),
    font(font_loader.load_font("res/Roboto-Medium.ttf", 60)),
    emitter(rn_eng),
    sh_wave("res/wave.sh", true)
{
    std::random_device rd;
    rn_eng.seed(rd());

    emitter.spawn_pos = glm::vec2(40.f, 500.f);
    emitter.spawn_size = glm::vec2(60.f, 100.f);
    emitter.spawn_time = 0.001f;
    // ...
    emitter.size_min = 2.f;
    emitter.size_max = 8.f;
    emitter.life_min = 2.f;
    emitter.life_max = 6.f;
    emitter.acc_min = glm::vec2(5.f, 3.f);
    emitter.acc_max = glm::vec2(12.f, 6.f);
    emitter.vel_min = glm::vec2(0.f, -50.f);
    emitter.vel_max = glm::vec2(0.f, -200.f);
    emitter.ang_vel_min = -glm::pi<float>();
    emitter.ang_vel_max = 0.f;
    emitter.color_min = glm::vec4(1.f, 0.1f, 0.f, 0.2f);
    emitter.color_max = glm::vec4(1.f, 0.7f, 0.1f, 0.6f);
    // ...
    emitter.reserve(1.f / emitter.spawn_time * emitter.life_max);
}

void Test_scene::process_input()
{
    if(io.imgui_wants_input)
        return;

    for(auto& event: io.events)
    {
        if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
            set_new_scene<Menu>(font, sh_wave);

        else if(event.type == SDL_KEYDOWN && !event.key.repeat && event.key.keysym.sym == SDLK_p)
            sample.play();
    }
}

void Test_scene::update()
{
    emitter.update(io.frametime);
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
    ImGui::Spacing();
    ImGui::Text("num particles: %d", emitter.last_active + 1);
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
        sprite.color.a = 0.3f;
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

    emitter.render(renderer2d);

    {
        Text text(font);
        text.text = "Hello Sailor!\nWhoa.";
        text.pos = glm::vec2(200.f, 100.f);
        Sprite sprite(text);
        sprite.color.a = 0.2f;
        renderer2d.render(text);
        renderer2d.render(sprite);
    }

    renderer2d.flush();
}

Emitter::Emitter(std::mt19937& rn_eng):
    rn_eng(rn_eng)
{}

void Emitter::update(float dt)
{
    for(auto i = 0; i < last_active + 1; ++i)
    {
        particles[i].life -= dt;
        if(particles[i].life <= 0.f)
            kill(i); // care: swap

        particles[i].pos += particles[i].acc * dt * dt / 0.5f + particles[i].vel * dt;
        particles[i].vel += particles[i].acc * dt;
        particles[i].rotation += particles[i].ang_vel * dt;
    }

    accumulator += dt;
    if(accumulator > 0.020)
        accumulator = 0.020;
    while(accumulator > spawn_time)
    {
        spawn();
        accumulator -= spawn_time;
    }
}

void Emitter::render(const Renderer2d& renderer) const
{
    renderer.flush();
    Blend::set(GL_SRC_ALPHA, GL_ONE);
    for(auto i = 0; i < last_active + 1; ++i)
        renderer.render(particles[i]);
    renderer.flush();
    Blend::set_default();
}

void Emitter::spawn()
{
    ++last_active;

    if(last_active == int(particles.size())) // to silence warning
        particles.emplace_back();

    auto& p = particles[last_active];

    // pos
    std::uniform_real_distribution<float> dist_pos(0.f, 1.f);
    p.pos = spawn_pos + glm::vec2(dist_pos(rn_eng) * spawn_size.x, dist_pos(rn_eng) * spawn_size.y);
    // size
    std::uniform_real_distribution<float> dist_size(size_min, size_max);
    p.size = glm::vec2(dist_size(rn_eng));
    // color
    std::uniform_real_distribution<float> dist_r(color_min.x, color_max.x);
    std::uniform_real_distribution<float> dist_g(color_min.y, color_max.y);
    std::uniform_real_distribution<float> dist_b(color_min.z, color_max.z);
    std::uniform_real_distribution<float> dist_a(color_min.w, color_max.w);
    p.color = glm::vec4(dist_r(rn_eng), dist_g(rn_eng), dist_b(rn_eng), dist_a(rn_eng));
    // rotation_point
    p.rotation_point = p.size / 2.f;
    // life
    std::uniform_real_distribution<float> dist_life(life_min, life_max);
    p.life = dist_life(rn_eng);
    // vel
    std::uniform_real_distribution<float> dist_vel_x(vel_min.x, vel_max.x);
    std::uniform_real_distribution<float> dist_vel_y(vel_min.y, vel_max.y);
    p.vel = glm::vec2(dist_vel_x(rn_eng), dist_vel_y(rn_eng));
    // acc
    std::uniform_real_distribution<float> dist_acc_x(acc_min.x, acc_max.x);
    std::uniform_real_distribution<float> dist_acc_y(acc_min.y, acc_max.y);
    p.acc = glm::vec2(dist_acc_x(rn_eng), dist_acc_y(rn_eng));
    // ang_vel
    std::uniform_real_distribution<float> dist_ang_vel(ang_vel_min, ang_vel_max);
    p.ang_vel = dist_ang_vel(rn_eng);
}

void Emitter::kill(int index)
{
    std::swap(particles[index], particles[last_active]);
    --last_active;
}

void Emitter::reserve(std::size_t num)
{
    particles.reserve(num);
}
