#pragma once
#include <NGAME/gl/shader.hpp>
class Model;

class Renderer3d
{
public:
    Renderer3d();

    void render(const Model& model);

private:
    Shader shader;
};
