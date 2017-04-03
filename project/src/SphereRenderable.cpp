#include "../include/SphereRenderable.hpp"
#include "../include/gl_helper.hpp"
#include "../teachers/Geometries.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <iostream>

SphereRenderable::SphereRenderable(
    ShaderProgramPtr shaderProgram, glm::vec4 color, int strips, int slices)
    : HierarchicalRenderable(shaderProgram),
      m_pBuffer(0), m_nBuffer(0), m_cBuffer(0)
{
    //Initialize geometry
    teachers::getUnitSphere(m_positions, m_normals, strips, slices);

    m_colors.resize(m_positions.size(), color);

    //Create buffers
    glcheck(glGenBuffers(1, &m_pBuffer)); //vertices
    glcheck(glGenBuffers(1, &m_nBuffer)); //normals
    glcheck(glGenBuffers(1, &m_cBuffer)); //colors

    //Activate buffer and send data to the graphics card
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_positions.size() * sizeof(glm::vec3), m_positions.data(), GL_STATIC_DRAW));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_nBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_normals.size() * sizeof(glm::vec3), m_normals.data(), GL_STATIC_DRAW));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_colors.size() * sizeof(glm::vec4), m_colors.data(), GL_STATIC_DRAW));
}

SphereRenderable::~SphereRenderable()
{
    glcheck(glDeleteBuffers(1, &m_pBuffer));
    glcheck(glDeleteBuffers(1, &m_nBuffer));
    glcheck(glDeleteBuffers(1, &m_cBuffer));
}

void SphereRenderable::do_draw()
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

    //Send vertex attributes to GPU
    if (positionLocation != ShaderProgram::null_location)
    {
        glcheck(glEnableVertexAttribArray(positionLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer));
        glcheck(glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, (void *)0));
    }
    if (normalLocation != ShaderProgram::null_location)
    {
        glcheck(glEnableVertexAttribArray(normalLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_nBuffer));
        glcheck(glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 0, (void *)0));
    }
    if (colorLocation != ShaderProgram::null_location)
    {
        glcheck(glEnableVertexAttribArray(colorLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer));
        glcheck(glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, 0, (void *)0));
    }

    //Draw triangles elements
    glcheck(glDrawArrays(GL_TRIANGLES, 0, m_positions.size()));

    //Release everything
    if (positionLocation != ShaderProgram::null_location)
    {
        glcheck(glDisableVertexAttribArray(positionLocation));
    }
    if (normalLocation != ShaderProgram::null_location)
    {
        glcheck(glDisableVertexAttribArray(normalLocation));
    }
    if (colorLocation != ShaderProgram::null_location)
    {
        glcheck(glDisableVertexAttribArray(colorLocation));
    }
}

void SphereRenderable::do_animate(float time)
{
}