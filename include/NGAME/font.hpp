#pragma once
#include "gl/texture.hpp"
#include <map>
#include <glm/vec4.hpp>

struct Glyph
{
    glm::ivec4 tex_coords;
    glm::ivec2 bearing;
    int advance;
};

class Font
{
public:
    Font(std::map<char, Glyph>&& glyphs, Texture&& atlas,
         int size_px, int new_line_space, int max_bearing_y, int max_below_baseline);

    const Glyph& get_glyph(char code) const;

    const Texture& get_atlas() const;

    int get_size_px() const;
    int get_new_line_space() const;
    int get_max_bearing_y() const;
    int get_max_below_baseline() const;

private:
    std::map<char, Glyph> glyphs;

    Texture atlas;
    int size_px;
    int new_line_space;
    // for calculating text bb
    int max_bearing_y;
    int max_below_baseline;
};
