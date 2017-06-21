#include "testscene.hpp"
#include <NGAME/renderer2d.hpp>
#include <NGAME/guru.hpp>
#include <NGAME/glad.h>

TestScene::TestScene():
    GuruScene(glm::vec2(0.f, 0.f), glm::vec2(100.f, 100.f))
{}

void TestScene::worldRender()
{
    guru::TriangleAgent triangle;
    triangle.pos = glm::vec2(50.f);
    triangle.size = 5.f;
    triangle.color = glm::vec4(0.f, 1.f, 0.f, 1.f);
    guru.setGlMode(GL_TRIANGLES);
    triangle.render(guru);

    guru::Circle circle;
    circle.pos = glm::vec2(30.f);
    circle.radius = 20.f;
    guru.setGlMode(GL_LINE_LOOP);
    circle.render(guru);
}
