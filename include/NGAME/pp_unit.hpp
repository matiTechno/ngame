#pragma once
#include <NGAME/gl/texture.hpp>
class Shader;
#include <NGAME/gl/buffers.hpp>
#include <glm/vec4.hpp>
#include <NGAME/gl/shader.hpp>

// typical vertex shader implementation for PP_unit:
//
// #VERTEX
// #version 430
// layout(location = 0) in vec4 vertex;
// out vec2 tex_coord;
// void main()
// {
// gl_Position = vec4(vertex.xy, 0, 1);
// tex_coord = vertex.zw;
// }

class PP_unit
{
public:
    PP_unit(int w, int h);

    void start() const;

    void render(bool end, const Shader& shader) const;

    // does checking
    void set_new_size(int w, int h);

    void set_scene(const glm::ivec4& scene_gl_coords);

private:
    BO bo_quad;
    VAO vao;
    Texture texs[2];
    FBO fbos[2];
    int w = 0, h = 0;
    // for viewport and scissor
    glm::ivec4 scene_gl_coords;
    mutable bool has_finished = true;
    mutable int bound_fbo;
};
