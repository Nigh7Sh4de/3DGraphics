#ifndef BEAR_RENDERABLE_HPP
#define BEAR_RENDERABLE_HPP

#include "../../teachers/MeshRenderable.hpp"
#include "../dynamics/Plane.hpp"

#include <vector>
#include <glm/glm.hpp>

/**@brief Render terrain on the screen. */
class BearRenderable : public teachers::MeshRenderable
{
public:
    ~BearRenderable();
    BearRenderable(ShaderProgramPtr program);

private:
    void do_keyPressedEvent(sf::Event &e);
    void do_keyReleasedEvent(sf::Event &e);
    void do_animate(float dTime, float time);

    float speed = 200.0;
    bool forwards = false;
    bool backwards = false;
    bool left = false;
    bool right = false;
};

typedef std::shared_ptr<BearRenderable> BearRenderablePtr;

#endif //PLANE_RENDERABLE_HPP
