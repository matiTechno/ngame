#include "flock.hpp"
#include <NGAME/renderer2d.hpp>
#include <NGAME/guru.hpp>
#include <NGAME/glad.h>
#include <NGAME/sprite.hpp>
#include <math.h>
#include <NGAME/app.hpp>

// PC - collision point -> circle center
glm::vec2 getPC(const Circle& circle, const Rect& rect)
{
    auto rectHalfSize = rect.size / 2.f;
    auto rectCenter = rect.pos + rectHalfSize;
    auto RC = circle.pos - rectCenter;
    auto clamped = glm::clamp(RC, -rectHalfSize, rectHalfSize);
    // P - closest collision point
    auto P = rectCenter + clamped;
    auto PC = circle.pos - P;
    return PC;
}

void reflect(Boid& boid, const Circle& circle)
{
    auto CB = boid.pos - circle.pos;
    if(glm::length(CB) < boid.radius + circle.radius)
        boid.vel = glm::reflect(boid.vel, glm::normalize(CB));
}

void reflect(Boid& boid, const Rect& rect)
{
    auto PC = getPC(boid, rect);
    if(glm::length(PC) < boid.radius)
        boid.vel = glm::reflect(boid.vel, glm::normalize(PC));
}

bool isCollision(const Circle& circle, const Rect& rect)
{
    auto PC = getPC(circle, rect);
    if(glm::length(PC) < circle.radius)
        return true;
    return false;
}

void Wall::render(const Renderer2d& renderer)
{
    renderer.render(pos, size, glm::ivec4(), nullptr, 0.f, glm::vec2(), color);
}

void Obstacle::render(guru::Guru& guru)
{
    guru.setGlMode(GL_LINE_LOOP);
    guru::Circle circle;
    circle.pos = pos;
    circle.color = color;
    circle.radius = radius;
    circle.render(guru);
}

void Boid::render(guru::Guru& guru)
{
    guru.setGlMode(GL_TRIANGLES);
    guru::TriangleAgent triangle;
    triangle.pos = pos;
    triangle.size = radius * 2.f;
    triangle.color = color;
    triangle.rotation = atan2f(vel.y, vel.x);
    triangle.render(guru);
}

SceneF::SceneF():
    GuruScene(glm::vec2(0.f, 0.f), glm::vec2(100.f, 100.f))
{
    auto range = getWorldRange();
    auto start = getWorldStart();

    walls[0].pos = start;
    walls[0].size = glm::vec2(1.f, range.y);
    walls[1].pos = walls[0].pos + glm::vec2(range.x - 1.f, 0.f);
    walls[1].size = walls[0].size;
    walls[2].pos = start;
    walls[2].size = glm::vec2(range.x, 1.f);
    walls[3].pos = walls[2].pos + glm::vec2(0.f, range.y - 1.f);
    walls[3].size = walls[2].size;
    for(auto& wall: walls)
        wall.color = glm::vec4(1.f, 0.f, 0.f, 0.5f);
    Obstacle o;
    o.color = glm::vec4(1.f, 1.f, 1.f, 1.f);
    o.pos = glm::vec2(40.f);
    o.radius = 15.f;
    obstacles.push_back(o);
    o.pos = glm::vec2(60.f, 65.f);
    o.radius = 10.f;
    obstacles.push_back(o);
    o.pos = glm::vec2(20.f, 75.f);
    o.radius = 5.f;
    obstacles.push_back(o);
    Boid boid;
    boid.pos = glm::vec2(90.f);
    boid.radius = 3.f;
    boid.vel = glm::vec2(-1.f, 1.f) * 15.f;
    boid.color = glm::vec4(0.f, 1.f, 0.f, 1.f);
    boids.push_back(boid);
}

static Rect rect;

void SceneF::intUpdate()
{
    for(auto& boid: boids)
    {
        boid.pos += boid.vel * io.frametime;

        // * tag possible obstacles

        std::vector<Obstacle*> tagged;
        Rect rect{boid.pos - glm::vec2(boid.sight), glm::vec2(boid.sight * 2.f)};
        renderer2d.render(rect.pos, rect.size, glm::ivec4(), nullptr, 0.f, glm::vec2(), glm::vec4(1.f, 1.f, 0.f, 0.1f));
        for(auto& obstacle: obstacles)
        {
            if(isCollision(obstacle, rect))
            {
                tagged.push_back(&obstacle);
                obstacle.color = glm::vec4(1.f, 0.f, 0.f, 1.f);
            }
            else
                obstacle.color = glm::vec4(0.f, 1.f, 1.f, 1.f);
        }

        // * convert them to boid local space
        // * find nearest collision point
        // * calculate steering force

        for(auto& obstacle: obstacles)
            reflect(boid, obstacle);

        for(auto& wall: walls)
            reflect(boid, wall);
    }
}

void SceneF::worldRender()
{
    intUpdate();

    for(auto& wall: walls)
        wall.render(renderer2d);
    renderer2d.flush();

    for(auto& obstacle: obstacles)
        obstacle.render(guru);
    for(auto& boid: boids)
        boid.render(guru);
}
