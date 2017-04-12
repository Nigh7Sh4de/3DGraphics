#include "../include/Viewer.hpp"
#include "../include/FrameRenderable.hpp"
#include "../teachers/MeshRenderable.hpp"

#include "../include/dynamics/DynamicSystem.hpp"
#include "../include/dynamics/DampingForceField.hpp"
#include "../include/dynamics/ConstantForceField.hpp"
#include "../include/dynamics/SpringForceField.hpp"
#include "../include/dynamics/EulerExplicitSolver.hpp"

#include "../include/dynamics_rendering/DynamicSystemRenderable.hpp"
#include "../include/dynamics_rendering/ParticleRenderable.hpp"
#include "../include/dynamics_rendering/ParticleListRenderable.hpp"
#include "../include/dynamics_rendering/ConstantForceFieldRenderable.hpp"
#include "../include/dynamics_rendering/SpringForceFieldRenderable.hpp"
#include "../include/dynamics_rendering/SpringListRenderable.hpp"
#include "../include/dynamics_rendering/ControlledForceFieldRenderable.hpp"
#include "../include/dynamics_rendering/QuadRenderable.hpp"

#include "../include/SphereRenderable.hpp"
#include "../include/Skybox.hpp"
#include "../include/dynamics_rendering/PuckRenderable.hpp"
#include "../include/dynamics_rendering/TerrainRenderable.hpp"
#include "../include/dynamics_rendering/BearRenderable.hpp"
#include "../include/dynamics_rendering/SnowFallRenderable.hpp"
#include "../teachers/CylinderRenderable.hpp"
#include "../include/texturing/TexturedPlaneRenderable.hpp"
#include "../include/keyframes/KeyframedCylinderRenderable.hpp"
#include "../include/keyframes/GeometricTransformation.hpp"

#include "../include/lighting/SpotLightRenderable.hpp"

void project_terrain(Viewer &viewer, DynamicSystemPtr &system, DynamicSystemRenderablePtr &systemRenderable, ShaderProgramPtr &shader);
void project_skybox(Viewer &viewer, DynamicSystemPtr &system, DynamicSystemRenderablePtr &systemRenderable, ShaderProgramPtr &shader);
void project_lake(Viewer &viewer, DynamicSystemPtr &system, DynamicSystemRenderablePtr &systemRenderable, ShaderProgramPtr &shader);
void project_character(Viewer &viewer, ShaderProgramPtr &shader);
void project_snowman(Viewer &viewer, DynamicSystemPtr &system, DynamicSystemRenderablePtr &systemRenderable, ShaderProgramPtr &shader);
void project_hockey_stick(Viewer &viewer, DynamicSystemPtr &system, DynamicSystemRenderablePtr &systemRenderable, ShaderProgramPtr &shader);
void project_puck(Viewer &viewer, DynamicSystemPtr &system, DynamicSystemRenderablePtr &systemRenderable, ShaderProgramPtr &shader);
void project_snowfall(Viewer &viewer, DynamicSystemPtr &system, DynamicSystemRenderablePtr &systemRenderable, ConstantForceFieldPtr gravityForceField, ShaderProgramPtr &shader);
void project_spring_gate(Viewer &viewer, DynamicSystemPtr &system, DynamicSystemRenderablePtr &systemRenderable, ShaderProgramPtr &shader);

