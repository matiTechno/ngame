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

Editor::~Editor()
{
    std::ofstream file;
    file.open(filename);
    if(!file.is_open())
        return;
    for(auto& line: lines)
        file << line.text << '\n';
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

        if(key == SDLK_ESCAPE)
        {
            if(mode == Mode::move)
                inputMoveBuffer.clear();
            else
            {
                mode = Mode::move;
                if(cursorPos.x > 0)
                    --cursorPos.x;
                lastLineX = cursorPos.x;
            }
            continue;
        }
        if(upper)
        {
            if(key >= 'a' && key <= 'z') key -= 32;
            else if(key == ',') key = '<';
            else if(key == '.') key = '>';
            else if(key == '/') key = '?';
            else if(key == ';') key = ':';
            else if(key == '\'')key = '\"';
            else if(key == '[') key = '{';
            else if(key == ']') key = '}';
            else if(key == '\\')key = '|';
            else if(key == '`') key = '~';
            else if(key == '1') key = '!';
            else if(key == '2') key = '@';
            else if(key == '3') key = '#';
            else if(key == '4') key = '$';
            else if(key == '5') key = '%';
            else if(key == '6') key = '^';
            else if(key == '7') key = '&';
            else if(key == '8') key = '*';
            else if(key == '9') key = '(';
            else if(key == '0') key = ')';
            else if(key == '-') key = '_';
            else if(key == '=') key = '+';
        }
        if(mode == Mode::insert)
            processInputInsert(key);
        else if(mode == Mode::move)
            processInputMove(key);
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

float Editor::getCursorAdvanceX()
{
    auto val = margin.x;
    const auto& text = lines[cursorPos.y].text;
    for(int i = 0; i < cursorPos.x; ++i)
        val += font->get_glyph(text[i]).advance;
    return val;
}

void Editor::processInputInsert(int key)
{
    switch(key)
    {
    case SDLK_RETURN: addNewLine(); break;
    case SDLK_BACKSPACE: erase(); break;
    case SDLK_TAB: add("    "); break;
    default: if(key >= 32 && key <= 126) add(key);
    }
}

// static_cast<int>() to avoid overflow
void Editor::processInputMove(int key)
{
    inputMoveBuffer.push_back(key);

    if(inputMoveBuffer == "h")
    {
        if(cursorPos.x > 0)
        {
            --cursorPos.x;
            lastLineX = cursorPos.x;
        }
        inputMoveBuffer.clear();
    }
    else if(inputMoveBuffer == "j")
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
        inputMoveBuffer.clear();
    }
    else if(inputMoveBuffer == "k")
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
        inputMoveBuffer.clear();
    }
    else if(inputMoveBuffer == "l")
    {
        if(cursorPos.x < static_cast<int>(lines[cursorPos.y].text.size()) - 1)
        {
            ++cursorPos.x;
            lastLineX = cursorPos.x;
        }
        inputMoveBuffer.clear();
    }
    else if(inputMoveBuffer == "a")
    {
        mode = Mode::insert;
        if(lines[cursorPos.y].text.size())
            ++cursorPos.x;
        inputMoveBuffer.clear();
    }
    else if(inputMoveBuffer == "i")
    {
        mode = Mode::insert;
        inputMoveBuffer.clear();
    }
    else if(inputMoveBuffer == "I")
    {
        mode = Mode::insert;
        cursorPos.x = 0;
        inputMoveBuffer.clear();
    }
    else if(inputMoveBuffer == "A")
    {
        mode = Mode::insert;
        cursorPos.x = lines[cursorPos.y].text.size();
        inputMoveBuffer.clear();
    }
    // hack
    else if(inputMoveBuffer == "gg" || inputMoveBuffer == "g")
    {
        if(inputMoveBuffer != "gg")
            return;
        cursorPos = glm::ivec2(0);
        lastLineX = 0;
        inputMoveBuffer.clear();
    }
    else if(inputMoveBuffer == "G")
    {
        cursorPos.y = lines.size() - 1;
        cursorPos.x = lines[cursorPos.y].text.size() - 1;
        lastLineX = cursorPos.x;
        inputMoveBuffer.clear();
    }
    else if(inputMoveBuffer == "o")
    {
        mode = Mode::insert;
        cursorPos.x = 0;
        addNewLine();
        std::swap(lines[cursorPos.y].text, lines[cursorPos.y - 1].text);
        inputMoveBuffer.clear();
    }
    else if(inputMoveBuffer == "O")
    {
        mode = Mode::insert;
        cursorPos.x = 0;
        addNewLine();
        --cursorPos.y;
        inputMoveBuffer.clear();
    }
    else
        inputMoveBuffer.clear();
}
