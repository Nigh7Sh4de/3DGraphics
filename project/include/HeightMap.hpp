#ifndef HEIGHT_MAP_HPP
#define HEIGHT_MAP_HPP

// #include "../dynamics/Particle.hpp"
// #include "../../teachers/CylinderRenderable.hpp"

#include <vector>
#include <glm/glm.hpp>

/**@brief Load an image as a height map
 *
 */
class HeightMap //: public teachers::CylinderRenderable
{
  public:
    /**@brief Build a particle renderable.
         *
         * Build a renderable to render a particle.
         * @param program The shader program used to render the particle.
         * @param particle The particle to render.
         */
    HeightMap(ShaderProgramPtr program, );

    ~HeightMap();

  private:
    void do_draw();
    void do_animate(float time);

    ParticlePtr m_particle;
    float animTime;
};

typedef std::shared_ptr<HeightMap> HeightMapPtr;

#endif //PUCK_RENDERABLE_HPP
