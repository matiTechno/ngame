#pragma once
class Del;
#include <memory>
#include <vector>
#include "scene.hpp"
#include <SDL2/SDL.h>
struct SDL_Window;
#include <cassert>
class Renderer2d;

struct IO
{
    std::vector<SDL_Event> events;
    float frametime;
    float av_frametime;
    bool imgui_wants_input;
    int w, h;
};

class App
{
    public:
        App(int width, int height, const char* title, unsigned int sdl_flags = 0, int mixer_flags = 0,
            int posx = SDL_WINDOWPOS_CENTERED, int posy = SDL_WINDOWPOS_CENTERED);

        ~App();

        template<typename T, typename ...Args>
            void start(Args... args)
            {
                assert(!should_close);

                scenes.push_back(std::make_unique<T>(std::forward<Args>(args)...));
                run();
            }

    private:
           friend class Scene;
           static App* handle;

        std::unique_ptr<Del> del_sdl;
        std::unique_ptr<Del> del_win;
        std::unique_ptr<Del> del_context;
        std::unique_ptr<Del> del_imgui;
        std::unique_ptr<Del> del_mix_init;
        std::unique_ptr<Del> del_mix_audio;
        IO io;
        std::unique_ptr<Renderer2d> renderer2d;
        std::vector<std::unique_ptr<Scene>> scenes;
        SDL_Window* win;
        bool should_close;

        void init_mixer(int mixer_flags);

        void run();
        void update();
        void process_input();
        void render();
        void manage_scenes();
};
