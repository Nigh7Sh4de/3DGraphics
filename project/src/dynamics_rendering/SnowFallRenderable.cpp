#include "../../include/dynamics_rendering/SnowFallRenderable.hpp"
#include "../../include/gl_helper.hpp"
#include "../../include/log.hpp"

// #include <glm/gtc/matrix_scaleform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <iostream>


SnowFallRenderable::~SnowFallRenderable()
{
}

SnowFallRenderable::SnowFallRenderable(ShaderProgramPtr shaderProgram, DynamicSystemPtr &system, DynamicSystemRenderablePtr &systemRenderable, ConstantForceFieldPtr &gravityForceField) :
    HierarchicalRenderable(shaderProgram), shader(shaderProgram), system(system), systemRenderable(systemRenderable), gravityForceField(gravityForceField)
{
}

void SnowFallRenderable::spawnSnow() {
    // puck particle
    float x = ((std::rand() % 1000) - 500.0) / 10.0;
    float y = ((std::rand() % 1000) - 500.0) / 10.0;
    glm::vec3 px = glm::vec3(x, y, 20); //position
    glm::vec3 pv = glm::vec3(0.0, 0.0, 0.0); //velocity
    float pm = 10.0; //mass
    float pr = 0.1; //radius
    ParticlePtr snowParticle = std::make_shared<Particle>(px, pv, pm, pr);
    system->addParticle(snowParticle);

    //Create a particleRenderable for particle of the system
    PuckRenderablePtr puckRenderable = std::make_shared<PuckRenderable>(shader, snowParticle, true, 30, glm::vec4(0.9));
    HierarchicalRenderable::addChild(systemRenderable, puckRenderable);
    gravityForceField->setParticles(system->getParticles());

    timers.push_back(0);
    snowflakes.push_back(puckRenderable);
}

void SnowFallRenderable::do_draw()
{
}

void SnowFallRenderable::do_animate(float dTime, float time)
{
  for (int i=0;i<snowflakes.size();i++) {
    timers[i] += dTime;
    if (timers[i] > duration) {
      PuckRenderablePtr s = snowflakes[i];
      ParticlePtr p = s->getParticle();
      systemRenderable->removeChild(s);
      system->removeParticle(p);
      gravityForceField->setParticles(system->getParticles());
      snowflakes.erase(snowflakes.begin()+i);
      timers.erase(timers.begin()+i);
    }
  }

  next += time;
  if (next > frequency) {
    next = 0;
    spawnSnow();
  }
}
