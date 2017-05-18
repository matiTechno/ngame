#include <NGAME/app.hpp>
#include <iostream>
#include <NGAME/imgui.h>
#include <SDL2/SDL_mixer.h>
#include <NGAME/sound.hpp>
#include <NGAME/gl/shader.hpp>
#include <NGAME/sprite.hpp>
#include <NGAME/renderer2d.hpp>
#include <NGAME/pp_unit.hpp>
class Test: public Scene
{
public:
    Test():
        sample("res/laser1.mp3"),
        shader("res/shader1.sh", true),
        sh_pp("res/pp_unit.sh", true),
        texture("res/example.png")
    {}

    void process_input() override
    {
        if(io.imgui_wants_input)
            return;
        for(auto& event: io.events)
        {
            if(event.type == SDL_KEYDOWN && !event.key.repeat && event.key.keysym.sym == SDLK_p)
                sample.play();
            else if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
                exit();
        }
    }
    void set_coords() override
    {
        size.x = io.w;
        size.y = io.h;
    }
    void render() override
    {
        shader.bind();
        pp_unit.start();

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
        {
            Sprite sprite;
            sprite.pos = glm::vec2(0.f, 0.f);
            sprite.size = glm::vec2(io.w, io.h);
            renderer2d.render(sprite);
        }

        renderer2d.render(sprite);

        renderer2d.flush(glm::vec2(0.f, 0.f), size);

        time += io.frametime;
        sh_pp.bind();
        glUniform1f(sh_pp.get_uni_location("time"), time);

        pp_unit.render(true, sh_pp);
    }

private:
    Sample sample;
    bool vsync = true;
    Shader shader, sh_pp;
    Texture texture;
    float time = 0;
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
