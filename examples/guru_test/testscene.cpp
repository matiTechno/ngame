#include "testscene.hpp"
#include <NGAME/renderer2d.hpp>
#include <NGAME/guru.hpp>

TestScene::TestScene():
    GuruScene(glm::vec2(0.f, 0.f), glm::vec2(100.f, 100.f))
{}

void TestScene::worldRender()
{
    constexpr int count = 3;
    guru::Vertex vertices[count] = {
        {{20, 20}, {}, {1.f, 0.f, 0.f, 1.f}},
        {{20, 50}, {}, {0.f, 0.f, 1.f, 1.f}},
        {{100, 20}, {}, {0.f, 1.f, 0.f, 1.f}}
    };
    guru.addShape(vertices, count);
}
