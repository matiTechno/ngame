#pragma once
#include "base.hpp"

class VAO: public GL_base
{
public:
    VAO();

    void bind() const;

private:
    static GLuint bound_id;
};

class BO: public GL_base
{
public:
    BO();

    void bind(GLenum target) const;
};

class FBO: public GL_base
{
public:
    FBO();

    void bind(GLenum target = GL_FRAMEBUFFER) const;
};

class RBO: public GL_base
{
    RBO();

    void bind() const;
};