void initialize_project_scene(Viewer &viewer)
{
    //Shaders
    ShaderProgramPtr flatShader =
        std::make_shared<ShaderProgram>("../shaders/flatVertex.glsl",
                                        "../shaders/flatFragment.glsl");
    ShaderProgramPtr phongShader = std::make_shared<ShaderProgram>("../shaders/phongVertex.glsl",
                                                                   "../shaders/phongFragment.glsl");
    ShaderProgramPtr texShader = std::make_shared<ShaderProgram>("../shaders/textureVertex.glsl",
                                                                 "../shaders/textureFragment.glsl");
    ShaderProgramPtr skyboxShader = std::make_shared<ShaderProgram>("../shaders/skyboxVertex.glsl",
                                                                 "../shaders/skyboxFragment.glsl");
    viewer.addShaderProgram(skyboxShader);
    viewer.addShaderProgram(flatShader);
    viewer.addShaderProgram(phongShader);
    viewer.addShaderProgram(texShader);

    FrameRenderablePtr frame = std::make_shared<FrameRenderable>(flatShader);
    viewer.addRenderable(frame);

    //Initialize a dynamic system (Solver, Time step, Restitution coefficient)
    DynamicSystemPtr system = std::make_shared<DynamicSystem>();
    EulerExplicitSolverPtr solver = std::make_shared<EulerExplicitSolver>();
    system->setSolver(solver);
    system->setDt(0.01);

    //Create a renderable associated to the dynamic system
    //This renderable is responsible for calling DynamicSystem::computeSimulationStep()in the animate() function
    //It also handles some of the key/mouse events
    DynamicSystemRenderablePtr systemRenderable = std::make_shared<DynamicSystemRenderable>(system);
    viewer.addRenderable(systemRenderable);

    //Position the camera
    viewer.getCamera().setViewMatrix(
        glm::lookAt(glm::vec3(0, -20, 20), glm::vec3(0, 0, 0), glm::vec3(0, -1, 0)));

    // Define a directional light for the whole scene
    glm::vec3 lightDirection = glm::normalize(glm::vec3(0.0, 0.5, 1.0));
//    glm::vec3 ghostWhite(248.0 / 255, 248.0 / 255, 1.0);
    glm::vec3 ghostWhite(0.8, 0.8, 0.8);
    DirectionalLightPtr directionalLight =
        std::make_shared<DirectionalLight>(lightDirection, ghostWhite, ghostWhite, ghostWhite);
    viewer.setDirectionalLight(directionalLight);


//GABRIEL


    //Define a transformation
    glm::mat4 parentTransformation, localTransformation;

    //Define a spot light 1 - RED
    glm::vec3 s_position(-10.0,-7.0,0.0), s_spotDirection = glm::normalize(glm::vec3(0.0,1.0,0.0)); // posicao da lampada e direcao dela
    //glm::vec3 s_ambient(0.0,0.0,0.0), s_diffuse(0.0,0.0,0.0), s_specular(0.0,0.0,0.0);
    glm::vec3 s_ambient(1.5,0.8,0.8), s_diffuse(1.5,0.2,0.2), s_specular(1.5,0.0,0.0);
    float s_constant=1.0, s_linear=0.1, s_quadratic=0.0;
    float s_cosInnerCutOff = std::cos(glm::radians(20.0f)); // tava é 20 rad
    float s_cosOuterCutOff = std::cos(glm::radians(40.0f)); // tava 40 rad
    SpotLightPtr spotLight = std::make_shared<SpotLight>(s_position, s_spotDirection,
                                                         s_ambient, s_diffuse, s_specular,
                                                         s_constant, s_linear, s_quadratic,
                                                         s_cosInnerCutOff, s_cosOuterCutOff);
    SpotLightRenderablePtr spotLightRenderable = std::make_shared<SpotLightRenderable>(flatShader, spotLight);
    localTransformation = glm::scale(glm::mat4(1.0), glm::vec3(1.0,1.0,1.0));
    spotLightRenderable->setLocalTransform(localTransformation);
    viewer.addSpotLight(spotLight);
    //viewer.addRenderable(spotLightRenderable);



    //Define a spot light 2 - GREEN
    glm::vec3 s_position2(10.0,-7.0,0.0), s_spotDirection2 = glm::normalize(glm::vec3(0.0,1.0,0.0)); // posicao da lampada e direcao dela
    //glm::vec3 s_ambient(0.0,0.0,0.0), s_diffuse(0.0,0.0,0.0), s_specular(0.0,0.0,0.0);
    glm::vec3 s_ambient2(0.8,1.5,0.8), s_diffuse2(0.2,1.5,0.2), s_specular2(0.0,1.5,0.0);
    float s_constant2=1.0, s_linear2=0.1, s_quadratic2=0.0;
    float s_cosInnerCutOff2 = std::cos(glm::radians(10.0f)); // tava é 20 rad
    float s_cosOuterCutOff2 = std::cos(glm::radians(20.0f)); // tava 40 rad
    SpotLightPtr spotLight2 = std::make_shared<SpotLight>(s_position2, s_spotDirection2,
                                                         s_ambient2, s_diffuse2, s_specular2,
                                                         s_constant2, s_linear2, s_quadratic2,
                                                         s_cosInnerCutOff2, s_cosOuterCutOff2);
    SpotLightRenderablePtr spotLightRenderable2 = std::make_shared<SpotLightRenderable>(flatShader, spotLight2);
    localTransformation = glm::scale(glm::mat4(1.0), glm::vec3(1.0,1.0,1.0));
    spotLightRenderable2->setLocalTransform(localTransformation);
    viewer.addSpotLight(spotLight2);
    //viewer.addRenderable(spotLightRenderable2); // show the cone or not

    //Define a spot light 3 - BLUE
    glm::vec3 s_position3(0.0,-7.0,0.0), s_spotDirection3 = glm::normalize(glm::vec3(0.0,1.0,0.0)); // posicao da lampada e direcao dela
    //glm::vec3 s_ambient(0.0,0.0,0.0), s_diffuse(0.0,0.0,0.0), s_specular(0.0,0.0,0.0);
    glm::vec3 s_ambient3(0.8,0.8,1.5), s_diffuse3(0.2,0.2,1.5), s_specular3(0.0,0.0,1.5);
    float s_constant3=1.0, s_linear3=0.1, s_quadratic3=0.0;
    float s_cosInnerCutOff3 = std::cos(glm::radians(10.0f)); // tava é 20 rad
    float s_cosOuterCutOff3 = std::cos(glm::radians(20.0f)); // tava 40 rad
    SpotLightPtr spotLight3 = std::make_shared<SpotLight>(s_position3, s_spotDirection3,
                                                         s_ambient3, s_diffuse3, s_specular3,
                                                         s_constant3, s_linear3, s_quadratic3,
                                                         s_cosInnerCutOff3, s_cosOuterCutOff3);
    SpotLightRenderablePtr spotLightRenderable3 = std::make_shared<SpotLightRenderable>(flatShader, spotLight3);
    localTransformation = glm::scale(glm::mat4(1.0), glm::vec3(1.0,1.0,1.0));
    spotLightRenderable3->setLocalTransform(localTransformation);
    viewer.addSpotLight(spotLight3);
    //viewer.addRenderable(spotLightRenderable3); // show the cone or not


//GABRIEL

    //Populate the dynamic system with particles, forcefields
    //and create renderables associated to them for visualization.
    project_terrain(viewer, system, systemRenderable, flatShader);
    project_skybox(viewer, system, systemRenderable, skyboxShader);
    project_lake(viewer, system, systemRenderable, texShader);
    project_character(viewer, flatShader);
    project_hockey_stick(viewer, system, systemRenderable, phongShader);
    project_snowman(viewer, system, systemRenderable, flatShader);
    project_puck(viewer, system, systemRenderable, flatShader);
    project_spring_gate(viewer, system, systemRenderable, flatShader);

    //Initialize a force field that apply to all the particles of the system to simulate gravity
    //Add it to the system as a force field
    ConstantForceFieldPtr gravityForceField = std::make_shared<ConstantForceField>(system->getParticles(), glm::vec3{0, 0, -10});
    system->addForceField(gravityForceField);

    project_snowfall(viewer, system, systemRenderable, gravityForceField, flatShader);

    // rotate the whole scene so its aligned with skybox
    systemRenderable->setParentTransform(glm::rotate(glm::mat4(1.0), (float)(M_PI / 2.0), glm::vec3(1.0, 0.0, 0.0)));

    //Finally, run the animation
    viewer.startAnimation();
}

