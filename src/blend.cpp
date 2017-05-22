#include <NGAME/blend.hpp>
#define NGAME_BLEND
#include <NGAME/glad.h>

void Blend::set(GLenum src, GLenum dst)
{
    glBlendFuncSeparate(src, dst, GL_ONE, GL_ONE);
}

void Blend::set_default()
{
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
}
