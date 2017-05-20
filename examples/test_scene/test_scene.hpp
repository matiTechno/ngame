#pragma once
#include <NGAME/scene.hpp>
#include <NGAME/sound.hpp>
#include <NGAME/gl/shader.hpp>
#include <NGAME/gl/texture.hpp>
#include <NGAME/font.hpp>

class Test_scene: public Scene
{
public:
    Test_scene();

    void process_input() override;

    void set_coords() override;

    void render() override;

private:
    Sample sample;
    bool vsync = true;
    Shader shader;
    Texture texture;
    float time = 0.f;
    Font font;
};
