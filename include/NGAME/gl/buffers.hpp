#pragma once
#include "base.hpp"

// access id by friend declaration

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

    friend class Gravity;
    friend class Mandelbrot;
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
