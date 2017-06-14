#pragma once

#include <glm/mat4x4.hpp>

class Camera
{
public:
    // fixed for testing
    // scene size
    Camera(float w, float h);

private:
    friend class Renderer3d;
    glm::mat4 proj;
    glm::mat4 view;
};
