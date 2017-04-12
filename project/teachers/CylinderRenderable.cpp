#include "./CylinderRenderable.hpp"
#include "./../include/gl_helper.hpp"
#include "./../include/log.hpp"
#include "./../include/Utils.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <iostream>

teachers::CylinderRenderable::CylinderRenderable(ShaderProgramPtr shaderProgram,
    bool normalPerVertex, unsigned int nbSlices, glm::vec4 color)
  : HierarchicalRenderable(shaderProgram),
    m_normalPerVertex(normalPerVertex),
    m_pBuffer(0), m_cBuffer(0), m_nBuffer(0), m_iBuffer(0), color(color)
{
    if (m_normalPerVertex)
        buildNormalPerVertex(nbSlices);   // with indexing
    else
        buildNormalPerFace(nbSlices);

    //Generate buffers
    glcheck(glGenBuffers(1, &m_pBuffer));
    glcheck(glGenBuffers(1, &m_cBuffer));
    glcheck(glGenBuffers(1, &m_nBuffer));
    if (m_normalPerVertex)
        glcheck(glGenBuffers(1, &m_iBuffer)); //indices

    //Activate buffer and send data to the graphics card
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_positions.size()*sizeof(glm::vec3), m_positions.data(), GL_STATIC_DRAW));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_colors.size()*sizeof(glm::vec4), m_colors.data(), GL_STATIC_DRAW));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_nBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_normals.size()*sizeof(glm::vec3), m_normals.data(), GL_STATIC_DRAW));
    if (m_normalPerVertex) {
        glcheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iBuffer));
        glcheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size()*sizeof(glm::ivec3), m_indices.data(), GL_STATIC_DRAW));
    }
}

teachers::CylinderRenderable::~CylinderRenderable()
{
    glcheck(glDeleteBuffers(1, &m_pBuffer));
    glcheck(glDeleteBuffers(1, &m_cBuffer));
    glcheck(glDeleteBuffers(1, &m_nBuffer));
    if (m_normalPerVertex)
        glcheck(glDeleteBuffers(1, &m_iBuffer));
}

void teachers::CylinderRenderable::do_draw()
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

    if (m_normalPerVertex) {    // indexed
        glcheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iBuffer));
        glcheck(glDrawElements(GL_TRIANGLES, m_indices.size()*3, GL_UNSIGNED_INT, (void*)0));
    } else {
        glcheck(glDrawArrays(GL_TRIANGLES,0, m_positions.size()));
    }

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

void teachers::CylinderRenderable::do_animate(float dTime, float time)
{
}


void teachers::CylinderRenderable::buildNormalPerFace(unsigned int nbSlices)
{
    // basic cylinder: radius 1, along z axis, bases on z == 0 and z == 1
    // The normal on a given coord around z is the coord itself!
    //
    // NORMAL PER FACE: in this case, each slice of the cylinder has its
    // own normal (1 for the 2 triangles of a slice)

    glm::vec3 bottomCenter(0.0, 0.0, 0.0);
    glm::vec3 topCenter(0.0, 0.0, 1.0);

    double dTheta = 2.0 * M_PI / (double) nbSlices;
    double currentTheta = 0.0;
    double currentCos = 1.0;
    double currentSin = 0.0;

    for (unsigned int i = 0; i < nbSlices; ++i) {
        double nextCos = cos(currentTheta + dTheta);
        double nextSin = sin(currentTheta + dTheta);
        // normal: computed in the middle of the slice
        glm::vec3 normal(cos(currentTheta + dTheta / 2.0),
                         sin(currentTheta + dTheta / 2.0), 0.0);
        // let's color this cube with normal values
        // (since we do not have illumination yet, it will create a shaded effect)

        // first triangle along z
        m_positions.push_back(glm::vec3(currentCos, currentSin, 0.0));
        m_positions.push_back(glm::vec3(nextCos, nextSin, 1.0));
        m_positions.push_back(glm::vec3(nextCos, nextSin, 0.0));
        for (unsigned int i = 0; i < 3; i++) {
            m_colors.push_back(color != glm::vec4(-1.0) ? color : glm::vec4(normal, 1.0));
            m_normals.push_back(normal);
        }

        // second triangle along z
        m_positions.push_back(glm::vec3(currentCos, currentSin, 0.0));
        m_positions.push_back(glm::vec3(nextCos, nextSin, 1.0));
        m_positions.push_back(glm::vec3(currentCos, currentSin, 1.0));
        for (unsigned int i = 0; i < 3; i++) {
            m_colors.push_back(color != glm::vec4(-1.0) ? color : glm::vec4(normal, 1.0));
            m_normals.push_back(normal);
        }

        // bottom and top bases
        m_positions.push_back(bottomCenter);
        m_positions.push_back(glm::vec3(currentCos, currentSin, 0.0));
        m_positions.push_back(glm::vec3(nextCos, nextSin, 0.0));

        // glm::vec4 color((i%2==0) ? 0.0 : 1.0, 0.0, (i%2==0) ? 1.0 : 0.0, 1.0);
        for (unsigned int i = 0; i < 3; i++) {
            m_normals.push_back(glm::vec3(0.0, 0.0, -1.0));
            m_colors.push_back(color != glm::vec4(-1.0) ? color : glm::vec4(0.0, 0.0, -1.0, 1.0));
        }

        m_positions.push_back(topCenter);
        m_positions.push_back(glm::vec3(currentCos, currentSin, 1.0));
        m_positions.push_back(glm::vec3(nextCos, nextSin, 1.0));
        for (unsigned int i = 0; i < 3; i++) {
            m_normals.push_back(glm::vec3(0.0, 0.0, +1.0));
            m_colors.push_back(color != glm::vec4(-1.0) ? color : glm::vec4(0.0, 0.0, +1.0, 1.0));
        }

        currentTheta = currentTheta + dTheta;
        currentCos = nextCos;
        currentSin = nextSin;
    }
}


