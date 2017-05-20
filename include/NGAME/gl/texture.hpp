#pragma once
#include "base.hpp"
#include <glm/vec2.hpp>
#include <utility>
#include <string>

class Texture: public GL_base
{
public:
    friend class PP_unit;

    Texture(const std::string& filename, bool sRGB = false, GLsizei levels = 1);
    // binds texture to GL_TEXTURE_2D
    Texture(GLenum internal_format, GLsizei width, GLsizei height, GLsizei levels = 1);

    void bind(GLenum target = GL_TEXTURE_2D, GLuint unit = 0) const;
    glm::ivec2 get_size() const;
    void set_size(const glm::ivec2& size);

private:
    glm::ivec2 size;
    static std::pair<GLuint, GLuint> bound_id;

    // only generates name (no bind call)
    Texture();
 };

class Sampler: public GL_base
{
public:
    Sampler();

    void bind(GLuint unit = 0) const;
    void set_parameter_i(GLenum pname, GLint param) const;

private:
    static std::pair<GLuint, GLuint> bound_id;
};
