#pragma once
#include <NGAME/scene.hpp>
class Font;
class Shader;

class Menu: public Scene
{
public:
    Menu(const Font& font, Shader& shader);

    void process_input() override;

    void render() override;

private:
    const Font& font;
    Shader& shader;
    float time = 0.f;
};
