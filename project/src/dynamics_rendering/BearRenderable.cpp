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


void BearRenderable::do_keyPressedEvent(sf::Event &e)
{
  std::cout<<"Got event"<<std::endl;

  // setModelMatrix( glm::translate(glm::mat4(1.0), glm::vec3(0.0, -1.0, 0.0)));

  // setParentTransform( glm::translate(glm::mat4(1.0), glm::vec3(0.0, -1.0, 0.0)) );

  glm::mat4 mat = glm::mat4(0.0);
  mat[1].y = 1.0;
  setLocalTransform( getModelMatrix() + mat);

}
