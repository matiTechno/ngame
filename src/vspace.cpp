#include <NGAME/vspace.hpp>
#include <NGAME/gl/texture.hpp>
#include <NGAME/sprite.hpp>
#include <NGAME/renderer2d.hpp>

Vspace::Vspace(float x, float y, float w, float h):
    v_vstart(x, y),
    v_vsize(w, h)
{}

glm::vec2 Vspace::vstart() const
{return v_vstart;}

glm::vec2 Vspace::vsize() const
{return v_vsize;}

glm::vec2 Vspace::pstart() const
{return v_pstart;}

glm::vec2 Vspace::psize() const
{return v_psize;}

void Vspace::start2()
{}

void Vspace::start()
{

}

void Vspace::zoom_to_center(float times)
{
    (void)times;
}

void Vspace::zoom_to_point(float times, const glm::vec2& point)
{
    (void)times;
    (void)point;
}

void Vspace::move(const glm::vec2& vec)
{
    (void)vec;
}

glm::vec2 Vspace::get_mouse_vs() const
{
    return glm::vec2();
}

void Vspace::render_background(const Texture& texture) const
{
    Sprite sprite;
    sprite.texture = &texture;

    renderer2d.render(sprite);
}

void Vspace::render_grid(float grid_size, float border_width, const glm::vec4& color) const
{
    (void)grid_size;
    (void)border_width;
    (void)color;
}
