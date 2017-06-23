#include "editor.hpp"
#include <NGAME/font_loader.hpp>
#include <NGAME/renderer2d.hpp>
#include <NGAME/app.hpp>
#include <NGAME/sprite.hpp>
#include <NGAME/font.hpp>
#include <NGAME/text.hpp>
#include <sstream>
#include <NGAME/glad.h>
#include <fstream>

Editor::Editor(const char* filename):
    filename(filename)
{
    std::fstream file;
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
    fileInput = ss.str();
    font = std::make_unique<Font>(font_loader.load_font("ProggyCleanCE.ttf", 18));
    margin.x = font->get_glyph(' ').advance;
    margin.y = font->get_linespace() + font->get_ascent();
    {
        lines.push_back(Text(*font));
        lines.back().pos = margin;
        lines.back().color = textColor;

        if(fileInput.size() && fileInput.back() == '\n')
            fileInput.pop_back();

        for(auto c: fileInput)
        {
            if(c == '\n')
            {
                lines.push_back(Text(*font));
                lines.back().pos = glm::vec2(margin.x, margin.y + font->get_linespace() * (lines.size() - 1));
                lines.back().color = textColor;
            }
            else
                lines.back().text.push_back(c);
        }
    }
    SDL_GL_SetSwapInterval(0);
    glClearColor(0.f, 0.1f, 0.f, 1.f);
}

void Editor::add(int code)
{
    lines[cursorPos.y].text.insert(cursorPos.x, 1, code);
    ++cursorPos.x;
}
void Editor::erase()
{
    if(cursorPos.x == 0 && cursorPos.y)
    {
        cursorPos.x = lines[cursorPos.y - 1].text.size();
        lines[cursorPos.y - 1].text.append(lines[cursorPos.y].text);
        lines.erase(lines.begin() + cursorPos.y);
        for(int i = cursorPos.y; i < lines.size(); ++i)
            lines[i].pos -= font->get_linespace();
        --cursorPos.y;
    }
    else if(cursorPos.x != 0)
    {
        lines[cursorPos.y].text.erase(cursorPos.x - 1, 1);
        --cursorPos.x;
    }
}

void Editor::add(std::string str)
{
    for(auto c: str)
        add(c);
}

void Editor::addNewLine()
{
    lines.insert(lines.begin() + cursorPos.y, Text(*font));
    ++cursorPos.y;

    auto& movedLine = lines[cursorPos.y];
    auto& insertedLine = lines[cursorPos.y - 1];

    insertedLine.color = textColor;
    insertedLine.pos = movedLine.pos;

    insertedLine.text = movedLine.text.substr(0, cursorPos.x);
    movedLine.text.erase(0, cursorPos.x);

    cursorPos.x = 0;

    for(int i = cursorPos.y; i < lines.size(); ++i)
        lines[i].pos.y += font->get_linespace();
}

Editor::~Editor()
{
    std::ofstream file;
    file.open(filename);
    if(!file.is_open())
        return;
    for(auto& line: lines)
        file << line.text << '\n';
}

float Editor::getCursorAdvanceX()
{
    auto val = margin.x;
    const auto& text = lines[cursorPos.y].text;
    for(int i = 0; i < cursorPos.x; ++i)
        val += font->get_glyph(text[i]).advance;
    return val;
}

