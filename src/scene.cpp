#include <NGAME/scene.hpp>
#include <NGAME/app.hpp>

Scene::Scene():
    io(App::handle->io),
    renderer2d(*App::handle->renderer2d),
    pp_unit(*App::handle->pp_unit),
    font_loader(*App::handle->font_loader),
    renderer3d(*App::handle->renderer3d),
    pos(0, 0),
    size(io.w, io.h),
    should_close(App::handle->should_close)
{}

int Scene::get_gl_y() const
{
    return io.h - (pos.y + size.y);
}
