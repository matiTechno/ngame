#pragma once
// EXPERIMENTAL !!!

#include "scene.hpp"
#include <glm/vec2.hpp>
class Texture;

// Vspace = virtual space (minimal visible rect)
// evolved from arkanoid example
// renderer2d projection is set to vspace rect at the beginning of every frame
class Vspace: public Scene
{
public:
    Vspace(float x, float y, float w, float h);

    void start() override final;

protected:
    // virtual space
    glm::vec2 get_vstart() const;
    glm::vec2 get_vsize() const;

    // NOT IMPLEMENTED YET
    // modify
    void zoom_to_center(float times);
    void zoom_to_point(float times, const glm::vec2& point);
    void move(const glm::vec2& vec);
    // convert mouse position from window to virtual space
    glm::vec2 get_mouse_vs() const;

    // projection = whole visible space
    glm::vec2 get_pstart() const;
    glm::vec2 get_psize() const;

    // renders centered texture
    // if texture.size < fb_size (framebuffer)
    // texture is scaled
    // else
    // only part of texture is used with size = fb_size
    void render_background(const Texture& texture) const;

    // border is always in grid rect
    // to make grid transparent render it with pp_unit (see game_of_life example)
    void render_grid(int num_x, float border_width, const glm::vec4& color) const;

private:
    glm::vec2 vstart, vsize;
    glm::vec2 pstart, psize;

    // if start-like function is needed override this
    // it will be invoked after start()
    virtual void start2();
};
