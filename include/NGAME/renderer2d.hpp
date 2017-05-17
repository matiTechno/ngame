#pragma once
#include <NGAME/gl/buffers.hpp>
#include <NGAME/gl/texture.hpp>
#include <NGAME/gl/shader.hpp>
class Sprite;
#include <glm/mat4x4.hpp>
#include <vector>

enum class Sampl_type
{
    linear,
    nearest
};

class Renderer2d
{
public:
    Renderer2d();
                                // projection
    void flush(const glm::vec2& start, const glm::vec2& range, Sampl_type type = Sampl_type::linear) const;

    void render(const Sprite& sprite) const;

private:
    Sampler s_linear, s_nearest;
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
        Texture* texture;
    };

    mutable std::vector<Batch> batches;
};
