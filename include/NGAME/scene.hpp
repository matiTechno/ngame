#pragma once
#include <glm/vec2.hpp>
#include <memory>
class Renderer2d;
struct IO;
class PP_unit;

class Scene {
public:
  Scene();
  virtual ~Scene() = default;

  virtual void process_input()
  {}
  virtual void update()
  {}
  virtual void set_coords()
  {}
  virtual void render()
  {}

protected:
  const IO& io;
  const Renderer2d& renderer2d;
  const PP_unit& pp_unit;
  glm::ivec2 pos;
  glm::ivec2 size;

  bool update_when_not_top = false;
  bool is_opaque = true;
  int scenes_to_pop = 0;

  template<typename T, typename ...Args>
  void set_new_scene(Args...args)
  {
      new_scene = std::make_unique<T>(std::forward<Args>(args)...);
  }

  bool is_top() const
  {return is_top_b;}

  void exit()
  {
      should_close = true;
  }

private:
  friend class App;

  bool is_top_b;
  bool& should_close;

  std::unique_ptr<Scene> new_scene;
};
