#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <NGAME/gl/buffers.hpp>
#include <string>

class Model
{
public:
    Model(const std::string& filename);

    glm::vec3 pos;
    glm::vec3 scale;
    glm::vec3 rotation;
    glm::vec4 color;

private:
    VAO vao;
    BO vbo;
    BO ebo;
    int num_elements;
};
