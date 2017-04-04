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

#include "../include/dynamics_rendering/PuckRenderable.hpp"
#include "../include/dynamics_rendering/TerrainRenderable.hpp"
#include "../include/SphereRenderable.hpp"
#include "../include/texturing/TexturedPlaneRenderable.hpp"

void project_terrain(Viewer &viewer, DynamicSystemPtr &system, DynamicSystemRenderablePtr &systemRenderable, ShaderProgramPtr &shader);
void project_lake(Viewer &viewer, DynamicSystemPtr &system, DynamicSystemRenderablePtr &systemRenderable, ShaderProgramPtr &shader);
void project_snowman(Viewer &viewer, DynamicSystemPtr &system, DynamicSystemRenderablePtr &systemRenderable, ShaderProgramPtr &shader);
void project_puck(Viewer &viewer, DynamicSystemPtr &system, DynamicSystemRenderablePtr &systemRenderable, ShaderProgramPtr &shader);
void project_spring_gate(Viewer &viewer, DynamicSystemPtr &system, DynamicSystemRenderablePtr &systemRenderable, ShaderProgramPtr &shader);

void initialize_project_scene(Viewer &viewer)
{
    //Set up a shader and add a 3D frame.
    ShaderProgramPtr flatShader =
        std::make_shared<ShaderProgram>("../shaders/flatVertex.glsl",
                                        "../shaders/flatFragment.glsl");
    viewer.addShaderProgram(flatShader);

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
        glm::lookAt(glm::vec3(0, 2, 2), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1)));

    //Define a directional light for the whole scene
    glm::vec3 d_direction = glm::normalize(glm::vec3(0.0,0.0,-1.0));
    glm::vec3 d_ambient(1.0,1.0,1.0), d_diffuse(1.0,1.0,0.975), d_specular(1.0,1.0,1.0);
    DirectionalLightPtr directionalLight = std::make_shared<DirectionalLight>(d_direction, d_ambient, d_diffuse, d_specular);
    viewer.setDirectionalLight(directionalLight);

    //Populate the dynamic system with particles, forcefields
    //and create renderables associated to them for visualization.
    project_terrain(viewer, system, systemRenderable, flatShader);
    project_lake(viewer, system, systemRenderable, flatShader);
    project_snowman(viewer, system, systemRenderable, flatShader);
    project_puck(viewer, system, systemRenderable, flatShader);
    project_spring_gate(viewer, system, systemRenderable, flatShader);

    //Initialize a force field that apply to all the particles of the system to simulate gravity
    //Add it to the system as a force field
    ConstantForceFieldPtr gravityForceField = std::make_shared<ConstantForceField>(system->getParticles(), glm::vec3{0, 0, -10});
    system->addForceField(gravityForceField);

    //Finally, run the animation
    viewer.startAnimation();
}

// void project_terrain(Viewer &viewer, ShaderProgramPtr &shader) {
//   std::shared_ptr<teachers::MeshRenderable> terrainMesh
//       = std::make_shared<teachers::MeshRenderable>(shader, "../meshes/suzanne.obj");
//   terrainMesh->setModelMatrix( glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, -3.0)));
//   viewer.addRenderable(terrainMesh);
// }

void project_terrain(Viewer &viewer, DynamicSystemPtr &system, DynamicSystemRenderablePtr &systemRenderable, ShaderProgramPtr &shader)
{
    TerrainRenderablePtr terrain = std::make_shared<TerrainRenderable>(shader);


    HierarchicalRenderable::addChild(systemRenderable, terrain);


    // GLfloat vertices[] = {
    //   -10, -10, 0,
    //   0, -10, 0,
    //   0, 0, 0,
    //   -10, 0, 0
    //   // ,
    //   // -1, 0, 0,
    //   // 0, -1, 0
    // };
    //
    // glEnableClientState(GL_VERTEX_ARRAY);
    // glVertexPointer(4, GL_FLOAT, 0, vertices);
    // glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    // glDisableClientState(GL_VERTEX_ARRAY);

    // unsigned int terrain_pBuffer;
    //
    // glGenBuffers(1, &terrain_pBuffer);
    // glBindBuffer(GL_ARRAY_BUFFER, terrain_pBuffer);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) & sizeof(int), vertices, GL_STATIC_DRAW);
    // glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
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
    ShaderProgramPtr texShader
        = std::make_shared<ShaderProgram>("../shaders/textureVertex.glsl",
                                          "../shaders/textureFragment.glsl");
    viewer.addShaderProgram(texShader);

    //Textured plane
    std::string filename = "../textures/ice_lake.jpg";
    TexturedPlaneRenderablePtr texPlane = std::make_shared<TexturedPlaneRenderable>(texShader, filename);
    texPlane->setParentTransform(glm::scale(glm::mat4(1.0), glm::vec3(10.0,10.0,10.0)));
    texPlane->setMaterial(Material::White());
    HierarchicalRenderable::addChild(systemRenderable, texPlane);
}

void project_snowman(Viewer &viewer, DynamicSystemPtr &system, DynamicSystemRenderablePtr &systemRenderable, ShaderProgramPtr &shader)
{
    SphereRenderablePtr bottomBall = std::make_shared<SphereRenderable>(shader, glm::vec4(0.9), 20, 20);
    SphereRenderablePtr middleBall = std::make_shared<SphereRenderable>(shader, glm::vec4(0.9), 20, 20);
    SphereRenderablePtr topBall = std::make_shared<SphereRenderable>(shader, glm::vec4(0.9), 20, 20);

    bottomBall->setLocalTransform(
        glm::scale(glm::mat4(1.0), glm::vec3(3, 3, 3)) *
        bottomBall->getModelMatrix());

    middleBall->setLocalTransform(
        glm::translate(glm::mat4(1.0), glm::vec3(0, 0, 3)) *
        glm::scale(glm::mat4(1.0), glm::vec3(2, 2, 2)) *
        middleBall->getModelMatrix());

    topBall->setLocalTransform(
        glm::translate(glm::mat4(1.0), glm::vec3(0, 0, 5)) *
        topBall->getModelMatrix());

    bottomBall->setParentTransform(
        glm::translate(glm::mat4(1.0), glm::vec3(-5, 0, 3)));

    HierarchicalRenderable::addChild(bottomBall, middleBall);
    HierarchicalRenderable::addChild(bottomBall, topBall);
    HierarchicalRenderable::addChild(systemRenderable, bottomBall);
}

void project_puck(Viewer &viewer, DynamicSystemPtr &system, DynamicSystemRenderablePtr &systemRenderable, ShaderProgramPtr &shader)
{
    // puck particle
    glm::vec3 px = glm::vec3(0.5, 0.0, 3.0);
    glm::vec3 pv = glm::vec3(5.0, 0.0, 0.0);
    float pr = 0.4;
    float pm = 300.0;
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
    glm::vec3 origin(5, -2.5, 0.0), displacement(0.0, 0.0, 0.0);
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
