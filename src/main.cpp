#include <NGAME/app.hpp>

#include <NGAME/imgui.h>
class Test: public Scene
{
public:
    void render() override
    {
        ImGui::ShowTestWindow();
    }
};

int main() {
  App app(640, 480, "test", SDL_WINDOW_MAXIMIZED);
  app.start<Test>();
  return 0;
}
