#include "flock.hpp"
#include <NGAME/renderer2d.hpp>
#include <NGAME/guru.hpp>
#include <NGAME/glad.h>
#include <NGAME/sprite.hpp>
#include <math.h>
#include <NGAME/app.hpp>

void reflect(Boid& boid, const Circle& obstacle)
{
    // B - boid center, O - obstacle center
    auto OB = boid.pos - obstacle.pos;
    if(glm::length(OB) < boid.radius + obstacle.radius)
        boid.vel = glm::reflect(boid.vel, glm::normalize(OB));
}

void reflect(Boid& boid, const Wall& wall)
{
    auto wallHalfSize = wall.size / 2.f;
    auto wallCenter = wall.pos + wallHalfSize;

    // B - boid center, W - wall center
    auto WB = boid.pos - wallCenter;
    auto clamped = glm::clamp(WB, -wallHalfSize, wallHalfSize);
    // P - closest collision point
    auto P = wallCenter + clamped;
    auto PB = boid.pos - P;

    if(glm::length(PB) < boid.radius)
        boid.vel = glm::reflect(boid.vel, glm::normalize(PB));
}

void Wall::render(const Renderer2d& renderer)
{renderer.render(pos, size, glm::ivec4(), nullptr, 0.f, glm::vec2(), glm::vec4(1.f, 0.f, 0.f, 0.5f));}

void Obstacle::render(guru::Guru& guru)
{
    guru.setGlMode(GL_LINE_LOOP);
    guru::Circle circle;
    circle.pos = pos;
    circle.color = glm::vec4(0.f, 1.f, 1.f, 1.f);
    circle.radius = radius;
    circle.render(guru);
}

void Boid::render(guru::Guru& guru)
{
    guru.setGlMode(GL_TRIANGLES);
    guru::TriangleAgent triangle;
    triangle.pos = pos;
    triangle.size = radius * 2.f;
    triangle.color = glm::vec4(0.f, 1.f, 0.f, 1.f);
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

    Obstacle obs;
    obs.pos = glm::vec2(40.f);
    obs.radius = 15.f;
    obstacles.push_back(obs);

    Boid boid;
    boid.pos = glm::vec2(70.f);
    boid.radius = 3.f;
    boid.vel = glm::vec2(-1.f, 1.f) * 15.f;
    boids.push_back(boid);
}

void SceneF::intUpdate()
{
    for(auto& boid: boids)
    {
        boid.pos += boid.vel * io.frametime;

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
