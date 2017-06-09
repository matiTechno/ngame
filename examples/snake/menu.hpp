#pragma once

#include <NGAME/scene.hpp>
#include <string>

class Menu: public Scene
{
public:
    Menu(const std::string& title, int score, bool end);

    void process_input() override;

    void render() override;

private:
    std::string title;
    std::string str_score;
    bool end;
};
