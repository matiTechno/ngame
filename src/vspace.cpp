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
    vrange(w, h)
{}

glm::vec2 Vspace::get_vstart() const
{return vstart;}

glm::vec2 Vspace::get_vrange() const
{return vrange;}

glm::vec2 Vspace::get_pstart() const
{return pstart;}

glm::vec2 Vspace::get_prange() const
{return prange;}

void Vspace::set_vs(const glm::vec2& start, const glm::vec2& range)
{
    vstart = start;
    vrange = range;
    set_projection();
}

void Vspace::render2()
{}

void Vspace::render()
{
    renderer2d.set_projection(pstart, prange);
    render2();
}

void Vspace::start2()
{}

void Vspace::start()
{
    size.x = io.w;
    size.y = io.h;
    set_projection();
    start2();
}

void Vspace::set_projection()
{
    prange = vrange;
    auto vaspect = vrange.x / vrange.y;
    if(vaspect < io.aspect)
        prange.x = io.aspect * prange.y;

    else if(vaspect > io.aspect)
        prange.y = prange.x / io.aspect;

    pstart = vstart - (prange - vrange) / 2.f;
}

void Vspace::zoom_to_center(float times)
{
    auto center = vstart + vrange / 2.f;
    vrange *= 1.f / times;
    set_projection();
    auto new_center = vstart + vrange / 2.f;
    vstart += center - new_center;
    set_projection();
}

void Vspace::zoom_to_cursor(float times, const glm::ivec2& cursor_pos)
{
    auto vs_c_pos = get_cursor_vs(cursor_pos);
    vrange *= 1.f / times;
    set_projection();
    auto new_vs_c_pos = get_cursor_vs(cursor_pos);
    vstart += vs_c_pos - new_vs_c_pos;
    set_projection();
}

void Vspace::move(const glm::vec2& vec)
{
    vstart += vec;
    set_projection();
}

void Vspace::move(const glm::vec2& cursor_pos, const glm::vec2& prev_cursor_pos)
{
    vstart += get_cursor_vs(prev_cursor_pos) - get_cursor_vs(cursor_pos);
    set_projection();
}

glm::vec2 Vspace::get_cursor_vs(const glm::ivec2& cursor_pos) const
{
    return pstart + glm::vec2(cursor_pos) * (prange / glm::vec2(io.w, io.h));
}

void Vspace::render_background(const Texture& texture)
{
    auto prev_pstart = renderer2d.get_pstart();
    auto prev_prange = renderer2d.get_prange();
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
    renderer2d.set_projection(prev_pstart, prev_prange);
}

void Vspace::render_grid(int num_x, int num_y, float border_width, const glm::vec4& color)
{
    auto prev_pstart = renderer2d.get_pstart();
    auto prev_prange = renderer2d.get_prange();
    renderer2d.set_projection(glm::vec2(0.f), glm::vec2(io.w, io.h));
    {
        glm::vec2 ga_start(0.f), ga_size(io.w, io.h);
        auto vaspect = vrange.x / vrange.y;
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

        auto grid_size = ga_size / glm::vec2(num_x, num_y);

        auto grid_pos = ga_start;

        glm::ivec4 dum;
        glm::vec2 dum2;

        for(int i = 0; i < num_y; ++i)
        {
            for(int j = 0; j < num_x; ++j)
            {
                renderer2d.render(grid_pos, glm::vec2(grid_size), dum, nullptr, 0.f, dum2, color);
                grid_pos.x += grid_size.x;
            }
            grid_pos.x = ga_start.x;
            grid_pos.y += grid_size.y;
        }
        renderer2d.flush();

        glBlendFuncSeparate(GL_ONE, GL_ZERO, GL_ONE, GL_ZERO);
        grid_pos = ga_start;
        for(int i = 0; i < num_y; ++i)
        {
            for(int j = 0; j < num_x; ++j)
            {
                renderer2d.render(grid_pos + border_width, glm::vec2(grid_size - border_width * 2.f), dum, nullptr, 0.f,
                                                                     dum2, glm::vec4(0.f));
                grid_pos.x += grid_size.x;
            }
            grid_pos.x = ga_start.x;
            grid_pos.y += grid_size.y;
        }
        renderer2d.flush();
        Blend::set_default();
    }
    renderer2d.set_projection(prev_pstart, prev_prange);
}
