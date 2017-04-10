#include "../../include/dynamics_rendering/TerrainRenderable.hpp"
#include "../../include/gl_helper.hpp"
#include "../../include/log.hpp"

// #include <glm/gtc/matrix_scaleform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <iostream>


TerrainRenderable::~TerrainRenderable()
{
    glcheck(glDeleteBuffers(1, &m_pBuffer));
    glcheck(glDeleteBuffers(1, &m_cBuffer));
    glcheck(glDeleteBuffers(1, &m_nBuffer));
}

float TerrainRenderable::calculateTerrainHeight(int x, int y) {
  return ((x*x / 4.0) + ((y-0.5)*(y-0.5) / 8.0) - 2.0) * glm::log(detail_s) ; //P[x][y]
}

glm::vec3 TerrainRenderable::calculateTerrainNormal(int x, int y) {
  return glm::normalize(glm::vec3((x / 2.0), ((y-0.5) / 4.0), 1)); //P[x][y]
}

TerrainRenderable::TerrainRenderable(ShaderProgramPtr shaderProgram) :
    HierarchicalRenderable(shaderProgram),
    m_pBuffer(0), m_cBuffer(0), m_nBuffer(0)
{

    // std::cout<<"Using detail (d,v,g,t) "<<detail<<", "<<detail_v<<", "<<detail_g<<", "<<detail_t<<std::endl;
    int i=0;
    for (i=0;i<detail_v*3-3;i+=6) {
      int p = i/6;
      int x1 = 0;
      int x2 = 0;
      int y1 = 0;
      int y2 = 0;
      if (p%detail_g<detail) {
        // std::cout<<"GOING RIGHT"<<std::endl;
        x1 = x2 = p % detail_g;
        y1 = (p/detail_g)*2;
        y2 = (p/detail_g)*2+1;
      }
      else {
        // std::cout<<"GOING LEFT"<<std::endl;
        x1 = detail_g - (p%detail_g);
        x2 = detail_g - (p%detail_g) - 1;
        y1 = (p/detail_g)*2+2;
        y2 = (p/detail_g)*2+1;
      }
      x1 -= detail_t;
      y1 -= detail_t;
      x2 -= detail_t;
      y2 -= detail_t;
      // std::cout<<"Building vertices"<<p*2<<": "<<x1<<" "<<y1<<" "<<z1<<std::endl;
      // std::cout<<"Building vertices"<<p*2<<" scaled: "<<x1*detail_s<<" "<<y1*detail_s<<" "<<z1*detail_s<<std::endl;
      m_positions[i] = detail_s * (x1);
      m_positions[i+1] = detail_s * (y1);
      m_positions[i+2] = calculateTerrainHeight(x1, y1);
      // std::cout<<"Building vertices"<<p*2+1<<" "<<x2<<" "<<y2<<" "<<z2<<std::endl;
      m_positions[i+3] = detail_s * (x2);
      m_positions[i+4] = detail_s * (y2);
      m_positions[i+5] = calculateTerrainHeight(x2, y2);;
    }
    // std::cout<<"Building vertices"<<19<<"* "<<0<<" "<<4<<" "<<1<<std::endl;
    m_positions[i] = detail_s * -(detail_t);
    m_positions[i+1] = detail_s * (detail_t);
    m_positions[i+2] = calculateTerrainHeight(-detail_t, detail_t);

    for (i=0;i<detail_v*3;i+=3) {
      glm::vec3 normal = calculateTerrainNormal(m_positions[i], m_positions[i+1]);
      m_normals[i] = normal.x;
      m_normals[i+1] = normal.y;
      m_normals[i+2] = normal.z;
    }

    int j;
    for (i=0, j=0;i<detail_v*4;i+=4,j+=3) {
      // std::cout<<"Building color "<<i<<" "<<0<<" "<<255-i<<" "<<1<<std::endl;
      float z = m_positions[j+2];
      float color = 0;
      if (z > 0) {
        color = z / glm::log(detail_s) * 0.1;
      }

      // std::cout<<"Building color at "<<z<<" "<<color<<std::endl;
      m_colors[i] = 1.05*color;
      m_colors[i+1] = 1.01*color;
      m_colors[i+2] = color;
      m_colors[i+3] = 1;
    }

    //Create buffers
    glGenBuffers(1, &m_pBuffer); //vertices
    glGenBuffers(1, &m_cBuffer); //colors
    glGenBuffers(1, &m_nBuffer); //normals

    //Activate buffer and send data to the graphics card
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, sizeof(m_positions), m_positions, GL_STATIC_DRAW));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, sizeof(m_colors), m_colors, GL_STATIC_DRAW));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_nBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, sizeof(m_normals), m_normals, GL_STATIC_DRAW));
}

void TerrainRenderable::do_draw()
{
    //Draw geometric data
    int positionLocation = m_shaderProgram->getAttributeLocation("vPosition");
    int colorLocation = m_shaderProgram->getAttributeLocation("vColor");
    int normalLocation = m_shaderProgram->getAttributeLocation("vNormal");
    int modelLocation = m_shaderProgram->getUniformLocation("modelMat");

    if (modelLocation != ShaderProgram::null_location) {
        glcheck(glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(getModelMatrix())));
    }

    if (positionLocation != ShaderProgram::null_location) {
        glcheck(glEnableVertexAttribArray(positionLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer));
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
    glcheck(glDrawArrays(GL_TRIANGLE_STRIP,0, detail_v));

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

void TerrainRenderable::do_animate(float time)
{}
