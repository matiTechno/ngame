#pragma once

#include "../gl/buffers.hpp"
#include <string>
#include <vector>

// only works with triangle faces
class Model
{
public:
    Model(const std::string& filename);

private:
    friend class Renderer3d;
    struct Mesh
    {
        VAO vao;
        BO vbo;
        int num_vertices;
    };
    std::vector<Mesh> meshes;
};
