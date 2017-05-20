#pragma once
#include "gl/buffers.hpp"
#include "gl/texture.hpp"
#include "gl/shader.hpp"
class Sprite;
#include <glm/mat4x4.hpp>
#include <vector>
class Text;

enum class Sampl_type
{
    linear,
    nearest
};

class Renderer2d
{
public:
    Renderer2d();

    void set_projection(const glm::vec2& start, const glm::vec2& range) const;

    void set_default_blending() const;

    // for font rendering sampling is always linear
    void flush(Sampl_type type = Sampl_type::linear) const;

    void render(const Sprite& sprite) const;

    void render(const Text& text) const;

private:
    Sampler sampl_linear, sampl_nearest;
    mutable const Sampler* sampl_sprite = &sampl_linear;
    Shader sh_sprite;
    VAO vao;
    BO bo_quad, bo_sprite;

    struct Vbo_instance
    {
        glm::vec4 color;
                  // pos + size
        glm::vec4 tex_coords;
        glm::mat4 model;
    };

    struct Batch
    {
        std::vector<Vbo_instance> vbo_data;
        const Texture* texture;
        bool is_sprite;
    };

    enum Rend_t
    {
        sp_no_tex,
        sp_tex,
        font
    };

    mutable std::vector<Batch> batches;
};
