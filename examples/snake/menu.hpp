#pragma once

#include <NGAME/scene.hpp>
#include <string>
class Font;

class Menu: public Scene
{
public:
    Menu(bool paused, int score, const Font& font);

    void process_input() override;

    void render() override;

private:
    const Font& font;
};