void Editor::start()
{
    size.x = io.w;
    size.y = io.h;
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

        if(key == SDLK_ESCAPE && mode != Mode::move)
        {
            mode = Mode::move;
            if(cursorPos.x > 0)
                --cursorPos.x;
            lastLineX = cursorPos.x;

        }
        // static_cast<int>() to avoid overflow
        else if(mode == Mode::move && !upper)
        {
            if(key == SDLK_h)
            {
                if(cursorPos.x > 0)
                {
                    --cursorPos.x;
                    lastLineX = cursorPos.x;
                }

            }
            else if(key == SDLK_j)
            {
                if(cursorPos.y < lines.size() - 1)
                    ++cursorPos.y;

                cursorPos.x = lastLineX;
                const auto& text = lines[cursorPos.y].text;
                if(cursorPos.x > static_cast<int>(text.size()) - 1)
                {
                    if(text.size())
                        cursorPos.x = text.size() - 1;
                    else
                        cursorPos.x = 0;
                }
            }
            else if(key == SDLK_k)
            {
                if(cursorPos.y > 0)
                    --cursorPos.y;

                cursorPos.x = lastLineX;
                const auto& text = lines[cursorPos.y].text;
                if(cursorPos.x > static_cast<int>(text.size()) - 1)
                {
                    if(text.size())
                        cursorPos.x = text.size() - 1;
                    else
                        cursorPos.x = 0;
                }
            }
            else if(key == SDLK_l)
            {
                if(cursorPos.x < static_cast<int>(lines[cursorPos.y].text.size()) - 1)
                {
                    ++cursorPos.x;
                    lastLineX = cursorPos.x;
                }
            }
            else if(key == SDLK_a)
            {
                mode = Mode::insert;
                if(lines[cursorPos.y].text.size())
                    ++cursorPos.x;
            }
            else if(key == SDLK_i)
            {
                mode = Mode::insert;
            }
        }
        else if(mode == Mode::insert)
        {
            if(key == SDLK_RETURN)
                addNewLine();

            else if(key == SDLK_BACKSPACE)
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

            else if(key == ',') upper ? add('<') : add(',');
            else if(key == '.') upper ? add('>') : add('.');
            else if(key == '/') upper ? add('?') : add('/');
            else if(key == ';') upper ? add(':') : add(';');
            else if(key == '\'')upper ? add('\"') : add('\'');
            else if(key == '[') upper ? add('{') : add('[');
            else if(key == ']') upper ? add('}') : add(']');
            else if(key == '\\')upper ? add('|') : add('\\');
            else if(key == '`') upper ? add('~') : add('`');
            else if(key == '1') upper ? add('!') : add('1');
            else if(key == '2') upper ? add('@') : add('2');
            else if(key == '3') upper ? add('#') : add('3');
            else if(key == '4') upper ? add('$') : add('4');
            else if(key == '5') upper ? add('%') : add('5');
            else if(key == '6') upper ? add('^') : add('6');
            else if(key == '7') upper ? add('&') : add('7');
            else if(key == '8') upper ? add('*') : add('8');
            else if(key == '9') upper ? add('(') : add('9');
            else if(key == '0') upper ? add(')') : add('0');
            else if(key == '-') upper ? add('_') : add('-');
            else if(key == '=') upper ? add('+') : add('=');
        }
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
    // line numbers
    float addXMargin = 2.f * font->get_glyph(' ').advance;
    {
        Text text(*font);
        text.pos = margin;
        for(int i = 0; i < lines.size(); ++i)
        {
            text.text.append(std::to_string(i));
            text.text.push_back('\n');
        }
        renderer2d.render(text);
        addXMargin += text.get_size().x;
    }
    // cursor
    if(visible)
    {
        auto proto = font->get_glyph('W');
        Sprite sprite;
        sprite.pos = glm::vec2(getCursorAdvanceX() + proto.bearing.x + addXMargin,
                               margin.y + cursorPos.y * font->get_linespace() - proto.bearing.y);
        sprite.size = glm::vec2(proto.tex_coords.z, font->get_ascent() - font->get_descent());
        sprite.color = glm::vec4(1.f, 0.5f, 0.f, 1.f);
        renderer2d.render(sprite);
    }
    // actual text
    for(auto& line: lines)
    {
        line.pos.x = addXMargin + margin.x;
        renderer2d.render(line);
    }
    // insert info
    if(mode == Mode::insert)
    {
        Text text(*font);
        text.pos = glm::vec2(0.f, size.y - font->get_linespace());
        text.text = " -- INSERT MODE --";
        renderer2d.render(text);
    }
    renderer2d.flush();
}
