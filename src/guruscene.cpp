#include <NGAME/guruscene.hpp>
#include <NGAME/guru.hpp>
#include <NGAME/app.hpp>
#include <NGAME/renderer2d.hpp>

namespace guru
{

GuruScene::GuruScene(glm::vec2 start, glm::vec2 range):
    guru(*App::nonConstHandle->guru),
    worldStart(start),
    worldRange(range)
{}

void GuruScene::start()
{
    size.x = io.w;
    size.y = io.h;
    setProjection();
    start2();
}

void GuruScene::worldRender()
{}

void GuruScene::start2()
{}

void GuruScene::render()
{
    renderer2d.set_projection(projStart, projRange);
    guru.setProjection(projStart, projRange);
    worldRender();
    guru.render();
}

void GuruScene::setProjection()
{
    projRange = worldRange;
    auto worldAspect = worldRange.x / worldRange.y;
    if(worldAspect < io.aspect)
        projRange.x = io.aspect * projRange.y;

    else if(worldAspect > io.aspect)
        projRange.y = projRange.x / io.aspect;

    projStart = worldStart - (projRange - worldRange) / 2.f;
}

} // guru
