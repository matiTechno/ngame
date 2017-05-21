#pragma once
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <string>
class Font;

class Text
{
public:
    Text(const Font& font);

    // pos + size
    glm::vec4 get_bbox() const;
    void set_pixel_size(int size);

    glm::vec2 pos;
    float scale = 1.f;
    // radians
    float rotation = 0.f;
    // relative to pos
    glm::vec2 rotation_point;
    glm::vec4 color = glm::vec4(1.f, 1.f, 1.f, 1.f);
    std::string text;
    // only applied when scale == 1.f && rotation == 0.f
    bool snap_to_grid = true;
    const Font* font;
};
