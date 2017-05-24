#include "sc_master.hpp"
#include "sc_level.hpp"

const Sc_master* Sc_master::handle = nullptr;

Sc_master::Sc_master()
{
    assert(handle == nullptr);
    handle = this;
    set_new_scene<Sc_level>();
}