void project_snowfall(Viewer &viewer, DynamicSystemPtr &system, DynamicSystemRenderablePtr &systemRenderable, ConstantForceFieldPtr gravityForceField, ShaderProgramPtr &shader)
{
    SnowFallRenderablePtr snowfall = std::make_shared<SnowFallRenderable>(shader, system, systemRenderable, gravityForceField);
    HierarchicalRenderable::addChild(systemRenderable, snowfall);
}

void project_terrain(Viewer &viewer, DynamicSystemPtr &system, DynamicSystemRenderablePtr &systemRenderable, ShaderProgramPtr &shader)
{
    TerrainRenderablePtr terrain = std::make_shared<TerrainRenderable>(shader);
    HierarchicalRenderable::addChild(systemRenderable, terrain);
}

void project_skybox(Viewer &viewer, DynamicSystemPtr &system, DynamicSystemRenderablePtr &systemRenderable, ShaderProgramPtr &shader)
{
    HierarchicalRenderablePtr skybox = std::make_shared<Skybox>(shader);
    viewer.addRenderable(skybox);
}

void project_lake(Viewer &viewer, DynamicSystemPtr &system, DynamicSystemRenderablePtr &systemRenderable, ShaderProgramPtr &shader)
{
    //Activate collision detection
    system->setCollisionsDetection(true);

    //Initialize the restitution coefficient for collision
    //1.0 = full elastic response
    //0.0 = full absorption
    system->setRestitution(0.9f);

    //Initialize a plane from 3 points and add it to the system as an obstacle
    glm::vec3 p1(-1.0, -1.0, 0.0);
    glm::vec3 p2(1.0, -1.0, 0.0);
    glm::vec3 p3(1.0, 1.0, 0.0);
    glm::vec3 p4(-1.0, 1.0, 0.0);
    PlanePtr plane = std::make_shared<Plane>(p1, p2, p3);
    system->addPlaneObstacle(plane);

    //Create a plane renderable to display the obstacle
    //Textured plane
    std::string filename = "../textures/ice_lake.jpg";
    TexturedPlaneRenderablePtr texPlane = std::make_shared<TexturedPlaneRenderable>(shader, filename);
    texPlane->setParentTransform(glm::scale(glm::mat4(2.0), glm::vec3(40.0, 30.0, 30.0)));
    texPlane->setMaterial(Material::White());
    HierarchicalRenderable::addChild(systemRenderable, texPlane);
}

