#pragma once
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
class Texture;

class Sprite
{
public:
    glm::vec2 pos;
    glm::vec2 size;
    glm::ivec4 tex_coords;
    Texture* texture = nullptr;
    // radians
    float rotation = 0.f;
    // relative to sprite top left corner
    glm::vec2 rotation_point;
    glm::vec4 color = glm::vec4(1.f, 1.f, 1.f, 1.f);
};
