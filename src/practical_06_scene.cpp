// #include "../include/practical_04_scene.hpp"
#include "../include/ShaderProgram.hpp"
#include "../include/Viewer.hpp"
#include "../include/FrameRenderable.hpp"
#include "../include/lighting/DirectionalLightRenderable.hpp"

#include "../include/keyframes/KeyframedCylinderRenderable.hpp"
#include "../include/keyframes/GeometricTransformation.hpp"

#include <iostream>
#include <iomanip>


static void practical_06_movingCylinder(Viewer& viewer, const ShaderProgramPtr& shader);
static void practical_06_movingTree(Viewer& viewer, const ShaderProgramPtr& shader);

void initialize_practical_06_scene(Viewer& viewer)
{
    // Shaders
    ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>(
        "../shaders/flatVertex.glsl", "../shaders/flatFragment.glsl");
    ShaderProgramPtr phongShader = std::make_shared<ShaderProgram>(
        "../shaders/phongVertex.glsl", "../shaders/phongFragment.glsl");
    viewer.addShaderProgram(flatShader);
    viewer.addShaderProgram(phongShader);

    // Frame
    FrameRenderablePtr frame = std::make_shared<FrameRenderable>(flatShader);
    viewer.addRenderable(frame);

    // Define a directional light for the whole scene
    glm::vec3 lightDirection = glm::normalize(glm::vec3(0.0, -1.0, -1.0));
    glm::vec3 ghostWhite(248.0/255, 248.0/255, 1.0);
    DirectionalLightPtr directionalLight =
        std::make_shared<DirectionalLight>(lightDirection, ghostWhite, ghostWhite, ghostWhite);
    viewer.setDirectionalLight(directionalLight);
    // Add a renderable to display the light and control it via mouse/key event
    glm::vec3 lightPosition(0.0, 5.0, 8.0);
    DirectionalLightRenderablePtr directionalLightRenderable
        = std::make_shared<DirectionalLightRenderable>(flatShader, directionalLight, lightPosition);
    glm::mat4 localTransformation = glm::scale(glm::mat4(1.0), glm::vec3(0.5, 0.5, 0.5));
    directionalLightRenderable->setLocalTransform(localTransformation);
    viewer.addRenderable(directionalLightRenderable);

    // Position the camera
    viewer.getCamera().setViewMatrix(
        glm::lookAt(glm::vec3(0, -8, 7), glm::vec3(0, 0, 4), glm::vec3(0, 0, 1)) );

    // Set the scene
    practical_06_movingCylinder(viewer, phongShader);
    practical_06_movingTree(viewer, phongShader);

    // Let's go!
    viewer.setAnimationLoop(true, 6.0);
    viewer.startAnimation();
}

void practical_06_movingCylinder(Viewer& viewer, const ShaderProgramPtr& shader)
{
    //Animated cylinder
    auto cylinder = std::make_shared<KeyframedCylinderRenderable>(shader, Material::Emerald());
    cylinder->setParentTransform(glm::mat4(1.0));

    //Keyframes on parent transformation: pairs of (time, transformation)
    cylinder->addParentTransformKeyframe(0.0, GeometricTransformation(glm::vec3(0.0, 0.0, 0.0), glm::angleAxis(0.0f, glm::vec3(0.0, 0.0, 1.0))) );
    cylinder->addParentTransformKeyframe(1.0, GeometricTransformation(glm::vec3(1.0, 0.0, 0.0), glm::angleAxis((float)(2.0*M_PI/4.0), glm::vec3(0.0, 1.0, 0.0))) );
    cylinder->addParentTransformKeyframe(2.0, GeometricTransformation(glm::vec3(0.0, 2.0, 0.0), glm::angleAxis((float)(2.0*M_PI/4.0), glm::vec3(0.0, 1.0, 0.0))) );
    cylinder->addParentTransformKeyframe(3.0, GeometricTransformation(glm::vec3(-2.0, 0.0, 0.0), glm::angleAxis((float)(6.0*M_PI/4.0), glm::vec3(0.0, 1.0, 0.0))) );
    cylinder->addParentTransformKeyframe(4.0, GeometricTransformation(glm::vec3(0.0, 0.0, 0.0), glm::angleAxis(0.0f, glm::vec3(0.0, 1.0, 0.0))) );

    viewer.addRenderable(cylinder);
}