void project_character(Viewer &viewer, ShaderProgramPtr &shader) {
      BearRenderablePtr bear = std::make_shared<BearRenderable>(shader);
      viewer.addRenderable(bear);
      bear->setModelMatrix( glm::translate(glm::mat4(1.0), glm::vec3(0.0, -5.0, 3.0)));
      bear->setParentTransform(glm::scale(glm::mat4(1.0), glm::vec3(0.3, -0.3, -0.3)));
}

void project_snowman(Viewer &viewer, DynamicSystemPtr &system, DynamicSystemRenderablePtr &systemRenderable, ShaderProgramPtr &shader)
{
    // snowman
    SphereRenderablePtr bottomBall = std::make_shared<SphereRenderable>(shader, glm::vec4(0.9), 20, 20);
    SphereRenderablePtr middleBall = std::make_shared<SphereRenderable>(shader, glm::vec4(0.9), 20, 20);
    SphereRenderablePtr topBall = std::make_shared<SphereRenderable>(shader, glm::vec4(0.9), 20, 20);
    SphereRenderablePtr leftEye = std::make_shared<SphereRenderable>(shader, glm::vec4(0.0), 20, 20);
    SphereRenderablePtr rightEye = std::make_shared<SphereRenderable>(shader, glm::vec4(0.0), 20, 20);
    std::shared_ptr<teachers::CylinderRenderable> nose = std::make_shared<teachers::CylinderRenderable>(shader, true, 30, glm::vec4(1.0, 0.0, 0.0, 0.0));

    bottomBall->setLocalTransform(
        glm::scale(glm::mat4(1.0), glm::vec3(3, 3, 3)) *
        bottomBall->getModelMatrix());

    middleBall->setLocalTransform(
        glm::translate(glm::mat4(1.0), glm::vec3(0, 0, 3)) *
        glm::scale(glm::mat4(1.0), glm::vec3(2, 2, 2)) *
        middleBall->getModelMatrix());

    topBall->setParentTransform(
        glm::translate(glm::mat4(1.0), glm::vec3(0, 0, 5)));

    leftEye->setLocalTransform(
        glm::translate(glm::mat4(1.0), glm::vec3(0.75, 0.5, 0.5)) *
        glm::scale(glm::mat4(1.0), glm::vec3(0.15, 0.15, 0.15)) *
        leftEye->getModelMatrix());

    rightEye->setLocalTransform(
        glm::translate(glm::mat4(1.0), glm::vec3(0.75, -0.5, 0.5)) *
        glm::scale(glm::mat4(1.0), glm::vec3(0.15, 0.15, 0.15)) *
        rightEye->getModelMatrix());

    nose->setLocalTransform(
        glm::translate(glm::mat4(1.0), glm::vec3(0.75, 0, 0.25)) *
        glm::rotate(glm::mat4(1.0), (float)(M_PI / 2.0), glm::vec3(0, 1, 0)) *
        glm::scale(glm::mat4(1.0), glm::vec3(0.15, 0.15, 1)) *
        nose->getModelMatrix());

    bottomBall->setParentTransform(
        glm::translate(glm::mat4(1.0), glm::vec3(-15, 0, 3)));

    HierarchicalRenderable::addChild(topBall, leftEye);
    HierarchicalRenderable::addChild(topBall, rightEye);
    HierarchicalRenderable::addChild(topBall, nose);
    HierarchicalRenderable::addChild(bottomBall, topBall);
    HierarchicalRenderable::addChild(bottomBall, middleBall);
    HierarchicalRenderable::addChild(systemRenderable, bottomBall);
}

