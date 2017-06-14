#include <NGAME/3d/renderer3d.hpp>
#include <NGAME/glad.h>
#include <NGAME/3d/inst3d.hpp>
#include <NGAME/3d/model.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <NGAME/3d/camera.hpp>

Renderer3d::Renderer3d():
    shader(
        #include "3d.sh"
        , "3d.sh"
        )
{}

void Renderer3d::set_camera(const Camera& camera) const
{
    shader.bind();
    glUniformMatrix4fv(shader.get_uni_location("proj"), 1, GL_FALSE, &camera.proj[0][0]);
    glUniformMatrix4fv(shader.get_uni_location("view"), 1, GL_FALSE, &camera.view[0][0]);
}

void Renderer3d::render(const Inst3d& instance) const
{
    assert(instance.model);

    glEnable(GL_DEPTH_TEST);

    shader.bind();
    glUniform3fv(shader.get_uni_location("u_color"), 1, &instance.color[0]);

    glm::mat4 model(1.f);
    model = glm::translate(model, instance.pos);
    // check if default directions are correct
    model = glm::rotate(model, instance.rotation.x, glm::vec3(1.f, 0.f, 0.f));
    model = glm::rotate(model, instance.rotation.y, glm::vec3(0.f, 1.f, 0.f));
    model = glm::rotate(model, instance.rotation.z, glm::vec3(0.f, 0.f, 1.f));
    model = glm::scale(model, instance.scale);

    glUniformMatrix4fv(shader.get_uni_location("model"), 1, GL_FALSE, &model[0][0]);

    // upload uniforms
    for(auto& mesh: instance.model->meshes)
    {
        mesh.vao.bind();
        glDrawArrays(GL_TRIANGLES, 0, mesh.num_vertices);
    }
    glDisable(GL_DEPTH_TEST);
}
