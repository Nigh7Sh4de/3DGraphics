#include "../../include/dynamics_rendering/PuckRenderable.hpp"
#include "../../include/gl_helper.hpp"
#include "../../include/log.hpp"
#include "../../include/Utils.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

PuckRenderable::~PuckRenderable() {}

PuckRenderable::PuckRenderable(ShaderProgramPtr shaderProgram, ParticlePtr particle,
                               bool normalPerVertex, unsigned int nbSlices, glm::vec4 color)
    : CylinderRenderable(shaderProgram, normalPerVertex, nbSlices, color),
      m_particle(particle) {}

void PuckRenderable::do_draw()
{
    //Update the parent and local transform matrix to position the geometric data according to the particle's data.
    const float &pRadius = m_particle->getRadius();
    const glm::vec3 &pPosition = m_particle->getPosition();
    glm::mat4 scale = glm::scale(glm::mat4(1.0), glm::vec3(pRadius));
    glm::mat4 translate = glm::translate(glm::mat4(1.0), glm::vec3(pPosition));
    setLocalTransform(translate * scale);

    CylinderRenderable::do_draw();
}

void PuckRenderable::do_animate(float time)
{
    if (time < 2) {
        m_particle->restart();
    }
}
