#include <NGAME/scene.hpp>
#include <NGAME/app.hpp>

Scene::Scene():
   io(App::handle->io),
   pos(0, 0),
   size(io.w, io.h),
   should_close(App::handle->should_close)
{}
