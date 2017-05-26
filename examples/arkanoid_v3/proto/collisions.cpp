#include "collisions.hpp"
#include "objects.hpp"
#include <glm/geometric.hpp>

Dir get_dir(const glm::vec2& vec)
{
    glm::vec2 dirs[] =
    {
        {-1.f, 0.f},
        {1.f, 0.f},
        {0.f, -1.f},
        {0.f, 1.f}
    };

    int dir = 0; // to silence warning
    float max = 0.f;

    for(auto i = 0; i < 4; ++i)
    {
        auto dot = glm::dot(glm::normalize(vec), dirs[i]);
        if(dot > max)
        {
            max = dot;
            dir = i;
        }
    }
    return static_cast<Dir>(dir);
}

Collision get_collision(const Bbox& bbox1, const Bbox& bbox2)
{
    Collision collision;
    collision.is = is_collision(bbox1, bbox2);
    if(!collision.is)
        return collision;

    // R1 - bbox1 center
    // R2 - bbox2 center
    // P - point of collision

    auto bb1_h_size = bbox1.size / 2.f;
    auto bb2_h_size = bbox2.size / 2.f;

    auto R1 = bbox1.pos + bb1_h_size;
    auto R2 = bbox2.pos + bb2_h_size;
    auto R2R1 = R1 - R2;
    auto R2P = glm::clamp(R2R1, -bb2_h_size, bb2_h_size);
    auto P = R2 + R2P;
    auto R1P = P - R1;

    collision.pene_dir = get_dir(R1P);
    collision.pene_vec = glm::normalize(R1P) * (bb1_h_size - glm::length(R1P) + add_pene);
    collision.point = P;

    return collision;
}

Collision get_collision(const Ball& ball, const Bbox& bbox)
{
    // B - ball center
    // R - bbox center
    // P - closest point from rect to ball

    auto bb_h_size = bbox.size / 2.f;

    auto B = ball.pos + ball.radius;
    auto R = bbox.pos + bb_h_size;
    auto RB = B - R;
    auto RP = glm::clamp(RB, -bb_h_size, bb_h_size);
    auto P = R + RP;
    auto BP = P - B;

    return Collision{glm::length(BP) < ball.radius, get_dir(BP),
                glm::normalize(BP) * (ball.radius - glm::length(BP) + add_pene), P};
}

bool is_collision(const Bbox& bbox1, const Bbox& bbox2)
{
    auto x = bbox1.pos.x + bbox1.size.x > bbox2.pos.x
            &&
            bbox2.pos.x + bbox2.size.x > bbox1.pos.x;

    auto y = bbox1.pos.y + bbox1.size.y > bbox2.pos.y
            &&
            bbox2.pos.y + bbox2.size.y > bbox1.pos.y;

    return x && y;
}

void reflect_vel(Ball& ball, const Collision& collision)
{
    switch(collision.pene_dir)
    {
    case Dir::left:
        ball.vel.x = glm::abs(ball.vel.x);
        break;

    case Dir::right:
        ball.vel.x = -glm::abs(ball.vel.x);
        break;

    case Dir::top:
        ball.vel.y = glm::abs(ball.vel.y);
        break;

    case Dir::down:
        ball.vel.y = -glm::abs(ball.vel.y);
    }
}
