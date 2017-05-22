#include "menu.hpp"
#include "NGAME/common.hpp"

Menu::Menu(const Font& font, Shader& shader):
    font(font),
    shader(shader)
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

    {
        pp_unit.start();
        Sprite sprite;
        sprite.pos = glm::vec2(0.f);
        sprite.size = size;
        sprite.color = glm::vec4(1.f, 0.f, 1.f, 0.7f);
        renderer2d.render(sprite);
        renderer2d.flush();

        time += io.frametime;
        shader.bind();
        glUniform1f(shader.get_uni_location("time"), time);
        glUniform4i(shader.get_uni_location("scene_coords"), pos.x, get_gl_y(), size.x, size.y);
        pp_unit.render(true, shader);
    }

    Text text(font);
    text.text = "menu";
    text.color = glm::vec4(0.f, 1.f, 1.f, 0.8f);

    text.pos = glm::vec2(size.x / 2 - text.get_size().x / 2, font.get_ascent());
    renderer2d.render(text);

    renderer2d.flush();
}
