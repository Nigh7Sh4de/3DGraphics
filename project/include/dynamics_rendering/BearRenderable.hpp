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
};

typedef std::shared_ptr<BearRenderable> BearRenderablePtr;

#endif //PLANE_RENDERABLE_HPP