void project_hockey_stick(Viewer &viewer, DynamicSystemPtr &system, DynamicSystemRenderablePtr &systemRenderable, ShaderProgramPtr &shader)
{
    // hockey stick
    auto stick = std::make_shared<KeyframedCylinderRenderable>(shader, Material::Bronze());
    stick->setLocalTransform(GeometricTransformation(glm::vec3{0.0, 0.0, 0.0}, glm::quat(glm::vec3(0.0, 0.0, 0.0)), glm::vec3{0.1, 0.25, 5.0}).toMatrix());

    stick->addParentTransformKeyframe(0.0, GeometricTransformation(glm::vec3{-14.0, -2.25, 5.0}, glm::quat(glm::vec3(0.3, 5, 0.0))));
    stick->addParentTransformKeyframe(1.5, GeometricTransformation(glm::vec3{-14.0, -2.25, 5.0}, glm::quat(glm::vec3(0.3, 5, 0.0))));
    stick->addParentTransformKeyframe(2, GeometricTransformation(glm::vec3{-14.0, -2.25, 5.0}, glm::quat(glm::vec3(0.3, 2.7, 0.0))));

    // stick leg
    auto stick_leg = std::make_shared<KeyframedCylinderRenderable>(shader, Material::Bronze());

    stick_leg->addParentTransformKeyframe(0.0, GeometricTransformation(glm::vec3{-0.5, -0.8, 5.0}, glm::quat(glm::vec3(0, 1.5, 1)), glm::vec3{0.25, 0.1, 1.0}));

    HierarchicalRenderable::addChild(stick, stick_leg);
    HierarchicalRenderable::addChild(systemRenderable, stick);
}

void project_puck(Viewer &viewer, DynamicSystemPtr &system, DynamicSystemRenderablePtr &systemRenderable, ShaderProgramPtr &shader)
{
    // puck particle
    glm::vec3 px = glm::vec3(-12.5, -4, 0.0);
    glm::vec3 pv = glm::vec3(15.0, 1.5, 7.5);
    float pr = 0.5;
    float pm = 100.0;
    ParticlePtr puckParticle = std::make_shared<Particle>(px, pv, pm, pr);
    system->addParticle(puckParticle);

    //Create a particleRenderable for particle of the system
    PuckRenderablePtr puckRenderable = std::make_shared<PuckRenderable>(shader, puckParticle, true, 30, glm::vec4(0.0));
    HierarchicalRenderable::addChild(systemRenderable, puckRenderable);
}

