#include "../include/ShaderProgram.hpp"
#include "../include/Viewer.hpp"

#include "../include/FrameRenderable.hpp"
#include "../teachers/CubeRenderable.hpp"
#include "../teachers/IndexedCubeRenderable.hpp"
#include "../teachers/CylinderRenderable.hpp"
#include "../teachers/MeshRenderable.hpp"


void initialize_practical_03_scene(Viewer& viewer)
{
    // create all shaders of this scene, then add them to the viewer
    ShaderProgramPtr flatShader
        = std::make_shared<ShaderProgram>("../shaders/flatVertex.glsl",
                                          "../shaders/flatFragment.glsl");
    viewer.addShaderProgram(flatShader);

    // create renderable objects
    viewer.addRenderable( std::make_shared<FrameRenderable>(flatShader));

    // Temporary variables to use to define transformation
    glm::mat4 rotationM(1.0), rot1(1.0), rot2(1.0);
    glm::mat4 scaleM(1.0);
    glm::mat4 translationM(1.0);

    //Torso
    std::shared_ptr<teachers::CylinderRenderable> torso
        = std::make_shared<teachers::CylinderRenderable>(flatShader);
    scaleM = glm::scale(glm::mat4(1.0), glm::vec3(0.5,0.5,2.0));
    torso->setLocalTransform(scaleM*torso->getModelMatrix());

    //Head
    std::shared_ptr<teachers::MeshRenderable> head
        = std::make_shared<teachers::MeshRenderable>(flatShader, "./../meshes/suzanne.obj");
    translationM = glm::translate(glm::mat4(1.0), glm::vec3(0,0,3));
    head->setParentTransform(translationM);
    rotationM = glm::rotate(glm::mat4(1.0), (float)(M_PI/2.0), glm::vec3(1,0,0));
    head->setLocalTransform(rotationM);

    //Left arm
    std::shared_ptr<teachers::CylinderRenderable> leftArm
        = std::make_shared<teachers::CylinderRenderable>(flatShader);
    rotationM = glm::rotate(glm::mat4(1.0), -(float)(M_PI/2.0), glm::vec3(0,1,0));
    translationM = glm::translate(glm::mat4(1.0), glm::vec3(0.0,0.0,2.0));
    leftArm->setParentTransform(translationM);
    scaleM = glm::scale(glm::mat4(1.0), glm::vec3(0.5,0.5,2.0));
    leftArm->setLocalTransform(rotationM*scaleM);

    //Left hand
    std::shared_ptr<teachers::MeshRenderable> leftHand
        = std::make_shared<teachers::MeshRenderable>(flatShader, "./../meshes/suzanne.obj");
    translationM = glm::translate(glm::mat4(1.0), glm::vec3(-2.5,0,0));
    leftHand->setParentTransform(translationM);
    scaleM = glm::scale(glm::mat4(1.0), glm::vec3(0.5,0.5,0.5));
    rot1 = glm::rotate(glm::mat4(1.0), (float)(M_PI/2.0), glm::vec3(1,0,0));
    rot2 = glm::rotate(glm::mat4(1.0), -(float)(M_PI/2.0), glm::vec3(0,0,1));
    leftHand->setLocalTransform(rot2*rot1*scaleM);

    //Right arm
    std::shared_ptr<teachers::CylinderRenderable> rightArm
        = std::make_shared<teachers::CylinderRenderable>(flatShader);
    rotationM = glm::rotate(glm::mat4(1.0), (float)(M_PI/2.0), glm::vec3(0,1,0));
    translationM = glm::translate(glm::mat4(1.0), glm::vec3(0.0,0.0,2.0));
    rightArm->setParentTransform(translationM);
    scaleM = glm::scale(glm::mat4(1.0), glm::vec3(0.5,0.5,2.0));
    rightArm->setLocalTransform(rotationM*scaleM);

    //Right hand
    std::shared_ptr<teachers::MeshRenderable> rightHand
        = std::make_shared<teachers::MeshRenderable>(flatShader, "./../meshes/suzanne.obj");
    translationM = glm::translate(glm::mat4(1.0), glm::vec3(2.5,0,0));
    rightHand->setParentTransform(translationM);
    scaleM = glm::scale(glm::mat4(1.0), glm::vec3(0.5,0.5,0.5));
    rot1 = glm::rotate(glm::mat4(1.0), (float)(M_PI/2.0), glm::vec3(1,0,0));
    rot2 = glm::rotate(glm::mat4(1.0), (float)(M_PI/2.0), glm::vec3(0,0,1));
    rightHand->setLocalTransform(rot2*rot1*scaleM);

    HierarchicalRenderable::addChild(torso, head);
    HierarchicalRenderable::addChild(torso, leftArm);
    HierarchicalRenderable::addChild(torso, rightArm);
    HierarchicalRenderable::addChild(leftArm, leftHand);
    HierarchicalRenderable::addChild(rightArm, rightHand);
    viewer.addRenderable(torso);
}
