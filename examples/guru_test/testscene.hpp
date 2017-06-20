#pragma once
#include <NGAME/guruscene.hpp>

class TestScene: public guru::GuruScene
{
public:
    TestScene();

private:
    void worldRender() override;
};
