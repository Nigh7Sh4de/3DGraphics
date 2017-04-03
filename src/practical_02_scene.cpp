#include "../include/ShaderProgram.hpp"
#include "../include/Viewer.hpp"

#include "../include/FrameRenderable.hpp"
#include "../teachers/CylinderRenderable.hpp"
#include "../teachers/MeshRenderable.hpp"


void initialize_practical_02_scene(Viewer& viewer)
{
    // create all shaders of this scene, then add them to the viewer
    ShaderProgramPtr flatShader
        = std::make_shared<ShaderProgram>("../shaders/flatVertex.glsl",
                                          "../shaders/flatFragment.glsl");
    viewer.addShaderProgram(flatShader);


    // create renderable objects
    viewer.addRenderable(std::make_shared<FrameRenderable>(flatShader));

    // cylinder with normal per face
    std::shared_ptr<teachers::CylinderRenderable> teachersCylinder
        = std::make_shared<teachers::CylinderRenderable>(flatShader, false, 30);
    teachersCylinder->setModelMatrix(glm::translate(glm::mat4(), glm::vec3(1.5, 0.0, 0.0)));
    viewer.addRenderable(teachersCylinder);

    // cylinder with normal per vertex
    std::shared_ptr<teachers::CylinderRenderable> teachersIndexedCylinder
        = std::make_shared<teachers::CylinderRenderable>(flatShader, true, 30);
    teachersIndexedCylinder->setModelMatrix(glm::translate(glm::mat4(), glm::vec3(-1.5, 0.0, 0.0)));
    viewer.addRenderable(teachersIndexedCylinder);

    std::shared_ptr<teachers::MeshRenderable> teachersMesh
        = std::make_shared<teachers::MeshRenderable>(flatShader, "../meshes/suzanne.obj");
    teachersMesh->setModelMatrix( glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, -3.0)));
    viewer.addRenderable(teachersMesh);
}
