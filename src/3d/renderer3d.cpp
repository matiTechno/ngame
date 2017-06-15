#include <NGAME/3d/renderer3d.hpp>
#include <NGAME/glad.h>
#include <NGAME/3d/inst3d.hpp>
#include <NGAME/3d/model.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <NGAME/3d/camera.hpp>
#include "cube.hpp"

Renderer3d::Renderer3d():
    shader(
        #include "3d.sh"
        , "3d.sh"),
    shader_light(
        #include "light.sh"
        , "light.sh")
{
    vbo_light.bind(GL_ARRAY_BUFFER);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
    vao_light.bind();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
}

void Renderer3d::set_camera(const Camera& camera) const
{
    shader.bind();
    glUniformMatrix4fv(shader.get_uni_location("proj"), 1, GL_FALSE, &camera.proj[0][0]);
    glUniformMatrix4fv(shader.get_uni_location("view"), 1, GL_FALSE, &camera.view[0][0]);
    glUniform3fv(shader.get_uni_location("cam_pos"), 1, &camera.pos[0]);
    shader_light.bind();
    glUniformMatrix4fv(shader_light.get_uni_location("proj"), 1, GL_FALSE, &camera.proj[0][0]);
    glUniformMatrix4fv(shader_light.get_uni_location("view"), 1, GL_FALSE, &camera.view[0][0]);
}

void Renderer3d::render(const Inst3d& instance) const
{
    assert(instance.model);

    glEnable(GL_DEPTH_TEST);

    shader.bind();
    glUniform3fv(shader.get_uni_location("mat.ambient"), 1, &instance.mat.ambient[0]);
    glUniform3fv(shader.get_uni_location("mat.diffuse"), 1, &instance.mat.diffuse[0]);
    glUniform3fv(shader.get_uni_location("mat.specular"), 1, &instance.mat.specular[0]);
    glUniform1f(shader.get_uni_location("mat.shininess"), instance.mat.shininess);

    glm::mat4 model(1.f);
    model = glm::translate(model, instance.pos);
    // check if default directions are correct
    model = glm::rotate(model, instance.rotation.x, glm::vec3(1.f, 0.f, 0.f));
    model = glm::rotate(model, instance.rotation.y, glm::vec3(0.f, 1.f, 0.f));
    model = glm::rotate(model, instance.rotation.z, glm::vec3(0.f, 0.f, 1.f));
    model = glm::scale(model, instance.scale);

    glUniformMatrix4fv(shader.get_uni_location("model"), 1, GL_FALSE, &model[0][0]);

    for(auto& mesh: instance.model->meshes)
    {
        mesh.vao.bind();
        glDrawArrays(GL_TRIANGLES, 0, mesh.num_vertices);
    }
    glDisable(GL_DEPTH_TEST);
}

void Renderer3d::set_light(const Light& light) const
{
    ++active_l;
    assert(active_l < lights.size() - 1);
    lights[active_l] = light;

    shader.bind();
    auto name = "lights[" + std::to_string(active_l) + ']';
    glUniform1i(shader.get_uni_location("num_lights"), active_l + 1);
    glUniform3fv(shader.get_uni_location(name + ".pos"), 1, &light.pos.x);
    glUniform3fv(shader.get_uni_location(name + ".color"), 1, &light.color.x);
    glUniform1f(shader.get_uni_location(name + ".attenuation"), light.attenuation);
}

void Renderer3d::render_lights() const
{
    glEnable(GL_DEPTH_TEST);
    shader_light.bind();
    vao_light.bind();
    for(int i = 0; i < active_l + 1; ++i)
    {
        glUniform1f(shader_light.get_uni_location("scale"), lights[i].scale);
        glUniform3fv(shader_light.get_uni_location("translate"), 1, &lights[i].pos[0]);
        glUniform3fv(shader_light.get_uni_location("u_color"), 1, &lights[i].color[0]);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(cube) / 3 * sizeof(float));
    }
    glDisable(GL_DEPTH_TEST);

    active_l = -1;
}
