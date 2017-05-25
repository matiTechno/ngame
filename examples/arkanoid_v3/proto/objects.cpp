#include "objects.hpp"
#include <NGAME/renderer2d.hpp>
#include <glm/trigonometric.hpp>

void Wall::render(const Renderer2d& renderer) const
{
    renderer.render(pos, size, glm::ivec4(), nullptr, 0.f, glm::vec2(), glm::vec4(1.f, 0.7f, 0.f, 0.3f));
}

void Brick::render(const Renderer2d& renderer) const
{
    renderer.render(pos, size, tex_coords, texture, 0.f, glm::vec2(), glm::vec4(1.f, 1.f, 1.f, 1.f));
}

void Life_bar::update(float dt)
{
    time += dt;
    auto coeff = glm::sin(time) * 0.5f + 0.8f;
    color.a = coeff;
}

void Life_bar::render(const Renderer2d& renderer) const
{
    auto pos = this->pos;
    for(auto i = 0; i < lifes; ++i)
    {
        renderer.render(pos, glm::vec2(quad_size), glm::ivec4(), nullptr, 0.f, glm::vec2(), color);
        pos.x += quad_size + spacing;
    }
}

// not fully implemented
void Ball::spawn(const Paddle& paddle, bool dir)
{
    (void)dir;
    pos.y = paddle.pos.y - 2 * radius;
    pos.x = paddle.pos.x + paddle.size.x / 2.f - radius;
}

void Ball::render(const Renderer2d& renderer) const
{
    renderer.render(pos, glm::vec2(radius * 2.f), glm::ivec4(0, 0, texture->get_size()), texture, 0.f, glm::vec2(),
                    glm::vec4(1.f, 1.f, 1.f, 0.5f));
}

void Paddle::render(const Renderer2d& renderer) const
{
    renderer.render(pos, size, glm::ivec4(), nullptr, 0.f, glm::vec2(), glm::vec4(1.f, 1.f, 1.f, 0.6f));
}
