#include "../../include/dynamics_rendering/BearRenderable.hpp"
#include "../../include/gl_helper.hpp"
#include "../../include/log.hpp"

// #include <glm/gtc/matrix_scaleform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <iostream>


BearRenderable::~BearRenderable()
{
}

BearRenderable::BearRenderable(ShaderProgramPtr shaderProgram) :
    teachers::MeshRenderable(shaderProgram, "../meshes/bear.obj")
{

}


void BearRenderable::do_animate(float dTime, float time)
{
    if (left || right || forwards || backwards)
      for (int i=0;i<m_positions.size();i++) {
        if (left)
          m_positions[i].x += dTime * speed;
        if (right)
          m_positions[i].x -= dTime * speed;
        if (backwards)
          m_positions[i].z -= dTime * speed;
        if (forwards)
          m_positions[i].z += dTime * speed;
      }

    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_positions.size()*sizeof(glm::vec3), m_positions.data(), GL_STATIC_DRAW));

}


void BearRenderable::do_keyReleasedEvent(sf::Event &e)
{
  switch (e.key.code)
  {
    case sf::Keyboard::Left:
      left = false;
      break;
    case sf::Keyboard::Right:
      right = false;
      break;
    case sf::Keyboard::Up:
      forwards = false;
      break;
    case sf::Keyboard::Down:
      backwards = false;
      break;
  }
}


void BearRenderable::do_keyPressedEvent(sf::Event &e)
{
  switch (e.key.code)
  {
    case sf::Keyboard::Left:
      left = true;
      break;
    case sf::Keyboard::Right:
      right = true;
      break;
    case sf::Keyboard::Up:
      forwards = true;
      break;
    case sf::Keyboard::Down:
      backwards = true;
      break;
  }
}
