#include <NGAME/font.hpp>

Font::Font(std::map<char, Glyph>&& glyphs, Texture&& atlas,
           int size_px, int new_line_space, int max_bearing_y, int max_below_baseline):
    glyphs(std::move(glyphs)),
    atlas(std::move(atlas)),
    size_px(size_px),
    new_line_space(new_line_space),
    max_bearing_y(max_bearing_y),
    max_below_baseline(max_below_baseline)
{}

const Glyph& Font::get_glyph(char code) const
{
    auto it = glyphs.find(code);
    if(it != glyphs.end())
        return it->second;
    else
        return glyphs.at('?');
}

const Texture& Font::get_atlas() const
{
    return atlas;
}

int Font::get_size_px() const
{
    return size_px;
}

int Font::get_new_line_space() const
{
    return new_line_space;
}

int Font::get_max_bearing_y() const
{
    return max_bearing_y;
}

int Font::get_max_below_baseline() const
{
    return max_below_baseline;
}
