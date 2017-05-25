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
    IO()
    {events.reserve(20);}
    std::vector<SDL_Event> events;
    float frametime;
    float av_frametime;
    bool imgui_wants_input;
    int w, h;
    float aspect;
    SDL_Window* win;
};

// some fixed values:
// allocated mixer channels
// Renderer2d batches reserve
// Renderer2d size of instance array
// atlas width in load_font()
// scenes_to_render reserve
// IO events reserve

// important:
// to change blend function use Blend class
// to make glBlend functions visible define NGAME_BLEND
// before including NGAME/glad.h
class App
{
public:
    App(int width, int height, const char* title, bool handle_quit, int gl_major, int gl_minor,
        unsigned int sdl_flags = 0, int mixer_flags = 0,
        int posx = SDL_WINDOWPOS_CENTERED, int posy = SDL_WINDOWPOS_CENTERED);

    ~App();

    template<typename T, typename ...Args>
    void start(Args&&... args)
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
    std::vector<Scene*> scenes_to_render;
    const bool handle_quit;

    void init_mixer(int mixer_flags);

    void run();
    void update();
    void process_input();
    void render();
    void manage_scenes();
};
