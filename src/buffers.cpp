#include <NGAME/gl/buffers.hpp>

GLuint VAO::bound_id = 0;

VAO::VAO():
    Del([this](){
    glDeleteVertexArrays(1, &id);
    if(bound_id == id)
        bound_id = 0;
})
{
    glGenVertexArrays(1, &id);
}

void VAO::bind() const
{
    if(bound_id != id)
    {
    glBindVertexArray(id);
    bound_id = id;
    }
}

BO::BO():
    Del([this](){
    glDeleteBuffers(1, &id);
})
{
    glGenBuffers(1, &id);
}

void BO::bind(GLenum target) const
{
    glBindBuffer(target, id);
}

FBO::FBO():
    Del([this](){
    glDeleteFramebuffers(1, &id);
})
{
    glGenFramebuffers(1, &id);
}

void FBO::bind(GLenum target) const
{
    glBindFramebuffer(target, id);
}

RBO::RBO():
    Del([this](){
    glDeleteRenderbuffers(1, &id);
})
{
    glGenRenderbuffers(1, &id);
}

void RBO::bind() const
{
    glBindRenderbuffer(GL_RENDERBUFFER, id);
}
