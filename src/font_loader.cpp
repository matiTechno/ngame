#include <NGAME/font_loader.hpp>
#include <NGAME/font.hpp>
#include <NGAME/glad.h>

Font Font_loader::load_font(const std::string& filename, int size_px) const
{
    (void)filename;
    (void)size_px;
    std::map<char, Glyph> glyphs;
    Texture texture(GL_R8, 100, 100, 1);
    return Font(std::move(glyphs), std::move(texture), 0, 0, 0, 0);
}
