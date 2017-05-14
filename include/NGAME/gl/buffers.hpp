#pragma once
#include "base.hpp"
#include "../del.hpp"

class VAO: public GL_base, public Del
{
public:
    VAO();

    void bind() const;

private:
    static GLuint bound_id;
};

class BO: public GL_base, public Del
{
public:
    BO();

    void bind(GLenum target) const;
};

class FBO: public GL_base, public Del
{
public:
    FBO();

    void bind(GLenum target = GL_FRAMEBUFFER) const;
};

class RBO: public GL_base, public Del
{
    RBO();

    void bind() const;
};
