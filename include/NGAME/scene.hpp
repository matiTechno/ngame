#pragma once
#include <glm/vec2.hpp>
#include <memory>
class Renderer2d;
struct IO;
class PP_unit;
class Font_loader;
class Renderer3d;

// virtual functions are executed in the same order as presented
// data in IO structure is ready to use in all functions
class Scene {
public:
    Scene();
    virtual ~Scene() = default;

    // always
    virtual void start()
    {}
    // only when top
    virtual void process_input()
    {}
    // if top or update_when_not_top
    virtual void update()
    {}
    // always
    virtual void set_coords()
    {}
    // only if scenes on top are not opaque
    virtual void render()
    {}

protected:
    const IO& io;
    const Renderer2d& renderer2d;
    const PP_unit& pp_unit;
    const Font_loader& font_loader;
    const Renderer3d& renderer3d;
    glm::ivec2 pos;
    glm::ivec2 size;

    bool update_when_not_top = false;
    bool is_opaque = true;
    int scenes_to_pop = 0;

    template<typename T, typename ...Args>
    void set_new_scene(Args&&...args)
    {
        new_scene = std::make_unique<T>(std::forward<Args>(args)...);
    }

    bool is_top() const
    {return is_top_b;}

    void quit()
    {
        should_close = true;
    }

    // pos.y in opengl coordinates
    // useful when sending pos
    // to postprocessing shader
    int get_gl_y() const;

private:
    friend class App;

    bool is_top_b;
    bool& should_close;

    std::unique_ptr<Scene> new_scene;
};
