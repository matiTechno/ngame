#pragma once

#include <NGAME/scene.hpp>
#include <fstream>
#include <memory>
#include <vector>
class Font;
class Text;

class Editor: public Scene
{
public:
    Editor(const char* filename);
    ~Editor();

    void start() override;
    void process_input() override;
    void render() override;

private:
    float accumulator = 0.f;
    const float blinkTime = 0.6f;
    bool visible = true;
    std::unique_ptr<Font> font;
    std::unique_ptr<Text> text;
    glm::vec2 penPos;
    std::vector<glm::vec2> lineEndings;

    void add(int code);
    void add(std::string str);
    void erase();
    std::fstream file;
};