void project_spring_gate(Viewer &viewer, DynamicSystemPtr &system, DynamicSystemRenderablePtr &systemRenderable, ShaderProgramPtr &shader)
{
    //Initialize particle attributes (radius, mass)
    float pr = 0.1, pm = 10.0;
    glm::vec3 px(0.0, 0.0, 0.0), pv(0.0, 0.0, 0.0);

    //Create particles on a squared uniform grid starting at origin
    std::vector<ParticlePtr> particles;
    glm::vec3 origin(15, -2.5, 0.0), displacement(0.0, 0.0, 0.0);
    int particlePerLine = 11;
    float gridWidth = 4.0, gridHeight = 4.0;
    float ystep = gridWidth / (float)(particlePerLine - 1);
    float zstep = gridHeight / (float)(particlePerLine - 1);
    particles.resize(particlePerLine * particlePerLine);
    for (size_t i = 0; i < particlePerLine; ++i)
    {
        for (size_t j = 0; j < particlePerLine; ++j)
        {
            displacement = glm::vec3(0, i * ystep, j * zstep);
            px = origin + displacement;
            particles[i * particlePerLine + j] = std::make_shared<Particle>(px, pv, pm, pr);
            system->addParticle(particles[i * particlePerLine + j]);
        }
    }

    //Fix particles on the y-borders
    for (size_t j = 0; j < particlePerLine; ++j)
    {
        particles[0 * particlePerLine + j]->setFixed(true);
        particles[(particlePerLine - 1) * particlePerLine + j]->setFixed(true);
    }

    //Initialize springs attributes (stiffness, rest length, damping)
    float stiffness = 8e3;
    float l0 = gridWidth / (particlePerLine - 1);
    float damping = 5.0;

    //Create springs between particles of the grid, horizontally and vertically
    //Store them in a list
    std::list<SpringForceFieldPtr> springForceFields;
    //Interior
    for (size_t i = 1; i < particlePerLine; ++i)
    {
        for (size_t j = 1; j < particlePerLine; ++j)
        {
            SpringForceFieldPtr spring1 = std::make_shared<SpringForceField>(particles[(i - 1) * particlePerLine + j], particles[i * particlePerLine + j], stiffness, l0, damping);
            springForceFields.push_back(spring1);
            system->addForceField(spring1);
            SpringForceFieldPtr spring2 = std::make_shared<SpringForceField>(particles[i * particlePerLine + (j - 1)], particles[i * particlePerLine + j], stiffness, l0, damping);
            springForceFields.push_back(spring2);
            system->addForceField(spring2);
        }
    }
    //Border case 1
    for (size_t j = 1; j < particlePerLine; ++j)
    {
        SpringForceFieldPtr spring = std::make_shared<SpringForceField>(particles[0 * particlePerLine + j], particles[0 * particlePerLine + (j - 1)], stiffness, l0, damping);
        springForceFields.push_back(spring);
        system->addForceField(spring);
    }
    //Border case 2
    for (size_t i = 1; i < particlePerLine; ++i)
    {
        SpringForceFieldPtr spring = std::make_shared<SpringForceField>(particles[(i - 1) * particlePerLine + 0], particles[i * particlePerLine + 0], stiffness, l0, damping);
        springForceFields.push_back(spring);
        system->addForceField(spring);
    }

    //Create a particleListRenderable to efficiently visualize the particles of the system
    ParticleListRenderablePtr particleListRenderable = std::make_shared<ParticleListRenderable>(shader, particles, glm::vec4(0.2));
    HierarchicalRenderable::addChild(systemRenderable, particleListRenderable);

    //Create a springListRenderable to efficiently visualize the springs of the system
    SpringListRenderablePtr springsRenderable = std::make_shared<SpringListRenderable>(shader, springForceFields, glm::vec4(0.2));
    HierarchicalRenderable::addChild(systemRenderable, springsRenderable);
}
