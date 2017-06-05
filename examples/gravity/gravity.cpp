#include "gravity.hpp"
#include <random>
#include <NGAME/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <NGAME/app.hpp>
#include <NGAME/imgui.h>
#include <glm/trigonometric.hpp>
#include <NGAME/blend.hpp>

constexpr int Gravity::num_particles;

Gravity::Gravity():
    Vspace(0.f, 0.f, 100.f, 100.f),
    sh_points("points.sh", false),
    sh_gravity("gravity.sh", false)
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

void Gravity::update()
{
    sh_gravity.bind();
    glUniform1f(sh_gravity.get_uni_location("dt"), io.frametime);
    // todo: upload proper values
    // Vspace modify functions implementation
    // change g_pos vector to ivec2 (in shader too)
    glm::vec2 pos(50.f);
    static float time = 0.f;
    time += io.frametime;
    pos.x += 20.f * glm::sin(time);
    pos.y += 20.f * glm::cos(time);
    glUniform2f(sh_gravity.get_uni_location("g_pos"), pos.x, pos.y);
    glUniform1i(sh_gravity.get_uni_location("g_active"), 1);

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

    ImGui::Begin("fps");
    ImGui::Text("frametime(ms): %.3f", io.av_frametime * 1000.f);
    if(ImGui::Button("vsync on/off"))
    {
        vsync = !vsync;
        SDL_GL_SetSwapInterval(vsync);
    }
    ImGui::End();
}
