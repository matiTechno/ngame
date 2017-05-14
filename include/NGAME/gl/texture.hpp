#pragma once
#include "base.hpp"
#include "../del.hpp"
#include <memory>
#include <glm/vec2.hpp>
#include <utility>
#include <string>

class Texture: public GL_base, public Del
{
public:
    Texture(const std::string& filename, bool sRGB = false, GLsizei levels = 1);
    // binds texture to GL_TEXTURE_2D
    Texture(GLenum internal_format, GLsizei width, GLsizei height, GLsizei levels = 1);
    // only generates name (no bind call)
    Texture();

    void bind(GLenum target = GL_TEXTURE_2D, GLuint unit = 0) const;
    glm::ivec2 get_size() const;

private:
    glm::ivec2 size;
    static std::pair<GLuint, GLuint> bound_id;
};

class Sampler: public GL_base, public Del
{
public:
    Sampler();

    void bind(GLuint unit = 0) const;
    void set_parameter_i(GLenum pname, GLint param) const;

private:
    static std::pair<GLuint, GLuint> bound_id;
};
