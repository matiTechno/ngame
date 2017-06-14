#include <NGAME/3d/model.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <NGAME/glad.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

struct Vertex
{
    glm::vec3 pos;
};

Model::Model(const std::string& filename)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;

    auto success = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename.c_str());

    if(err.size())
        std::cout << "tinyobjloader, filename = " << filename << ": " << err << std::endl;

    if(!success)
        throw std::exception();

    meshes.reserve(shapes.size());

    std::vector<Vertex> vertices;
    // something better?
    vertices.reserve(attrib.vertices.size());

    for(auto& shape: shapes)
    {
        std::size_t index_offset = 0;
        for(auto num: shape.mesh.num_face_vertices)
        {
            for(std::size_t i = 0; i < num; ++i)
            {
                auto id = shape.mesh.indices[index_offset + i];
                auto vx = attrib.vertices[3 * id.vertex_index + 0];
                auto vy = attrib.vertices[3 * id.vertex_index + 1];
                auto vz = attrib.vertices[3 * id.vertex_index + 2];

                vertices.push_back({{vx, vy, vz}});
            }
            index_offset += num;
        }
        meshes.emplace_back();
        auto& mesh = meshes.back();
        mesh.num_vertices = vertices.size();
        mesh.vbo.bind(GL_ARRAY_BUFFER);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
        mesh.vao.bind();
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);
        vertices.clear();
    }
}
