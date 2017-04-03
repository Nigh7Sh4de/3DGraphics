#include "./IndexedCubeRenderable.hpp"
#include "./../include/gl_helper.hpp"
#include "./../include/log.hpp"
#include "./../include/Utils.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <iostream>

teachers::IndexedCubeRenderable::IndexedCubeRenderable(ShaderProgramPtr shaderProgram)
  : HierarchicalRenderable(shaderProgram),
    m_pBuffer(0), m_cBuffer(0), m_nBuffer(0), m_iBuffer(0)
{
    std::cerr << "Hey, teacher, leave them kids alone! -- Indexed cube" << std::endl;

    // 8 vertices
    m_positions.push_back(glm::vec3(-0.5, -0.5, -0.5));
    m_positions.push_back(glm::vec3(0.5, -0.5, -0.5));
    m_positions.push_back(glm::vec3(0.5, 0.5, -0.5));
    m_positions.push_back(glm::vec3(-0.5, 0.5, -0.5));
    m_positions.push_back(glm::vec3(-0.5, -0.5, 0.5));
    m_positions.push_back(glm::vec3(-0.5, 0.5, 0.5));
    m_positions.push_back(glm::vec3(0.5, 0.5, 0.5));
    m_positions.push_back(glm::vec3(0.5, -0.5, 0.5));

    // 8 normals (== vertex coordinates, since the cube is centered)
    for(const glm::vec3& x : m_positions) {
        m_normals.push_back(glm::normalize(x));
    }

    // 12 triangles, 3 indices each
    m_indices.push_back(glm::ivec3(0, 2, 1));
    m_indices.push_back(glm::ivec3(0, 3, 2));
    m_indices.push_back(glm::ivec3(1, 6, 7));
    m_indices.push_back(glm::ivec3(1, 2, 6));
    m_indices.push_back(glm::ivec3(5, 4, 7));
    m_indices.push_back(glm::ivec3(5, 7, 6));
    m_indices.push_back(glm::ivec3(4, 3, 0));
    m_indices.push_back(glm::ivec3(4, 5, 3));
    m_indices.push_back(glm::ivec3(3, 6, 2));
    m_indices.push_back(glm::ivec3(3, 5, 6));
    m_indices.push_back(glm::ivec3(4, 0, 1));
    m_indices.push_back(glm::ivec3(4, 1, 7));

    //Assign one color to each of the 12 triangles
    m_colors.push_back(glm::vec4(1,0,0,1));
    m_colors.push_back(glm::vec4(0,1,0,1));
    m_colors.push_back(glm::vec4(0,0,1,1));
    m_colors.push_back(glm::vec4(0,1,1,1));
    m_colors.push_back(glm::vec4(1,0,0,1));
    m_colors.push_back(glm::vec4(0,1,0,1));
    m_colors.push_back(glm::vec4(0,0,1,1));
    m_colors.push_back(glm::vec4(0,1,1,1));


    //Create buffers
    glGenBuffers(1, &m_pBuffer); //vertices
    glGenBuffers(1, &m_cBuffer); //colors
    glGenBuffers(1, &m_nBuffer); //normals
    glGenBuffers(1, &m_iBuffer); //indices

    //Activate buffer and send data to the graphics card
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_positions.size()*sizeof(glm::vec3), m_positions.data(), GL_STATIC_DRAW));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_colors.size()*sizeof(glm::vec4), m_colors.data(), GL_STATIC_DRAW));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_nBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_normals.size()*sizeof(glm::vec3), m_normals.data(), GL_STATIC_DRAW));
    glcheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iBuffer));
    glcheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size()*sizeof(glm::ivec3), m_indices.data(), GL_STATIC_DRAW));
}

void teachers::IndexedCubeRenderable::do_draw()
{
    //Location
    int positionLocation = m_shaderProgram->getAttributeLocation("vPosition");
    int colorLocation = m_shaderProgram->getAttributeLocation("vColor");
    int normalLocation = m_shaderProgram->getAttributeLocation("vNormal");
    int modelLocation = m_shaderProgram->getUniformLocation("modelMat");

    //Send data to GPU
    if(modelLocation != ShaderProgram::null_location) {
        glcheck(glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(getModelMatrix())));
    }

    if(positionLocation != ShaderProgram::null_location) {
        //Activate location
        glcheck(glEnableVertexAttribArray(positionLocation));
        //Bind buffer
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer));
        //Specify internal format
        glcheck(glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }

    if(colorLocation != ShaderProgram::null_location) {
        glcheck(glEnableVertexAttribArray(colorLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer));
        glcheck(glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }

    if(normalLocation != ShaderProgram::null_location) {
        glcheck(glEnableVertexAttribArray(normalLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_nBuffer));
        glcheck(glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }

    //Draw triangles elements
    glcheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iBuffer));
    glcheck(glDrawElements(GL_TRIANGLES, m_indices.size()*3, GL_UNSIGNED_INT, (void*)0));

    if(positionLocation != ShaderProgram::null_location) {
        glcheck(glDisableVertexAttribArray(positionLocation));
    }
    if(colorLocation != ShaderProgram::null_location) {
        glcheck(glDisableVertexAttribArray(colorLocation));
    }
    if(normalLocation != ShaderProgram::null_location) {
        glcheck(glDisableVertexAttribArray(normalLocation));
    }
}

void teachers::IndexedCubeRenderable::do_animate(float time)
{
}

teachers::IndexedCubeRenderable::~IndexedCubeRenderable()
{
    glcheck(glDeleteBuffers(1, &m_pBuffer));
    glcheck(glDeleteBuffers(1, &m_cBuffer));
    glcheck(glDeleteBuffers(1, &m_nBuffer));
    glcheck(glDeleteBuffers(1, &m_iBuffer));
}