void teachers::CylinderRenderable::buildNormalPerVertex(unsigned int nbSlices)
{
    // basic cylinder: radius 1, along z axis, bases on z == 0 and z == 1
    // The normal on a given coord around z is the coord itself!
    //
    // NORMAL PER VERTEX: in this case each vertex has its own normal,
    // shared by adjacent triangles

    unsigned int nbVertices = nbSlices * 4;  // 2 on the trunk, 1 on each base
    nbVertices += 2;  // plus the center of each base
    m_positions.resize(nbVertices, glm::vec3(0.0, 0.0, 0.0));
    m_normals.resize(nbVertices, glm::vec3(0.0, 0.0, 0.0));
    m_colors.resize(nbVertices, color != glm::vec4(-1.0) ? color : glm::vec4(0.0, 0.0, 0.0, 1.0));

    // centers of each base
    unsigned int iBottom = nbVertices - 2;
    unsigned int iTop = nbVertices - 1;
    glm::vec3 bottomNormal(0.0, 0.0, -1.0);
    glm::vec3 topNormal(0.0, 0.0, +1.0);

    m_positions[iBottom] = glm::vec3(0.0, 0.0, 0.0);
    m_positions[iTop]    = glm::vec3(0.0, 0.0, 1.0);
    m_normals[iBottom] = bottomNormal;
    m_normals[iTop]    = topNormal;
    m_colors[iBottom] = color != glm::vec4(-1.0) ? color : glm::vec4(bottomNormal, 1.0);
    m_colors[iTop]    = color != glm::vec4(-1.0) ? color : glm::vec4(topNormal, 1.0);

    // let's round the bases
    double dTheta = 2.0 * M_PI / (double) nbSlices;
    unsigned int n = nbSlices;
    for (unsigned int i = 0; i < nbSlices; ++i) {
        double currentCos = cos(i * dTheta);
        double currentSin = sin(i * dTheta);

        m_positions[i]       = glm::vec3(currentCos, currentSin, 0.0);
        m_positions[i + n]   = glm::vec3(currentCos, currentSin, 1.0);
        m_positions[i + 2*n] = glm::vec3(currentCos, currentSin, 0.0);
        m_positions[i + 3*n] = glm::vec3(currentCos, currentSin, 1.0);

        m_normals[i]       = glm::vec3(currentCos, currentSin, 0.0);
        m_normals[i + n]   = glm::vec3(currentCos, currentSin, 1.0);
        m_normals[i + 2*n] = glm::vec3(0.0, 0.0, -1.0);
        m_normals[i + 3*n] = glm::vec3(0.0, 0.0, +1.0);

        // let's color this cube with normal values
        // (since we do not have illumination yet, it will create a shaded effect)
        m_colors[i]       = color != glm::vec4(-1.0) ? color : glm::vec4(currentCos, currentSin, 0.0, 1.0);
        m_colors[i + n]   = color != glm::vec4(-1.0) ? color : glm::vec4(currentCos, currentSin, 0.0, 1.0);
        m_colors[i + 2*n] = color != glm::vec4(-1.0) ? color : glm::vec4(0.0, 0.0, -1.0, 1.0); // black!
        m_colors[i + 3*n] = color != glm::vec4(-1.0) ? color : glm::vec4(0.0, 0.0, 1.0, 1.0);

        // triangles: 4 on each slice (2 on the trunk, 1 on each base)
        m_indices.push_back(glm::ivec3(i,         (i + 1)%n,     i + n));
        m_indices.push_back(glm::ivec3((i + 1)%n, (i + 1)%n + n, i + n));
        m_indices.push_back(glm::ivec3(i + 2*n, iBottom,         (i + 1)%n + 2*n));
        m_indices.push_back(glm::ivec3(i + 3*n, (i + 1)%n + 3*n, iTop));
    }
}
