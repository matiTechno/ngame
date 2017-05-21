#include <NGAME/font.hpp>

Font::Font(std::map<int, Glyph>&& glyphs, Texture&& atlas,
           int size_px, int linespace, int ascent, int descent):
    glyphs(std::move(glyphs)),
    atlas(std::move(atlas)),
    size_px(size_px),
    linespace(linespace),
    ascent(ascent),
    descent(descent)
{}

const Glyph& Font::get_glyph(int code) const
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

int Font::get_linespace() const
{
    return linespace;
}

int Font::get_ascent() const
{
    return ascent;
}

int Font::get_descent() const
{
    return descent;
}
