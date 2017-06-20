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

    void addShape(Vertex* vertices, int count, const glm::mat4& modelMatrix = glm::mat4(1.f));
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

    // call this at the end of Scene::render()
    friend class GuruScene;
    void render();bool fontMode = false;
};

} // guru
