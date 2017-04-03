#include "./CubeRenderable.hpp"
#include "./../include/gl_helper.hpp"
#include "./../include/log.hpp"
#include "./../include/Utils.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <iostream>

teachers::CubeRenderable::CubeRenderable(ShaderProgramPtr shaderProgram)
  : HierarchicalRenderable(shaderProgram),
    m_pBuffer(0), m_cBuffer(0), m_nBuffer(0)
{
    std::cerr << "Hey, teacher, leave them kids alone! -- Cube" << std::endl;

    m_positions.push_back(glm::vec3(-0.5, -0.5, -0.5));
    m_positions.push_back(glm::vec3(0.5, 0.5, -0.5));
    m_positions.push_back(glm::vec3(0.5, -0.5, -0.5));
    m_normals.push_back(glm::vec3(0.0,0.0,-1.0));
    m_normals.push_back(glm::vec3(0.0,0.0,-1.0));
    m_normals.push_back(glm::vec3(0.0,0.0,-1.0));

    m_positions.push_back(glm::vec3(-0.5, -0.5, -0.5));
    m_positions.push_back(glm::vec3(-0.5, 0.5, -0.5));
    m_positions.push_back(glm::vec3(0.5, 0.5, -0.5));
    m_normals.push_back(glm::vec3(0.0,0.0,-1.0));
    m_normals.push_back(glm::vec3(0.0,0.0,-1.0));
    m_normals.push_back(glm::vec3(0.0,0.0,-1.0));

    m_positions.push_back(glm::vec3(0.5, -0.5, -0.5));
    m_positions.push_back(glm::vec3(0.5, 0.5, 0.5));
    m_positions.push_back(glm::vec3(0.5, -0.5, 0.5));
    m_normals.push_back(glm::vec3(1.0,0.0,0.0));
    m_normals.push_back(glm::vec3(1.0,0.0,0.0));
    m_normals.push_back(glm::vec3(1.0,0.0,0.0));

    m_positions.push_back(glm::vec3(0.5, -0.5, -0.5));
    m_positions.push_back(glm::vec3(0.5, 0.5, -0.5));
    m_positions.push_back(glm::vec3(0.5, 0.5, 0.5));
    m_normals.push_back(glm::vec3(1.0,0.0,0.0));
    m_normals.push_back(glm::vec3(1.0,0.0,0.0));
    m_normals.push_back(glm::vec3(1.0,0.0,0.0));

    m_positions.push_back(glm::vec3(-0.5, 0.5, 0.5));
    m_positions.push_back(glm::vec3(-0.5, -0.5, 0.5));
    m_positions.push_back(glm::vec3(0.5, -0.5, 0.5));
    m_normals.push_back(glm::vec3(0.0,0.0,1.0));
    m_normals.push_back(glm::vec3(0.0,0.0,1.0));
    m_normals.push_back(glm::vec3(0.0,0.0,1.0));

    m_positions.push_back(glm::vec3(-0.5, 0.5, 0.5));
    m_positions.push_back(glm::vec3(0.5, -0.5, 0.5));
    m_positions.push_back(glm::vec3(0.5, 0.5, 0.5));
    m_normals.push_back(glm::vec3(0.0,0.0,1.0));
    m_normals.push_back(glm::vec3(0.0,0.0,1.0));
    m_normals.push_back(glm::vec3(0.0,0.0,1.0));

    m_positions.push_back(glm::vec3(-0.5, -0.5, 0.5));
    m_positions.push_back(glm::vec3(-0.5, 0.5, -0.5));
    m_positions.push_back(glm::vec3(-0.5, -0.5, -0.5));
    m_normals.push_back(glm::vec3(-1.0,0.0,0.0));
    m_normals.push_back(glm::vec3(-1.0,0.0,0.0));
    m_normals.push_back(glm::vec3(-1.0,0.0,0.0));

    m_positions.push_back(glm::vec3(-0.5, -0.5, 0.5));
    m_positions.push_back(glm::vec3(-0.5, 0.5, 0.5));
    m_positions.push_back(glm::vec3(-0.5, 0.5, -0.5));
    m_normals.push_back(glm::vec3(-1.0,0.0,0.0));
    m_normals.push_back(glm::vec3(-1.0,0.0,0.0));
    m_normals.push_back(glm::vec3(-1.0,0.0,0.0));

    m_positions.push_back(glm::vec3(-0.5, 0.5, -0.5));
    m_positions.push_back(glm::vec3(0.5, 0.5, 0.5));
    m_positions.push_back(glm::vec3(0.5, 0.5, -0.5));
    m_normals.push_back(glm::vec3(0.0,1.0,0.0));
    m_normals.push_back(glm::vec3(0.0,1.0,0.0));
    m_normals.push_back(glm::vec3(0.0,1.0,0.0));

    m_positions.push_back(glm::vec3(-0.5, 0.5, -0.5));
    m_positions.push_back(glm::vec3(-0.5, 0.5, 0.5));
    m_positions.push_back(glm::vec3(0.5, 0.5, 0.5));
    m_normals.push_back(glm::vec3(0.0,1.0,0.0));
    m_normals.push_back(glm::vec3(0.0,1.0,0.0));
    m_normals.push_back(glm::vec3(0.0,1.0,0.0));

    m_positions.push_back(glm::vec3(-0.5, -0.5, 0.5));
    m_positions.push_back(glm::vec3(-0.5, -0.5, -0.5));
    m_positions.push_back(glm::vec3(0.5, -0.5, -0.5));
    m_normals.push_back(glm::vec3(0.0,-1.0,0.0));
    m_normals.push_back(glm::vec3(0.0,-1.0,0.0));
    m_normals.push_back(glm::vec3(0.0,-1.0,0.0));

    m_positions.push_back(glm::vec3(-0.5, -0.5, 0.5));
    m_positions.push_back(glm::vec3(0.5, -0.5, -0.5));
    m_positions.push_back(glm::vec3(0.5, -0.5, 0.5));
    m_normals.push_back(glm::vec3(0.0,-1.0,0.0));
    m_normals.push_back(glm::vec3(0.0,-1.0,0.0));
    m_normals.push_back(glm::vec3(0.0,-1.0,0.0));


    //Assign one color to each of the 12 triangles, 3 vertices each
    for (unsigned int i = 0; i < 12; i++) {
        glm::vec4 color = randomColor();
        m_colors.push_back(color);
        m_colors.push_back(color);
        m_colors.push_back(color);
    }

    //Create buffers
    glcheck(glGenBuffers(1, &m_pBuffer));  // positions
    glcheck(glGenBuffers(1, &m_cBuffer));  // colors
    glcheck(glGenBuffers(1, &m_nBuffer));  // normals

    //Activate buffer and send data to the graphics card
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_positions.size()*sizeof(glm::vec3), m_positions.data(), GL_STATIC_DRAW));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_colors.size()*sizeof(glm::vec4), m_colors.data(), GL_STATIC_DRAW));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_nBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_normals.size()*sizeof(glm::vec3), m_normals.data(), GL_STATIC_DRAW));
}

