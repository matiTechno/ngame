#include <NGAME/scene.hpp>
#include <NGAME/app.hpp>

Scene::Scene():
   io(App::handle->io),
   renderer2d(*App::handle->renderer2d),
   pp_unit(*App::handle->pp_unit),
   pos(0, 0),
   size(io.w, io.h),
   should_close(App::handle->should_close)
{}
