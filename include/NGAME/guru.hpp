#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
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
};

// BUG: adding different shape with current number of vertices per instance
// will not flush the buffers
// this class needs major redesign
class Guru
{
public:
    Guru();

    void addShape(const Vertex* vertices, int count, const glm::vec4& color = glm::vec4(1.f),
                  const glm::mat4& modelMatrix = glm::mat4(1.f));
    void setGlMode(GLenum mode);
    // can be nullptr,
    // only used with glMode = GL_TRIANGLES and similar
    void setTexture(Texture* texture);
    void setBlendFunc(GLenum srcAlpha, GLenum dstAlpha);
    void setFontMode(bool on);
    void setProjection(glm::vec2 start, glm::vec2 range);

private:
    std::array<Vertex, 100> vertices;
    std::array<glm::mat4, 10000> matrices;
    std::array<glm::vec4, 10000> colors;
    VAO vao;
    BO vbo, boMat4, boColor;
    Shader shader;
    int numVtoRender = 0;
    int numVperShape;
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
