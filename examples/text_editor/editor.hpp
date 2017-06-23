#pragma once

#include <NGAME/scene.hpp>
#include <memory>
#include <vector>
class Font;
class Text;
#include <glm/vec4.hpp>

// I should move OpenGL context to other thread so
// I cound run SDL event loop without waiting
// (now I can do this too, but it is burning the gpu)

// todo: change allocation to more efficient for text buffer
// after implementing inserts change input processing to more flexible
// *** split into functions?
// create swp file on 'hard' exit
// opening editor without file specified
// render only visible lines
// calculate diffrently lineNumbers text size
// projection follow cursor
class Editor: public Scene
{
public:
    Editor(const char* filename);
    ~Editor();

    void start() override;
    void process_input() override;
    void render() override;

private:
    enum class Mode
    {
        insert,
        move
    } mode = Mode::move;

    float accumulator = 0.f;
    const float blinkTime = 0.6f;
    bool visible = true;

    glm::vec2 margin;

    std::unique_ptr<Font> font;
    const glm::vec4 textColor{1.f, 1.f, 0.f, 1.f};
    std::vector<Text> lines;

    glm::ivec2 cursorPos{0};
    int lastLineX = 0;

    void add(int code);
    void addNewLine();
    void add(std::string str);
    void erase();

    // includes margin
    float getCursorAdvanceX();

    std::string filename;
};
