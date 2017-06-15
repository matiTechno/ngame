#pragma once

#include <glm/vec3.hpp>
class Model;

struct Material
{
    Material() = default;
    Material(int mat);
    enum
    {
        yellow_rubber,
        emerald,
        gold,
        test,
        // ...
        total
    };
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

struct Inst3d
{
    // must not be null when passed to
    // renderer3d
    const Model* model = nullptr;

    glm::vec3 pos = glm::vec3(0.f);
    glm::vec3 scale = glm::vec3(1.f);
    glm::vec3 rotation = glm::vec3(0.f);

    Material mat;
};

inline Material::Material(int mat_t)
{
    assert(mat_t > -1 && mat_t < total);
    switch(mat_t)
    {
    case yellow_rubber:
        ambient = {0.05f, 0.05f, 0.f};
        diffuse = {0.5f, 0.5f, 0.4f};
        specular = {0.7f, 0.7f, 0.04f};
        shininess = 10.f;
        break;

    case emerald:
        ambient = {0.0215f, 0.1745f, 0.0215f};
        diffuse = {0.07568f, 0.61424f, 0.07568f};
        specular = {0.633f, 0.727811f, 0.633f};
        shininess = 76.f;
        break;

    case gold:
        ambient = {0.24725f, 0.1995f, 0.0745f};
        diffuse = {0.75164f, 0.60648f, 0.22648f};
        specular = {0.628281f, 0.555802f, 0.366065f};
        shininess = 51.2f;
        break;

    case test:
        ambient = glm::vec3(0.1f);
        diffuse = glm::vec3(1.f);
        specular = glm::vec3(0.5f);
        shininess = 128.f;
        break;
    }
}
