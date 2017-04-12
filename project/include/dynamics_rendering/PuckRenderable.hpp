#ifndef PUCK_RENDERABLE_HPP
#define PUCK_RENDERABLE_HPP

#include "../dynamics/Particle.hpp"
#include "../../teachers/CylinderRenderable.hpp"

#include <vector>
#include <glm/glm.hpp>

/**@brief Render a puck to the screen.
 *
 * Render a puck to the screen. Since a puck is modeled by
 * a cylinder, this renderable renders the corresponding CylinderRenderable.
 */
class PuckRenderable : public teachers::CylinderRenderable
{
  public:
    /**@brief Build a particle renderable.
         *
         * Build a renderable to render a particle.
         * @param program The shader program used to render the particle.
         * @param particle The particle to render.
         */
    PuckRenderable(ShaderProgramPtr program, ParticlePtr particle,
                   bool normalPerVertex = true, unsigned int nbSlices = 20, glm::vec4 color = glm::vec4(-1.0));

    ~PuckRenderable();

    ParticlePtr getParticle();

  private:
    void do_draw();
    void do_animate(float dTime, float time);

    ParticlePtr m_particle;
    float animTime;
};

typedef std::shared_ptr<PuckRenderable> PuckRenderablePtr;

#endif //PUCK_RENDERABLE_HPP
