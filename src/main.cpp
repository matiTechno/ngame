#include <NGAME/app.hpp>
#include <iostream>
#include <NGAME/imgui.h>
#include <SDL2/SDL_mixer.h>
#include <NGAME/sound.hpp>
#include <NGAME/gl/shader.hpp>
#include <NGAME/sprite.hpp>
#include <NGAME/renderer2d.hpp>
class Test: public Scene
{
public:
    Test():
        sample("laser1.mp3"),
        shader("shader1.sh", true),
        texture("example.png")
    {}

    void process_input() override
    {
        size.x = io.w;
        size.y = io.h;
        if(io.imgui_wants_input)
            return;
        for(auto& event: io.events)
            if(event.type == SDL_KEYDOWN && !event.key.repeat && event.key.keysym.sym == SDLK_p)
                sample.play();
    }
    void render() override
    {
        shader.bind();

        ImGui::ShowTestWindow();

        ImGui::Begin("control");
        ImGui::Text("frametime(ms): %.3f", io.av_frametime * 1000.f);
        ImGui::Separator();
        ImGui::Spacing();
        if(ImGui::Button("vsync on / off"))
        {
            vsync = !vsync;
            SDL_GL_SetSwapInterval(vsync);
        }
        ImGui::End();

        Sprite sprite;
        sprite.pos = glm::vec2(20.f, 20.f);
        sprite.size = glm::vec2(100.f);
        sprite.texture = &texture;
        sprite.tex_coords = glm::ivec4(0, 0, texture.get_size());

        renderer2d.render(sprite);
        renderer2d.flush(glm::vec2(0.f, 0.f), size);
    }

private:
    Sample sample;
    bool vsync = true;
    Shader shader;
    Texture texture;
};

int main() {
    try
    {
  App app(640, 480, "test", SDL_WINDOW_MAXIMIZED, MIX_INIT_MP3);
  app.start<Test>();
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }

  return 0;
}
