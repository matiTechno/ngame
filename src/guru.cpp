#include <NGAME/guru.hpp>
#include <NGAME/blend.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <NGAME/glad.h>
#include <NGAME/gl/texture.hpp>

namespace guru
{

Guru::Guru():
    shader(
        #include "guru.sh"
        , "guru.sh")
{
    GLint temp1, temp2;
    glGetIntegerv(GL_SRC_ALPHA, &temp1);
    glGetIntegerv(GL_DST_ALPHA, &temp2);
    srcAlpha = temp1;
    dstAlpha = temp2;

    vao.bind();

    vbo.bind(GL_ARRAY_BUFFER);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, pos)));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, texCoord)));
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, color)));

    // model matrix
    boMat4.bind(GL_ARRAY_BUFFER);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glEnableVertexAttribArray(5);
    glEnableVertexAttribArray(6);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), reinterpret_cast<const void*>(0));
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), reinterpret_cast<const void*>(sizeof(float) * 4));
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), reinterpret_cast<const void*>(sizeof(float) * 8));
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), reinterpret_cast<const void*>(sizeof(float) * 12));
}

void Guru::addShape(guru::Vertex* vertices, int count, const glm::mat4& modelMatrix)
{
    assert(numVtoRender + count <= this->vertices.size());

    if(numVperShape != count)
    {
        render();
        numVperShape = count;
    }

    auto numShapes = numVtoRender / numVperShape;
    matrices[numShapes] = modelMatrix;

    for(int i = numVtoRender; i < numVtoRender + count; ++i)
    {
        this->vertices[i] = *vertices;
        ++vertices;
    }
    numVtoRender += count;
}

void Guru::render()
{
    if(numVtoRender == 0)
        return;

    shader.bind();
    if(fontMode)
    {
        assert(texture);
        glUniform1i(shader.get_uni_location("renderType"), 2);
        texture->bind();
    }
    else if(glMode == GL_TRIANGLES && texture)
    {
        glUniform1i(shader.get_uni_location("renderType"), 1);
        texture->bind();
    }
    else
        glUniform1i(shader.get_uni_location("renderType"), 0);

    vbo.bind(GL_ARRAY_BUFFER);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * numVtoRender, vertices.data(), GL_STREAM_DRAW);
    boMat4.bind(GL_ARRAY_BUFFER);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * numVtoRender / numVperShape, matrices.data(), GL_STREAM_DRAW);
    vao.bind();
    glDrawArraysInstanced(GL_TRIANGLES, 0, numVperShape, numVtoRender / numVperShape);
    numVtoRender = 0;
}

void Guru::setGlMode(GLenum mode)
{
    if(glMode != mode)
    {
        render();
        glMode = mode;
    }
}

void Guru::setTexture(Texture* texture)
{
    if(this->texture != texture)
    {
        render();
        this->texture = texture;
    }
}

void Guru::setBlendFunc(GLenum srcAlpha, GLenum dstAlpha)
{
    if(this->srcAlpha != srcAlpha || this->dstAlpha != dstAlpha)
    {
        render();
        this->srcAlpha = srcAlpha;
        this->dstAlpha = dstAlpha;
        Blend::set(srcAlpha, dstAlpha);
    }
}

void Guru::setFontMode(bool on)
{
    if(fontMode != on)
    {
        render();
        fontMode = on;
    }
}

void Guru::setProjection(glm::vec2 start, glm::vec2 range)
{
    render();
    auto projection = glm::ortho(start.x, start.x + range.x, start.y + range.y, start.y);
    shader.bind();
    glUniformMatrix4fv(shader.get_uni_location("projection"), 1, GL_FALSE, &projection[0][0]);
}

} // guru
