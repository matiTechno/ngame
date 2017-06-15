#include <NGAME/3d/camera.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(float w, float h)
{
    proj = glm::perspective(glm::pi<float>() / 2.f, w / h, 0.1f, 100.f);

    const glm::vec3 forward(0.f, 0.f, -1.f);
    pos = glm::vec3(0.f, 0.f, 2.f);
    view = glm::lookAt(pos, pos + forward, glm::vec3(0.f, 1.f, 0.f));
}
