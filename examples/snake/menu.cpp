#include "menu.hpp"
#include <NGAME/sprite.hpp>
#include <NGAME/renderer2d.hpp>
#include <NGAME/app.hpp>

Menu::Menu(const std::string& title, int score, bool end):
    title(title),
    str_score("score: " + std::to_string(score)),
    end(end)
{
    is_opaque = false;
    size = glm::ivec2(400);
    pos = glm::ivec2(io.h, io.w) / 2 - size / 2;
}

void Menu::process_input()
{
    for(auto& event: io.events)
    {
        if(event.type == SDL_KEYDOWN)
        {
            auto key = event.key.keysym.sym;
            if(key == SDLK_ESCAPE)
                scenes_to_pop = 1;
        }
    }
}

void Menu::render()
{
    renderer2d.set_projection(glm::vec2(0.f), size);
    Sprite sprite;
    sprite.pos = glm::vec2(0.f);
    sprite.size = size;
    sprite.color = glm::vec4(0.f, 0.f, 0.f, 0.7f);
    renderer2d.render(sprite);
    renderer2d.flush();
}
