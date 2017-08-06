#pragma once

#include <glm/mat4x4.hpp>
#include <array>
#include "gl/shader.hpp"
#include "gl/buffers.hpp"
class Texture;
#define GL_TRIANGLES 0x0004

// brand new renderer
namespace guru
{

struct Vertex
{
    glm::vec2 pos;
    glm::vec2 texCoord;
    glm::vec4 color;
};

class Guru
{
public:
    Guru();

    void addShape(const Vertex* vertices, int count, const glm::mat4& modelMatrix = glm::mat4(1.f));
    void setGlMode(GLenum mode);
    // can be nullptr,
    // only used with glMode = GL_TRIANGLES and similar
    void setTexture(Texture* texture);
    void setBlendFunc(GLenum srcAlpha, GLenum dstAlpha);
    void setFontMode(bool on);
    void setProjection(glm::vec2 start, glm::vec2 range);

private:
    std::array<Vertex, 10000> vertices;
    std::array<glm::mat4, 10000> matrices;
    VAO vao;
    BO vbo, boMat4;
    Shader shader;
    int numVtoRender = 0;
    int numVperShape = 6;
    // ...
    GLenum glMode = GL_TRIANGLES;
    Texture* texture = nullptr;
    // get these values at runtime
    GLenum srcAlpha, dstAlpha;

    bool fontMode = false;

    // call this at the end of Scene::render()
    friend class GuruScene;
    void render();
};

// this shouldn't be there but...
class TriangleAgent
{
public:
    TriangleAgent();
    void render(Guru& guru);

    float rotation;
    glm::vec2 pos;
    float size;
    glm::vec4 color{1.f, 1.f, 1.f, 1.f};

private:
    Vertex vertices[6];
};

class Circle
{
public:
    Circle();
    void render(Guru& guru);

    glm::vec2 pos;
    float radius;
    glm::vec4 color{1.f, 1.f, 1.f, 1.f};

private:
    static constexpr int count = 15;
    Vertex vertices[count];
};

} // guru
