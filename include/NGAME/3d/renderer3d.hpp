#pragma once
#include "../gl/shader.hpp"
class Inst3d;
class Camera;

class Renderer3d
{
public:
    Renderer3d();

    void render(const Inst3d& instance) const;

    void set_camera(const Camera& camera) const;

private:
    Shader shader;
};