void practical_06_movingTree(Viewer& viewer, const ShaderProgramPtr& shader)
{
    //Tree modeling:
    //The modeling is hierarchical (setLocalTransform, setParentTransform)
    //The animation is hierarchical too (addParentTransformKeyframe, addLocalTransformKeyframe)

    //Main branch
    auto root = std::make_shared<KeyframedCylinderRenderable>(shader);
    root->setLocalTransform( GeometricTransformation( glm::vec3{}, glm::quat(), glm::vec3{0.1,0.1,2.0}).toMatrix() );

    root->addParentTransformKeyframe(0.0, GeometricTransformation( glm::vec3{2,0,0}) );
    root->addParentTransformKeyframe(1.0, GeometricTransformation( glm::vec3{2,0,0}, glm::quat(glm::vec3(0, 0.1, 0))) );
    root->addParentTransformKeyframe(2.0, GeometricTransformation( glm::vec3{2,0,0}, glm::quat(glm::vec3(0, 0.1, 0))) );
    root->addParentTransformKeyframe(2.5, GeometricTransformation( glm::vec3{2,0,0}, glm::quat(glm::vec3(0, 0.3, 0))) );
    root->addParentTransformKeyframe(3.0, GeometricTransformation( glm::vec3{2,0,0}, glm::quat(glm::vec3(0, 0.3, 0))) );
    root->addParentTransformKeyframe(5.0, GeometricTransformation( glm::vec3{2,0,0}, glm::quat(glm::vec3(0, 0.0, 0))) );

    //First primary branch
    auto r1 = std::make_shared<KeyframedCylinderRenderable>(shader);

    r1->addParentTransformKeyframe(0.0, GeometricTransformation( glm::vec3{0,0,2.0}, glm::quat( glm::vec3(0, 0.78, 0))) );
    r1->addParentTransformKeyframe(2.0, GeometricTransformation( glm::vec3{0,0,2.0}, glm::quat( glm::vec3(0, 0.84, 0))) );
    r1->addParentTransformKeyframe(5.0, GeometricTransformation( glm::vec3{0,0,2.0}, glm::quat( glm::vec3(0, float(M_PI_4), 0))) );

    r1->addLocalTransformKeyframe(0.0, GeometricTransformation( glm::vec3{}, glm::quat(), glm::vec3{0.1,0.1,2.0}) );
    r1->addLocalTransformKeyframe(2.0, GeometricTransformation( glm::vec3{}, glm::quat(), glm::vec3{0.06,0.06,2.0}) );
    r1->addLocalTransformKeyframe(5.0, GeometricTransformation( glm::vec3{}, glm::quat(), glm::vec3{0.1,0.1,2.0}) );
    HierarchicalRenderable::addChild(root, r1);


    auto r2 = std::make_shared<KeyframedCylinderRenderable>(shader);

    r2->addParentTransformKeyframe(0.0, GeometricTransformation( glm::vec3{0,0,2.0}, glm::quat( glm::vec3( 0, 0.35, 0))) );
    r2->addParentTransformKeyframe(1.0, GeometricTransformation( glm::vec3{0,0,2.0}, glm::quat( glm::vec3( 0, 0.25, 0))) );
    r2->addParentTransformKeyframe(2.0, GeometricTransformation( glm::vec3{0,0,2.0}, glm::quat( glm::vec3( 0, 0.32, 0))) );
    r2->addParentTransformKeyframe(3.0, GeometricTransformation( glm::vec3{0,0,2.0}, glm::quat( glm::vec3( 0, 0.25, 0))) );
    r2->addParentTransformKeyframe(5.0, GeometricTransformation( glm::vec3{0,0,2.0}, glm::quat( glm::vec3( 0, 0.35, 0))) );

    r2->addLocalTransformKeyframe(0.0, GeometricTransformation( glm::vec3{}, glm::quat(), glm::vec3{0.1,0.1,2.0}) );
    r2->addLocalTransformKeyframe(2.0, GeometricTransformation( glm::vec3{}, glm::quat(), glm::vec3{0.04,0.04,2.0}) );
    r2->addLocalTransformKeyframe(5.0, GeometricTransformation( glm::vec3{}, glm::quat(), glm::vec3{0.1,0.1,2.0}) );

    HierarchicalRenderable::addChild(r1, r2);

    //First secondary branch
    auto s1 = std::make_shared<KeyframedCylinderRenderable>(shader);

    s1->addParentTransformKeyframe(0.0, GeometricTransformation( glm::vec3{0,0,0.4}, glm::angleAxis( float(M_PI_4), glm::normalize(glm::vec3( 1,1,0)) ), glm::vec3{1,1,1}) );

    s1->addLocalTransformKeyframe(0.0, GeometricTransformation( glm::vec3{}, glm::quat(), glm::vec3{0.1,0.1,1.5}) );
    s1->addLocalTransformKeyframe(3.0, GeometricTransformation( glm::vec3{}, glm::quat(), glm::vec3{0.06,0.06,1.5}) );
    s1->addLocalTransformKeyframe(5.0, GeometricTransformation( glm::vec3{}, glm::quat(), glm::vec3{0.1,0.1,1.5}) );
    HierarchicalRenderable::addChild(r1, s1);

    auto s11 = std::make_shared<KeyframedCylinderRenderable>(shader);
    s11->addParentTransformKeyframe(0.0, GeometricTransformation( glm::vec3{0,0,1.5}, glm::quat( glm::vec3( 0, -0.78, 0) ), glm::vec3{1,1,1}) );
    s11->addParentTransformKeyframe(2.0, GeometricTransformation( glm::vec3{0,0,1.5}, glm::quat( glm::vec3( 0, -0.4, 0) ), glm::vec3{1,1,1}) );
    s11->addParentTransformKeyframe(3.0, GeometricTransformation( glm::vec3{0,0,1.5}, glm::quat( glm::vec3( 0, -0.6, 0) ), glm::vec3{1,1,1}) );
    s11->addParentTransformKeyframe(4.5, GeometricTransformation( glm::vec3{0,0,1.5}, glm::quat( glm::vec3( 0, -0.65, 0) ), glm::vec3{1,1,1}) );
    s11->addParentTransformKeyframe(5.0, GeometricTransformation( glm::vec3{0,0,1.5}, glm::quat( glm::vec3( 0, -0.78, 0) ), glm::vec3{1,1,1}) );

    s11->addLocalTransformKeyframe(0.0, GeometricTransformation( glm::vec3{}, glm::quat(), glm::vec3{0.1,0.1,2.0}) );
    s11->addLocalTransformKeyframe(3.0, GeometricTransformation( glm::vec3{}, glm::quat(), glm::vec3{0.04,0.04,1.5}) );
    s11->addLocalTransformKeyframe(5.0, GeometricTransformation( glm::vec3{}, glm::quat(), glm::vec3{0.1,0.1,2.0}) );

    HierarchicalRenderable::addChild(s1, s11);

    //Second secondary branch
    auto s2 = std::make_shared<KeyframedCylinderRenderable>(shader);
    s2->setParentTransform( GeometricTransformation( glm::vec3{0,0,1.4}, glm::angleAxis( float(M_PI_4), glm::normalize(glm::vec3(-0.3 ,-1,0)) ), glm::vec3{1,1,1} ).toMatrix() );
    s2->setLocalTransform( GeometricTransformation( glm::vec3{}, glm::quat(), glm::vec3{0.1,0.1,2.0}).toMatrix() );
    HierarchicalRenderable::addChild(r1, s2);

    auto s21 = std::make_shared<KeyframedCylinderRenderable>(shader);
    s21->setParentTransform( GeometricTransformation( glm::vec3{0,0,2.0}, glm::angleAxis( float(-M_PI_4), glm::normalize(glm::vec3( 0, -1, 0)) ), glm::vec3{1,1,1} ).toMatrix() );
    s21->setLocalTransform( GeometricTransformation( glm::vec3{}, glm::quat(), glm::vec3{0.1,0.1,2.0}).toMatrix() );
    HierarchicalRenderable::addChild(s2, s21);

    //Third secondary branch
    auto s3 = std::make_shared<KeyframedCylinderRenderable>(shader);
    s3->addParentTransformKeyframe(0.0, GeometricTransformation( glm::vec3{0, 0, 2 }, glm::quat( glm::vec3{-0.78, -0.6, -0.5})) );
    s3->addParentTransformKeyframe(2.0, GeometricTransformation( glm::vec3{0, 0, 2 }, glm::quat( glm::vec3{-0.78,  0.3, -0.5})) );
    s3->addParentTransformKeyframe(5.0, GeometricTransformation( glm::vec3{0, 0, 2 }, glm::quat( glm::vec3{-0.78, -0.6, -0.5})) );

    s3->addLocalTransformKeyframe(0.0, GeometricTransformation( glm::vec3{}, glm::quat(), glm::vec3{0.1,0.1,2.0}));
    s3->addLocalTransformKeyframe(1.0, GeometricTransformation( glm::vec3{}, glm::quat(), glm::vec3{0.05,0.05,3.0}));
    s3->addLocalTransformKeyframe(4.0, GeometricTransformation( glm::vec3{}, glm::quat(), glm::vec3{0.08,0.08,2.5}));
    s3->addLocalTransformKeyframe(5.0, GeometricTransformation( glm::vec3{}, glm::quat(), glm::vec3{0.1,0.1,2.0}));
    HierarchicalRenderable::addChild(r1, s3);

    viewer.addRenderable(root);
}
