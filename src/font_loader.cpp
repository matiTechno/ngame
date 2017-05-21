#include <NGAME/font_loader.hpp>
#include <NGAME/font.hpp>
#include <NGAME/glad.h>
#include <fstream>
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

Font Font_loader::load_font(const std::string& filename, int size_px) const
{
    auto data = load_ttf(filename);
    stbtt_fontinfo fontinfo;

    if(stbtt_InitFont(&fontinfo, data.data(), 0) == 0)
        throw std::runtime_error("Font_loader, filename = " + filename + ": stbtt_InitFont failed");

    auto scale = stbtt_ScaleForPixelHeight(&fontinfo, size_px);
    int ascent, descent, linegap;
    stbtt_GetFontVMetrics(&fontinfo, &ascent, &descent, &linegap);

    std::map<int, Glyph> glyphs;
    std::map<int, unsigned char*> bitmaps;
    int tex_width = 1024;
    int tex_height = 0;

    int max_bm_h = 0;
    glm::ivec2 pos(0.f);
    for(auto i = 32; i < 127; ++i)
    {
        auto id = stbtt_FindGlyphIndex(&fontinfo, i);
        if(id == 0)
            continue;

        Glyph glyph;
        int dum;
        stbtt_GetGlyphHMetrics(&fontinfo, id, &glyph.advance, &dum);
        glyph.advance *= scale;
        bitmaps[i] = stbtt_GetGlyphBitmap(&fontinfo, scale, scale, id, &glyph.tex_coords.z, &glyph.tex_coords.w,
                                          &glyph.bearing.x, &glyph.bearing.y);

        glyph.bearing.y *= -1;

        if(pos.x + glyph.tex_coords.z > tex_width)
        {
            pos.x = 0;
            pos.y += max_bm_h + 1;
            max_bm_h = 0;
        }

        glyph.tex_coords.x = pos.x;
        glyph.tex_coords.y = pos.y;

        pos.x += glyph.tex_coords.z + 1;
        if(glyph.tex_coords.w > max_bm_h)
            max_bm_h = glyph.tex_coords.w;

        glyphs.emplace(i, glyph);
    }
    tex_height += pos.y + max_bm_h;

    Texture atlas(GL_R8, tex_width, tex_height, 1);

    std::vector<char> vec_clear(tex_width * tex_height, 0);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, tex_width, tex_height, GL_RED, GL_BYTE, vec_clear.data());

    GLint unpack_alignment;
    glGetIntegerv(GL_UNPACK_ALIGNMENT, &unpack_alignment);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for(auto& glyph: glyphs)
        glTexSubImage2D(GL_TEXTURE_2D, 0, glyph.second.tex_coords.x, glyph.second.tex_coords.y,
                        glyph.second.tex_coords.z, glyph.second.tex_coords.w, GL_RED, GL_UNSIGNED_BYTE,
                        bitmaps.at(glyph.first));

    glPixelStorei(GL_UNPACK_ALIGNMENT, unpack_alignment);

    for(auto& bitmap: bitmaps)
        stbtt_FreeBitmap(bitmap.second, nullptr);

    return Font(std::move(glyphs), std::move(atlas), size_px, (ascent - descent + linegap) * scale,
                ascent * scale, descent * scale);
}

std::vector<unsigned char> Font_loader::load_ttf(const std::string& filename) const
{
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if(!file)
        throw std::runtime_error("Font_loader: could not open " + filename);

    auto size = file.tellg();
    std::vector<unsigned char> data(size);

    file.seekg(0);
    file.read(reinterpret_cast<char*>(data.data()), size);

    return data;
}
