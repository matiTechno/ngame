#include "editor.hpp"
#include <NGAME/font_loader.hpp>
#include <NGAME/renderer2d.hpp>
#include <NGAME/app.hpp>
#include <NGAME/blend.hpp>
#include <NGAME/sprite.hpp>
#include <NGAME/font.hpp>
#include <NGAME/text.hpp>
#include <sstream>
#include <NGAME/glad.h>

Editor::Editor(const char* filename)
{
    file.open(filename);
    if(!file.is_open())
    {
        file.clear();
        file.open(filename, std::ios::out);
        if(!file.is_open())
            throw std::runtime_error("could not open file" + std::string(filename));
    }
    std::stringstream ss;
    ss << file.rdbuf();
    std::string fileInput;
    fileInput.reserve(1000);
    fileInput = ss.str();
    file.close();
    file.open(filename, std::ios::out);
    if(!file.is_open())
        throw std::runtime_error("could not open file");

    font = std::make_unique<Font>(font_loader.load_font("ProggyCleanCE.ttf", 18));
    text = std::make_unique<Text>(*font);
    text->color = glm::vec4(1.f, 1.f, 0.f, 1.f);
    text->pos = glm::vec2(0.f, font->get_ascent());
    text->text.reserve(1000);
    penPos = text->pos;
    add(fileInput);

    SDL_GL_SetSwapInterval(0);
    glClearColor(0.f, 0.15f, 0.f, 1.f);
}

Editor::~Editor()
{
    file << text->text;
    if(text->text.back() != '\n')
        file << '\n';
}

void Editor::start()
{
    size.x = io.w;
    size.y = io.h;
}

void Editor::add(int code)
{
    if(code == '\n')
    {
        lineEndings.push_back(penPos);
        penPos.y += font->get_linespace();
        penPos.x = 0.f;
    }
    else
        penPos.x += font->get_glyph(code).advance;

    text->text.push_back(code);
}

void Editor::add(std::string str)
{
    for(auto c: str)
        add(c);
}

void Editor::erase()
{
    if(text->text.size())
    {
        if(text->text.back() == '\n')
        {
            penPos = lineEndings.back();
            lineEndings.pop_back();
        }
        else
            penPos.x -= font->get_glyph(text->text.back()).advance;

        text->text.pop_back();
    }
}

void Editor::process_input()
{
    for(auto& event: io.events)
    {
        if(event.type != SDL_KEYDOWN)
            return;

        accumulator = 0.f;
        visible = true;

        auto key = event.key.keysym.sym;
        auto upper = SDL_GetModState() & (KMOD_CAPS | KMOD_SHIFT);

        if(key == SDLK_RETURN)
            add('\n');

        else if(key == SDLK_BACKSPACE && text->text.size())
            erase();

        else if(key >= 'a' && key <= 'z')
        {
            int val = 0;
            if(upper)
                val -= 32;
            add(key + val);
        }
        else if(key == ' ')
            add(' ');

        else if(key == SDLK_TAB)
            add("    ");

        // I could use contidional operator, but...
        else if(key == ',') {if(upper) add('<'); else add(',');}
        else if(key == '.') {if(upper) add('>'); else add('.');}
        else if(key == '/') {if(upper) add('?'); else add('/');}
        else if(key == ';') {if(upper) add(':'); else add(';');}
        else if(key == '\''){if(upper) add('\"');else add('\'');}
        else if(key == '[') {if(upper) add('{'); else add('[');}
        else if(key == ']') {if(upper) add('}'); else add(']');}
        else if(key == '\\'){if(upper) add('|'); else add('\\');}
        else if(key == '`') {if(upper) add('~'); else add('`');}
        else if(key == '1') {if(upper) add('!'); else add('1');}
        else if(key == '2') {if(upper) add('@'); else add('2');}
        else if(key == '3') {if(upper) add('#'); else add('3');}
        else if(key == '4') {if(upper) add('$'); else add('4');}
        else if(key == '5') {if(upper) add('%'); else add('5');}
        else if(key == '6') {if(upper) add('^'); else add('6');}
        else if(key == '7') {if(upper) add('&'); else add('7');}
        else if(key == '8') {if(upper) add('*'); else add('8');}
        else if(key == '9') {if(upper) add('('); else add('9');}
        else if(key == '0') {if(upper) add(')'); else add('0');}
        else if(key == '-') {if(upper) add('_'); else add('-');}
        else if(key == '=') {if(upper) add('+'); else add('=');}
    }
}

void Editor::render()
{
    accumulator += io.frametime;
    while(accumulator >= blinkTime)
    {
        accumulator -= blinkTime;
        visible = !visible;
    }
    renderer2d.set_projection(glm::vec2(0.f, 0.f), size);
    if(visible)
    {
        auto proto = font->get_glyph('W');
        Sprite sprite;
        sprite.pos = glm::vec2(penPos.x + proto.bearing.x, penPos.y - proto.bearing.y - 2.f);
        sprite.size = glm::vec2(proto.tex_coords.z, proto.tex_coords.w + 4.f);
        sprite.color = glm::vec4(1.f, 1.f, 0.f, 1.f);
        renderer2d.render(sprite);
    }
    renderer2d.render(*text);
    renderer2d.flush();
}
