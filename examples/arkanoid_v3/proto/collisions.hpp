#pragma once
class Bbox;
class Ball;
#include <glm/vec2.hpp>

enum class Dir
{
    left,
    right,
    top,
    down
};

struct Collision
{
    bool is;
    Dir pene_dir;
    glm::vec2 pene_vec;
    glm::vec2 point;
};

// returned Collision is valid for first object
Collision get_collision(Bbox& bbox1, Bbox& bbox2);

Collision get_collision(Ball& ball, Bbox& bbox);

void reflect_vel(Ball& ball, const Collision& collision);
