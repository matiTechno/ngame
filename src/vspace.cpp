#include <NGAME/vspace.hpp>
#include <NGAME/gl/texture.hpp>
#include <NGAME/sprite.hpp>
#include <NGAME/renderer2d.hpp>
#include <NGAME/app.hpp>
#include <NGAME/blend.hpp>

#define NGAME_BLEND
#include <NGAME/glad.h>

Vspace::Vspace(float x, float y, float w, float h):
    vstart(x, y),
    vsize(w, h)
{}

glm::vec2 Vspace::get_vstart() const
{return vstart;}

glm::vec2 Vspace::get_vsize() const
{return vsize;}

glm::vec2 Vspace::get_pstart() const
{return pstart;}

glm::vec2 Vspace::get_psize() const
{return psize;}

void Vspace::start2()
{}

void Vspace::start()
{
    size.x = io.w;
    size.y = io.h;

    psize = vsize;
    auto vaspect = vsize.x / vsize.y;
    if(vaspect < io.aspect)
        psize.x = io.aspect * psize.y;

    else if(vaspect > io.aspect)
        psize.y = psize.x / io.aspect;

    pstart = vstart - (psize - vsize) / 2.f;

    renderer2d.set_projection(pstart, psize);

    start2();
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
    auto prev_pstart = renderer2d.get_pstart();
    auto prev_psize = renderer2d.get_prange();
    renderer2d.set_projection(glm::vec2(0.f), glm::vec2(io.w, io.h));
    {
        Sprite sprite;
        sprite.pos = glm::vec2(0.f);
        sprite.size = size;
        sprite.texture = &texture;

        auto tex_size = texture.get_size();
        if(tex_size.x >= size.x && tex_size.y >= size.y)
        {
            glm::ivec2 c_pos = tex_size / 2 - size / 2;
            sprite.tex_coords = glm::ivec4(c_pos, size);
        }
        else
        {
            auto tex_aspect = float(tex_size.x) / tex_size.y;
            glm::ivec4 tex_coords(0, 0, tex_size);

            if(io.aspect > tex_aspect)
            {
                tex_coords.w = tex_size.x / io.aspect;
                tex_coords.y = (tex_size.y - tex_coords.w) / 2;
            }
            else if(io.aspect < tex_aspect)
            {
                tex_coords.z = tex_size.y * io.aspect;
                tex_coords.x = (tex_size.x - tex_coords.z) / 2;
            }

            sprite.tex_coords = tex_coords;
        }
        renderer2d.render(sprite);
        renderer2d.flush();
    }
    renderer2d.set_projection(prev_pstart, prev_psize);
}

void Vspace::render_grid(int num_x, float border_width, const glm::vec4& color) const
{
    auto prev_pstart = renderer2d.get_pstart();
    auto prev_psize = renderer2d.get_prange();
    renderer2d.set_projection(glm::vec2(0.f), glm::vec2(io.w, io.h));
    {
        glm::vec2 ga_start(0.f), ga_size(io.w, io.h);
        auto vaspect = vsize.x / vsize.y;
        if(io.aspect > vaspect)
        {
            ga_size.x = vaspect * ga_size.y;
            ga_start.x = (io.w - ga_size.x) / 2.f;
        }
        else if(io.aspect < vaspect)
        {
            ga_size.y = ga_size.x / vaspect;
            ga_start.y = (io.h - ga_size.y) / 2.f;
        }

        auto grid_size = ga_size.x / num_x;
        auto num_y = ga_size.y / grid_size;

        auto grid_pos = ga_start;

        for(int i = 0; i < num_y; ++i)
        {
            for(int j = 0; j < num_x; ++j)
            {
                Sprite sprite;
                sprite.pos = grid_pos;
                sprite.size = glm::vec2(grid_size);
                sprite.color = color;
                renderer2d.render(sprite);
                grid_pos.x += grid_size;
            }
            grid_pos.x = ga_start.x;
            grid_pos.y += grid_size;
        }
        renderer2d.flush();

        glBlendFuncSeparate(GL_ONE, GL_ZERO, GL_ONE, GL_ZERO);
        grid_pos = ga_start;
        for(int i = 0; i < num_y; ++i)
        {
            for(int j = 0; j < num_x; ++j)
            {
                Sprite sprite;
                sprite.pos = grid_pos + border_width;
                sprite.size = glm::vec2(grid_size - border_width * 2.f);
                sprite.color = glm::vec4(0.f);
                renderer2d.render(sprite);
                grid_pos.x += grid_size;
            }
            grid_pos.x = ga_start.x;
            grid_pos.y += grid_size;
        }
        renderer2d.flush();
        Blend::set_default();
    }
    renderer2d.set_projection(prev_pstart, prev_psize);
}
