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
    glm::vec3 color{1.f, 1.f, 1.f};
    // cube model has (1, 1, 1) size
    float scale = 0.1f;
    float attenuation = 0.7f;
};

class Renderer3d
{
public:
    Renderer3d();

    void render(const Inst3d& instance) const;

    void set_camera(const Camera& camera) const;

    // sets active_l to -1
    // call this after rendering all 3d instances
    // warning: does not use instancing
    void render_lights() const;

    // call this every frame
    void set_light(const Light& light) const;

private:
    Shader shader;
    Shader shader_light;
    VAO vao_light;
    BO vbo_light;
    // in shader it is set to 100
    mutable std::array<Light, 100> lights;
    mutable int active_l = -1;
};
