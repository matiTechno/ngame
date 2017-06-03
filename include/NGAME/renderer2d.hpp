#pragma once
#include "gl/buffers.hpp"
#include "gl/texture.hpp"
#include "gl/shader.hpp"
class Sprite;
#include <glm/mat4x4.hpp>
#include <vector>
class Text;
#include <array>

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

    // for font rendering sampling is always linear
    void flush(Sampl_type type = Sampl_type::linear) const;

    void render(const Sprite& sprite) const;

    void render(const Text& text) const;

    // to support SOA
    void render(const glm::vec2& pos, const glm::vec2& size, const glm::ivec4& tex_coords, const Texture* texture,
                float rotation, const glm::vec2& rotation_point, const glm::vec4& color) const;

    // last used projection
    glm::vec2 get_pstart() const;
    glm::vec2 get_prange() const;

private:
    Sampler sampl_linear, sampl_nearest;
    mutable const Sampler* sampl_sprite = &sampl_linear;
    Shader sh_sprite;
    VAO vao;
    BO bo_quad, bo_sprite;
    mutable glm::vec2 pstart, prange;

    struct Vbo_instance
    {
        glm::vec4 color;
        // pos + size
        glm::vec4 tex_coords;
        glm::mat4 model;
    };

    struct Batch
    {
        Batch(std::size_t start, std::size_t size, const Texture* texture, bool is_sprite):
            start(start),
            size(size),
            texture(texture),
            is_sprite(is_sprite)
        {}

        std::size_t start, size;
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
    mutable std::array<Vbo_instance, 100000> instances;
};
