#include <NGAME/app.hpp>
#include <NGAME/glad.h>
#include <NGAME/imgui.h>
#include "imgui_impl_sdl_gl3.h"
#include <chrono>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <NGAME/renderer2d.hpp>
#include <NGAME/pp_unit.hpp>
#include <NGAME/scene.hpp>
#include <NGAME/del.hpp>
#include <NGAME/font_loader.hpp>

const App* App::handle = nullptr;

App::~App() = default;

App::App(int width, int height, const char *title, unsigned int sdl_flags, int mixer_flags, int posx, int posy)
{
    assert(!handle);
    handle = this;

    SDL_version ver_compiled, ver_linked;
    SDL_VERSION(&ver_compiled);
    SDL_GetVersion(&ver_linked);

    std::cout << "compiled against SDL " << int(ver_compiled.major) << '.'
              << int(ver_compiled.minor) << '.' << int(ver_compiled.patch)
              << std::endl;
    std::cout << "linked against SDL " << int(ver_linked.major) << '.'
              << int(ver_linked.minor) << '.' << int(ver_linked.patch)
              << std::endl;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
        throw std::runtime_error(std::string("SDL_Init failed: ") + SDL_GetError());

    del_sdl = std::make_unique<Del>([]() { SDL_Quit(); });

    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS,
                        SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);

    win = SDL_CreateWindow(title, posx, posy, width, height,
                           SDL_WINDOW_OPENGL | sdl_flags);
    if (!win)
        throw std::runtime_error(std::string("SDL_CreateWindow failed: ") +
                                 SDL_GetError());

    auto temp_win = win;

    del_win =
            std::make_unique<Del>([temp_win]() { SDL_DestroyWindow(temp_win); });

    auto context = SDL_GL_CreateContext(win);
    if (!context)
        throw std::runtime_error(std::string("SDL_GL_CreateContext failed: ") +
                                 SDL_GetError());

    del_context =
            std::make_unique<Del>([context]() { SDL_GL_DeleteContext(context); });

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(SDL_GL_GetProcAddress)))
        throw std::runtime_error("gladLoadGLLoader failed");

    std::cout << "vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    SDL_GL_SetSwapInterval(1);

    ImGui_ImplSdlGL3_Init(win);
    del_imgui = std::make_unique<Del>([]() { ImGui_ImplSdlGL3_Shutdown(); });

    init_mixer(mixer_flags);

    renderer2d = std::make_unique<Renderer2d>();
    SDL_GL_GetDrawableSize(win, &io.w, &io.h);
    pp_unit = std::make_unique<PP_unit>(io.w, io.h);
    font_loader = std::make_unique<Font_loader>();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void App::run() {
    auto now = std::chrono::high_resolution_clock::now();

    while (scenes.size() && !should_close) {

        auto new_time = std::chrono::high_resolution_clock::now();
        io.frametime = std::chrono::duration<float>(new_time - now).count();
        now = new_time;

        static auto count = 0;
        ++count;
        static auto accu = 0.f;
        accu += io.frametime;
        if (accu > 0.5f) {
            io.av_frametime = accu / count;
            count = 0;
            accu = 0.f;
        }
        process_input();
        update();
        render();
        manage_scenes();
    }
}

void App::update() {
    for(auto& scene: scenes)
    {
        if(scene->update_when_not_top || scene->is_top_b)
            scene->update();
    }
}

void App::process_input() {
    io.events.clear();
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        io.events.push_back(event);

        ImGui_ImplSdlGL3_ProcessEvent(&event);
    }

    SDL_GL_GetDrawableSize(win, &io.w, &io.h);

    ImGui_ImplSdlGL3_NewFrame(win);
    io.imgui_wants_input = ImGui::GetIO().WantCaptureKeyboard && ImGui::GetIO().WantCaptureMouse;

    scenes.back()->process_input();
}

void App::render() {

    pp_unit->set_new_size(io.w, io.h);
    glClear(GL_COLOR_BUFFER_BIT);

    for(auto it = scenes.rbegin(); it != scenes.rend(); ++ it)
    {
        auto& scene = **it;

        if(scene.size.x < 0)
            scene.size.x = 0;
        if(scene.size.y < 0)
            scene.size.y = 0;

        scene.set_coords();

        glm::ivec4 scene_gl_coords(scene.pos.x, io.h - (scene.pos.y + scene.size.y), scene.size.x, scene.size.y);
        glViewport(scene_gl_coords.x, scene_gl_coords.y, scene_gl_coords.z, scene_gl_coords.w);
        pp_unit->set_scene(scene_gl_coords);

        scene.render();
        if(scene.is_opaque)
            break;
    }

    ImGui::Render();

    SDL_GL_SwapWindow(win);
}

void App::manage_scenes() {
    auto new_scene = std::move(scenes.back()->new_scene);

    auto num_to_pop = scenes.back()->scenes_to_pop;
    assert(num_to_pop >= 0 && num_to_pop <= int(scenes.size()));
    for(auto i = 0; i < num_to_pop; ++i)
        scenes.pop_back();

    if(new_scene)
        scenes.push_back(std::move(new_scene));

    if(!scenes.size())
        return;

    for(auto it = scenes.begin(); it != scenes.end() - 1; ++it)
        (*it)->is_top_b = false;

    scenes.back()->is_top_b = true;
}

void App::init_mixer(int mixer_flags)
{
    SDL_version ver_compiled;
    SDL_MIXER_VERSION(&ver_compiled);
    const SDL_version* ver_linked = Mix_Linked_Version();

    std::cout << "compiled against SDL_mixer " << int(ver_compiled.major) << '.' <<
                 int(ver_compiled.minor) << '.' << int(ver_compiled.patch) << std::endl;
    std::cout << "linked against SDL_mixer " << int(ver_linked->major) << '.' <<
                 int(ver_linked->minor) << '.' << int(ver_linked->patch) << std::endl;

    if(mixer_flags)
    {
        int initted = Mix_Init(mixer_flags);
        if(initted != mixer_flags)
            throw std::runtime_error(std::string("Mix_Init failed, mixer_flags = ") +
                                     std::to_string(mixer_flags) + ": " + Mix_GetError());

        del_mix_init = std::make_unique<Del>([](){
            Mix_Quit();
        });
    }

    if(Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
        throw std::runtime_error(std::string("Mix_OpenAudio failed: ") + Mix_GetError());

    del_mix_audio = std::make_unique<Del>([](){
        Mix_CloseAudio();
    });

    Mix_AllocateChannels(50);
}
