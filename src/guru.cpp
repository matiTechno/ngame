#include <NGAME/guru.hpp>
#include <NGAME/blend.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <NGAME/glad.h>
#include <NGAME/gl/texture.hpp>

namespace guru
{

TriangleAgent::TriangleAgent()
{
    vertices[0].pos = glm::vec2(-0.5f, 0.35f);
    vertices[1].pos = glm::vec2(0.f, 0.f);
    vertices[2].pos = glm::vec2(0.5f, 0.f);
    vertices[3].pos = glm::vec2(-0.5f, -0.35f);
    vertices[4].pos = glm::vec2(0.f, 0.f);
    vertices[5].pos = glm::vec2(0.5f, 0.f);
}

void TriangleAgent::render(Guru& guru)
{
    for(int i = 0; i < 6; ++i)
        vertices[i].color = color;

    glm::mat4 modelMatrix(1.f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(pos, 0.f));
    modelMatrix = glm::rotate(modelMatrix, rotation, glm::vec3(0.f, 0.f, 1.f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(size, size, 1.f));
    guru.addShape(vertices, 6, modelMatrix);
}

Circle::Circle()
{
    auto a = 2.f * glm::pi<float>() / count;
    for(int i = 0; i < count; ++i)
        vertices[i].pos = glm::vec2(glm::sin(i * a), glm::cos(i * a));
}

void Circle::render(Guru& guru)
{
    for(int i = 0; i < count; ++i)
        vertices[i].color = color;

    glm::mat4 modelMatrix(1.f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(pos, 0.f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(radius, radius, 1.f));
    guru.addShape(vertices, count, modelMatrix);
}

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

void Guru::addShape(const guru::Vertex* vertices, int count, const glm::mat4& modelMatrix)
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
    glDrawArraysInstanced(glMode, 0, numVperShape, numVtoRender / numVperShape);
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
