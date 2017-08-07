#include "flock.hpp"
#include <NGAME/renderer2d.hpp>
#include <NGAME/guru.hpp>
#include <NGAME/glad.h>
#include <NGAME/sprite.hpp>
#include <math.h>
#include <NGAME/app.hpp>
#include <glm/gtx/rotate_vector.hpp>

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
    triangle.rotation = dirAngle;
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
    {
        Boid boid;
        boid.pos = glm::vec2(50.f, 92.f);
        boid.radius = 3.f;
        boid.vel = glm::normalize(glm::vec2(-1.f, 1.f)) * 25.f;
        boid.color = glm::vec4(0.f, 1.f, 0.f, 1.f);
        boids.push_back(boid);
    }
    {
        Boid boid;
        boid.pos = glm::vec2(90.f);
        boid.radius = 3.f;
        boid.vel = glm::normalize(glm::vec2(-1.f, 1.f)) * 25.f;
        boid.color = glm::vec4(0.f, 1.f, 0.f, 1.f);
        boids.push_back(boid);
    }
    {
        Boid boid;
        boid.pos = glm::vec2(15.f);
        boid.radius = 3.f;
        boid.vel = glm::normalize(glm::vec2(-1.f, 1.f)) * 25.f;
        boid.color = glm::vec4(0.f, 1.f, 0.f, 1.f);
        boids.push_back(boid);
    }
    {
        Boid boid;
        boid.pos = glm::vec2(60.f, 20.f);
        boid.radius = 3.f;
        boid.vel = glm::normalize(glm::vec2(-1.f, 1.f)) * 25.f;
        boid.color = glm::vec4(0.f, 1.f, 0.f, 1.f);
        boids.push_back(boid);
    }
}

static Rect rect;

void SceneF::intUpdate()
{
    for(auto& boid: boids)
    {
        auto accLen = glm::length(boid.acc);
        if(accLen > boid.maxAcc)
            boid.acc = boid.maxAcc * glm::normalize(boid.acc);

        auto velLen = glm::length(boid.vel);
        if(velLen > boid.maxVel)
            boid.vel = boid.maxVel * glm::normalize(boid.vel);

        boid.vel += boid.acc * io.frametime;
        boid.pos += boid.vel * io.frametime;
        boid.acc = glm::vec2(0.f);
        boid.dirAngle = atan2f(boid.vel.y, boid.vel.x);
        boid.sight = glm::length(boid.vel * 0.5f);

        // 1: tag possible obstacles

        std::vector<Circle*> tagged;
        Rect rect{boid.pos - glm::vec2(boid.sight), glm::vec2(boid.sight * 2.f)};

        // detection area visualisation
        {
            //renderer2d.render(rect.pos, rect.size, glm::ivec4(), nullptr, 0.f, glm::vec2(), glm::vec4(1.f, 1.f, 0.f, 0.05f));

            glm::vec2 pos(boid.pos.x, boid.pos.y - boid.radius);
            renderer2d.render(pos, glm::vec2(boid.sight, boid.radius * 2.f),
                              glm::ivec4(), nullptr, -boid.dirAngle, boid.pos - pos, glm::vec4(1.f, 1.f, 0.f, 0.2f));
        }

        for(auto& obstacle: obstacles)
        {
            if(isCollision(obstacle, rect))
                tagged.push_back(&obstacle);
        }
        for(auto& obstacle: boids)
        {
            if(&obstacle != &boid && isCollision(obstacle, rect))
                tagged.push_back(&obstacle);
        }

        float collDistX = 1000.f; // @ make more generic
        float collLocalY;
        for(auto ptr: tagged)
        {
            // 2: convert them to boid local space

            auto& obstacle = *ptr;
            auto localPos = glm::rotate(obstacle.pos - boid.pos, -boid.dirAngle);

            // 3: find nearest collision point

            auto expandedRadius = boid.radius + obstacle.radius;

            if(localPos.x < 0.f || glm::abs(localPos.y) > expandedRadius)
                continue;

            auto sqrtPart = glm::sqrt(expandedRadius * expandedRadius - localPos.y * localPos.y);
            auto x = localPos.x - sqrtPart;
            if(x < 0.f)
                x = localPos.x + sqrtPart;

            if(x < collDistX)
            {
                collDistX = x;
                collLocalY = localPos.y;
            }
        }

        // 4: calculate steering force

        if(collDistX != 0.f)
        {
            float coeff = 1.f + (boid.sight - collDistX) / collDistX;
            auto localForce = glm::vec2(-30.f, (-collLocalY) * 5.f) * coeff;
            boid.acc += glm::rotate(localForce, boid.dirAngle);
        }

        if(glm::length(boid.vel) < boid.maxVel)
            boid.acc += 30.f * glm::normalize(boid.vel);

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
