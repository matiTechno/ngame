#pragma once

#include <glm/vec3.hpp>
class Model;

struct Inst3d
{
    // must not be null when passed to
    // renderer3d
    const Model* model = nullptr;

    glm::vec3 pos = glm::vec3(0.f);
    glm::vec3 scale = glm::vec3(1.f);
    glm::vec3 rotation = glm::vec3(0.f);
    glm::vec3 color = glm::vec3(1.f);
};
