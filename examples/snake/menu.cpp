#include "menu.hpp"
#include <NGAME/sprite.hpp>
#include <NGAME/renderer2d.hpp>
#include <NGAME/app.hpp>
#include <NGAME/text.hpp>
#include "snake.hpp"

constexpr int Menu::beg_option;

Menu::Menu(Snake& snake, bool paused, int score, const Font& font):
    snake(snake),
    font(font),
    paused(paused)
{
    is_opaque = false;
    const int num_texts = 4;
    texts.reserve(4);
    for(int i = 0; i < num_texts; ++i)
        texts.emplace_back(font);
    if(paused)
    {
        texts[0].text = "game paused";
        texts[beg_option].text = "resume";
    }
    else
    {
        texts[0].text = "game over";
        texts[beg_option].text = "start again";
    }
    texts[1].text = "score: " + std::to_string(score);
    texts[beg_option + 1].text = "exit";

    size.x = 0;
    const float spacing = 15.f;
    size.y = (texts.size() - 1) * spacing;
    auto pos_y = font.get_ascent();
    for(auto& text: texts)
    {
        auto tsize = text.get_size();
        text.pos.y = pos_y;
        pos_y += tsize.y + spacing;
        size.y += tsize.y;
        if(tsize.x > size.x)
            size.x = tsize.x;
    }
    for(auto& text: texts)
        text.pos.x = size.x / 2.f - text.get_size().x / 2.f;
}

void Menu::set_coords()
{
    pos = glm::ivec2(io.w, io.h) / 2 - size / 2;
}

void Menu::process_input()
{
    time += io.frametime;

    for(auto& event: io.events)
    {
        if(event.type == SDL_KEYDOWN)
        {
            auto key = event.key.keysym.sym;
            if(key == SDLK_ESCAPE && paused)
                scenes_to_pop = 1;

            else if(key == SDLK_s || key == SDLK_DOWN)
            {
                ++option;
                if(option == texts.size())
                    option = beg_option;
            }
            else if(key == SDLK_w || key == SDLK_UP)
            {
                --option;
                if(option == beg_option - 1)
                    option = texts.size() - 1;
            }
            else if(key == SDLK_RETURN || key == SDLK_SPACE)
            {
                switch(option)
                {
                case beg_option:
                    if(paused == 0)
                        snake.init_level();
                    scenes_to_pop = 1;
                    break;

                case beg_option + 1:
                    quit();
                }
            }
        }
    }
    for(int i = beg_option; i < texts.size(); ++i)
    {
        if(i == option)
            texts[i].color = glm::vec4(1.f, 1.f, 0.f, 1.f);
        else
            texts[i].color = glm::vec4(1.f, 1.f, 1.f, 0.5f);
    }
}

void Menu::render()
{
    renderer2d.set_projection(glm::vec2(0.f), size);
    Sprite sprite;
    sprite.pos = glm::vec2(0.f);
    sprite.size = size;
    sprite.color = glm::vec4(0.f, 0.f, 0.f, 0.85f);
    renderer2d.render(sprite);
    for(auto& text: texts)
        renderer2d.render(text);
    renderer2d.flush();
}
