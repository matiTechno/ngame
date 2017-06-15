#pragma once
#include "../gl/shader.hpp"
class Inst3d;
class Camera;
#include <glm/vec3.hpp>
#include <NGAME/gl/buffers.hpp>
#include <array>

struct Light
{
    glm::vec3 pos;
    glm::vec3 color;
};

class Renderer3d
{
public:
    Renderer3d();

    void render(const Inst3d& instance) const;

    void set_camera(const Camera& camera) const;

    // this will set active_l to -1
    // call this after rendering all 3d instances
    void render_lights() const;

    // call this every frame
    void set_light(const Light& light) const;

private:
    Shader shader;
    Shader shader_light;
    VAO vao_light;
    BO vbo_light;
    // in shader it is set to 20
    mutable std::array<Light, 20> lights;
    mutable int active_l = -1;
};
