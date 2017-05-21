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
    Font(std::map<int, Glyph>&& glyphs, Texture&& atlas,
         int size_px, int linespace, int ascent, int descent);

    const Glyph& get_glyph(int code) const;

    const Texture& get_atlas() const;

    int get_size_px() const;
    int get_linespace() const;
    int get_ascent() const;
    // typically negative
    int get_descent() const;

private:
    std::map<int, Glyph> glyphs;

    Texture atlas;
    int size_px;
    int linespace;
    // for calculating text bbox
    int ascent;
    int descent;
};
