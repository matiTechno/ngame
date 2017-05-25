#include "collisions.hpp"

Collision get_collision(Bbox& bbox1, Bbox& bbox2)
{
    (void)bbox1;
    (void)bbox2;
    return Collision();
}

Collision get_collision(Ball& ball, Bbox& bbox)
{
    (void)ball;
    (void)bbox;
    return Collision();
}

void reflect_vel(Ball& ball, const Collision& collision)
{
    (void)ball;
    (void)collision;
}
