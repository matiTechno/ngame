#pragma once
class Bbox;
class Ball;
#include <glm/vec2.hpp>

// it is added to pene_vec to avoid some floating point errors
// when correcting position
static constexpr float add_pene = 0.001f;

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

// if there is no collision does early return
Collision get_collision(const Bbox& bbox1, const Bbox& bbox2);

Collision get_collision(const Ball& ball, const Bbox& bbox);

// for more lightweight stuff
bool is_collision(const Bbox& bbox1, const Bbox& bbox2);

void reflect_vel(Ball& ball, const Collision& collision);
