#include "menu.hpp"
#include "NGAME/common.hpp"

Menu::Menu(const Font& font):
    font(font)
{
    is_opaque = false;
    size = glm::ivec2(250.f);
    pos = glm::ivec2(40.f, 400.f);
}

void Menu::process_input()
{
    for(auto& event: io.events)
    {
        if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
            scenes_to_pop = 1;
    }
}

void Menu::render()
{
    renderer2d.set_projection(glm::vec2(0.f), size);

    Sprite sprite;
    sprite.pos = glm::vec2(0.f);
    sprite.size = size;
    sprite.color = glm::vec4(0.f, 0.f, 0.f, 0.5f);
    renderer2d.render(sprite);

    Text text(font);
    text.text = "menu";
    text.color = glm::vec4(0.f, 1.f, 1.f, 1.f);

    text.pos = glm::vec2(size.x / 2 - text.get_size().x / 2, font.get_ascent());
    renderer2d.render(text);

    renderer2d.flush();
}
