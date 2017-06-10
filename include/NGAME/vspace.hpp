#pragma once

#include "scene.hpp"
#include <glm/vec2.hpp>
class Texture;
#include <NGAME/gl/shader.hpp>

// vspace = virtual space (minimal visible rect)
// evolved from arkanoid example
// only works as scene with size == fb_size (it's easy to fix but I don't have
// a good reason to do it)

// implement start2() as start function
// if fb_size changes:
// * vstart
// * vrange
// * pstart
// * prange
//
// are updated in start() function before
// any other function is called

// implement render2() as render function
// projection is set in renderer2d just before render2() invocation

class Vspace: public Scene
{
public:
    Vspace(float x, float y, float w, float h);
    Vspace() = default;

    void start() override final;
    void render() override final;

protected:
    // virtual space
    glm::vec2 get_vstart() const;
    glm::vec2 get_vrange() const;
    // projection = whole visible area
    glm::vec2 get_pstart() const;
    glm::vec2 get_prange() const;

    // projection modifiers
    void zoom_to_center(float times);
    void zoom_to_cursor(float times, const glm::ivec2& cursor_pos);
    void move(const glm::vec2& cursor_pos, const glm::vec2& prev_cursor_pos);
    void move(const glm::vec2& vec);
    void set_vs(const glm::vec2& start, const glm::vec2& range);

    // convert cursor position from window to projection space
    glm::vec2 get_cursor_vs(const glm::ivec2& cursor_pos) const;

    // you certainly want to call renderer2d.flush() before
    // these functions (they change projection in renderer2d but restore it on return)

    // renders centered texture
    // if texture.size < fb_size (framebuffer)
    // texture is scaled
    // else
    // only part of texture is used with size = fb_size
    void render_background(const Texture& texture);

    // renders grid in vspace area
    // border is contained inside grid rect
    // to make grid transparent render it with pp_unit
    // (see game of life example)
    //                                           in pixels
    void render_grid(int num_x, int num_y, float border_width, const glm::vec4& color);

private:
    glm::vec2 vstart, vrange;
    glm::vec2 pstart, prange;

    // invoked at the end of render()
    virtual void render2();
    // invoked at the end of start()
    virtual void start2();

    void set_projection();
};
