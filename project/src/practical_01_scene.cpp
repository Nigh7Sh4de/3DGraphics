#include "../include/ShaderProgram.hpp"
#include "../include/Viewer.hpp"

#include "../include/FrameRenderable.hpp"
#include "../teachers/CubeRenderable.hpp"
#include "../teachers/IndexedCubeRenderable.hpp"


void initialize_practical_01_scene(Viewer& viewer)
{
    // create all shaders of this scene, then add them to the viewer
    ShaderProgramPtr defaultShader
        = std::make_shared<ShaderProgram>("../shaders/defaultVertex.glsl",
                                          "../shaders/defaultFragment.glsl");
    ShaderProgramPtr flatShader
        = std::make_shared<ShaderProgram>("../shaders/flatVertex.glsl",
                                          "../shaders/flatFragment.glsl");
    viewer.addShaderProgram(defaultShader);
    viewer.addShaderProgram(flatShader);

    // create renderable objects
    viewer.addRenderable(std::make_shared<FrameRenderable>(defaultShader));

    std::shared_ptr<teachers::CubeRenderable> teachersCube
        = std::make_shared<teachers::CubeRenderable>(flatShader);
    teachersCube->setModelMatrix(glm::translate(glm::mat4(), glm::vec3(2.0, 0.0, 0.0)));
    viewer.addRenderable(teachersCube);

    std::shared_ptr<teachers::IndexedCubeRenderable> teachersIndexedCube
        = std::make_shared<teachers::IndexedCubeRenderable>(flatShader);
    teachersIndexedCube->setModelMatrix(glm::translate(glm::mat4(), glm::vec3(-2.0, 0.0, 0.0)));
    viewer.addRenderable(teachersIndexedCube);

    // MeshRenderablePtr mesh = std::make_shared<MeshRenderable>(pointLightShader, "./../meshes/suzanne.obj");
    // mesh->setModelMatrix( glm::translate(glm::mat4(1.0), glm::vec3(-6,2,0)) );
    // viewer.addRenderable(mesh);
}
