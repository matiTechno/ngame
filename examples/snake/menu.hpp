#pragma once

#include <NGAME/scene.hpp>
class Font;
class Snake;
#include <NGAME/text.hpp>
#include <vector>

class Menu: public Scene
{
public:
    Menu(Snake& snake, bool paused, int score, const Font& font);

    void set_coords() override;

    void process_input() override;

    void render() override;

private:
    Snake& snake;
    const Font& font;
    bool paused;
    static constexpr int beg_option = 2;
    int option = beg_option;
    std::vector<Text> texts;
    float time = 0.f;
};
