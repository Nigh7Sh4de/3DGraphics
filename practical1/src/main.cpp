#include "../include/Viewer.hpp"
#include "../include/log.hpp"
#include "./../include/ShaderProgram.hpp"
#include "./../include/FrameRenderable.hpp"
#include "./../include/CubeRenderable.hpp"
#include "./../include/IndexedCubeRenderable.hpp"

#include <iostream>

static void initialize_scene (Viewer& viewer)
{
  // create your renderables and shader programs
  // and add them to the viewer

  // Shader program instanciation
  // Path to the vertex shader glsl code
  std::string vShader = "./../shaders/defaultVertex.glsl";
  // Path to the fragment shader glsl code
  std::string fShader = "./../shaders/defaultFragment.glsl";
  // Compile and link the shaders into a program
  ShaderProgramPtr defaultShader = std::make_shared<ShaderProgram>(vShader, fShader);
  // Add the shader program to the Viewer
  viewer.addShaderProgram(defaultShader);

  //Compile and link the flat shaders into a shader program
  vShader = "./../shaders/flatVertex.glsl";
  fShader = "./../shaders/flatFragment.glsl";
  ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>(vShader, fShader);
  viewer.addShaderProgram(flatShader);

  // When instanciating a renderable,
  // you must specify the shader program used to draw it.
  FrameRenderablePtr frame = std::make_shared<FrameRenderable>(defaultShader);
  viewer.addRenderable(frame);

  //Instanciate a CubeRenderable while specifying its shader program
  CubeRenderablePtr cube = std::make_shared<CubeRenderable>(flatShader);
  viewer.addRenderable(cube);

  //Instanciate an IndexedCubeRenderable while specifying its shader program
  IndexedCubeRenderablePtr indexedCube = std::make_shared<IndexedCubeRenderable>(flatShader);
  viewer.addRenderable(indexedCube);
}

int main()
{
  int width = 1280; // Width of the window (pixel)
  int height = 720; // Height of the window (pixel)
  Viewer viewer(width, height);

  initialize_scene(viewer);

  // Display the window and its associated framebuffer
  while (viewer.isRunning()) {
    viewer.handleEvent();
    viewer.draw();
    viewer.display();
  }

  return EXIT_SUCCESS;
}
