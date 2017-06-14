#include "test_scene.hpp"
#include <NGAME/3d/renderer3d.hpp>
#include <NGAME/3d/camera.hpp>

Test_scene::Test_scene():
    model("monkey.obj")
{
    Camera camera(size.x, size.y);
    renderer3d.set_camera(camera);
    inst1.model = &model;
    inst1.color = glm::vec3(1.f, 0.2f, 0.f);
}

void Test_scene::render()
{
    renderer3d.render(inst1);
}