teachers::CubeRenderable::~CubeRenderable()
{
    glcheck(glDeleteBuffers(1, &m_pBuffer));
    glcheck(glDeleteBuffers(1, &m_cBuffer));
    glcheck(glDeleteBuffers(1, &m_nBuffer));
}

void teachers::CubeRenderable::do_draw()
{
    //Location
    int modelLocation = m_shaderProgram->getUniformLocation("modelMat");
    int positionLocation = m_shaderProgram->getAttributeLocation("vPosition");
    int colorLocation = m_shaderProgram->getAttributeLocation("vColor");
    int normalLocation = m_shaderProgram->getAttributeLocation("vNormal");

    //Send data to GPU
    if (modelLocation != ShaderProgram::null_location) {
        glcheck(glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(getModelMatrix())));
    }

    if (positionLocation != ShaderProgram::null_location) {
        //Activate location
        glcheck(glEnableVertexAttribArray(positionLocation));
        //Bind buffer
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer));
        //Specify internal format
        glcheck(glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }

    if (colorLocation != ShaderProgram::null_location) {
        glcheck(glEnableVertexAttribArray(colorLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer));
        glcheck(glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }

    if (normalLocation != ShaderProgram::null_location) {
        glcheck(glEnableVertexAttribArray(normalLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_nBuffer));
        glcheck(glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }

    //Draw triangles elements
    glcheck(glDrawArrays(GL_TRIANGLES,0, m_positions.size()));

    if (positionLocation != ShaderProgram::null_location) {
        glcheck(glDisableVertexAttribArray(positionLocation));
    }
    if (colorLocation != ShaderProgram::null_location) {
        glcheck(glDisableVertexAttribArray(colorLocation));
    }
    if (normalLocation != ShaderProgram::null_location) {
        glcheck(glDisableVertexAttribArray(normalLocation));
    }
}

void teachers::CubeRenderable::do_animate(float time)
{
}
