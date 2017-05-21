#pragma once
class Del;
#include <memory>
#include <vector>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_events.h>
struct SDL_Window;
#include <cassert>
class Renderer2d;
class PP_unit;
class Scene;
class Font_loader;

struct IO
{
    std::vector<SDL_Event> events;
    float frametime;
    float av_frametime;
    bool imgui_wants_input;
    int w, h;
};

// some fixed values:
// allocated mixer channels = 100
// Renderer2d batches reserve = 50
// Renderer2d size of instance array = 100K
// atlas width in load_font() = 1024;
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
    static const App* handle;

    std::unique_ptr<Del> del_sdl;
    std::unique_ptr<Del> del_win;
    std::unique_ptr<Del> del_context;
    std::unique_ptr<Del> del_imgui;
    std::unique_ptr<Del> del_mix_init;
    std::unique_ptr<Del> del_mix_audio;
    IO io;
    std::unique_ptr<Renderer2d> renderer2d;
    std::unique_ptr<PP_unit> pp_unit;
    std::unique_ptr<Font_loader> font_loader;
    std::vector<std::unique_ptr<Scene>> scenes;
    SDL_Window* win;
    mutable bool should_close = false;

    void init_mixer(int mixer_flags);

    void run();
    void update();
    void process_input();
    void render();
    void manage_scenes();
};
