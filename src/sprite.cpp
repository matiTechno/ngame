#include <NGAME/sprite.hpp>
#include <NGAME/text.hpp>

Sprite::Sprite(const Text& text):
    rotation(text.rotation)
{
    auto bb = text.get_bbox();
    pos.x = bb.x;
    pos.y = bb.y;
    size.x = bb.z;
    size.y = bb.w;
    rotation_point = text.pos - pos + text.rotation_point;
}
