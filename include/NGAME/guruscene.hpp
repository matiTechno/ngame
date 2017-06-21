#pragma once
#include "scene.hpp"

namespace guru
{

class Guru;

class Vertex;

// works as Vspace but with less features
// sets projection for Renderer2d and Guru
class GuruScene: public Scene
{
public:
    GuruScene(glm::vec2 start, glm::vec2 range);

    void start() override;

    void render() override;

protected:
    Guru& guru;

    glm::vec2 getWorldStart() const {return worldStart;}
    glm::vec2 getWorldRange() const {return worldRange;}
    glm::vec2 getProjStart() const {return projStart;}
    glm::vec2 getProjRange() const {return projRange;}

private:
    glm::vec2 worldStart, worldRange;
    glm::vec2 projStart, projRange;

    // override these
    virtual void start2();
    virtual void worldRender();

    void setProjection();
};

} // guru
