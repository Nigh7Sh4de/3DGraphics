#include "../../include/dynamics_rendering/TerrainRenderable.hpp"
#include "../../include/gl_helper.hpp"
#include "../../include/log.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <iostream>


TerrainRenderable::~TerrainRenderable()
{
    glcheck(glDeleteBuffers(1, &m_pBuffer));
    glcheck(glDeleteBuffers(1, &m_cBuffer));
    glcheck(glDeleteBuffers(1, &m_nBuffer));
}

float calculateTerrainHeight(int x, int y) {
  return (x*x / 4.0) + ((y-0.5)*(y-0.5) / 8.0); //P[x][y]
}

glm::vec3 calculateTerrainNormal(int x, int y) {
  return glm::normalize(glm::vec3((x / 2.0), ((y-0.5) / 4.0), 1)); //P[x][y]
}

TerrainRenderable::TerrainRenderable(ShaderProgramPtr shaderProgram) :
    HierarchicalRenderable(shaderProgram),
    m_pBuffer(0), m_cBuffer(0), m_nBuffer(0)
{
    int scale = 20;
    for (int i=0;i<111-3;i+=6) {
      int p = i/6;
      int x1 = 0;
      int x2 = 0;
      int y1 = 0;
      int y2 = 0;
      if (p%9<5) {
        x1 = x2 = p % 9;
        y1 = (p/9)*2;
        y2 = (p/9)*2+1;
      }
      else {
        x1 = 9 - (p%9);
        x2 = 9 - (p%9) - 1;
        y1 = (p/9)*2+2;
        y2 = (p/9)*2+1;
      }
      x1 -= 2;
      y1 -= 2;
      x2 -= 2;
      y2 -= 2;
      float z1 = calculateTerrainHeight(x1, y1) - 1; //P[x][y]
      float z2 = calculateTerrainHeight(x2, y2) - 1; //P[x][y]
      std::cout<<"Building vertices"<<p*2<<": "<<x1<<" "<<y1<<" "<<z1<<std::endl;
      std::cout<<"Building vertices"<<p*2<<" scaled: "<<x1*scale<<" "<<y1*scale<<" "<<z1*scale<<std::endl;
      m_positions[i] = scale * (x1);
      m_positions[i+1] = scale * (y1);
      m_positions[i+2] = scale * (z1);
      std::cout<<"Building vertices"<<p*2+1<<" "<<x2<<" "<<y2<<" "<<z2<<std::endl;
      m_positions[i+3] = scale * (x2);
      m_positions[i+4] = scale * (y2);
      m_positions[i+5] = scale * (z2);
    }
    std::cout<<"Building vertices"<<19<<"* "<<0<<" "<<4<<" "<<1<<std::endl;
    m_positions[108] = scale * -2;
    m_positions[109] = scale * 2;
    m_positions[110] = scale * (calculateTerrainHeight(-2, 2));
    // m_positions = {
    //   -10, -10, 0,
    //   0, -10, 0,
    //   0, 0, 0
    // };
    // m_positions.push_back(p4);
    //
    // glm::vec3 normal(0, 1, 0);
    for (int i=0;i<111;i+=3) {
      glm::vec3 normal = calculateTerrainNormal(m_positions[i], m_positions[i+1]);
      m_normals[i] = normal.x;
      m_normals[i+1] = normal.y;
      m_normals[i+2] = normal.z;
    }
    // m_normals.push_back(normal);
    // m_normals.push_back(normal);
    // m_normals.push_back(normal);
    // m_normals.push_back(normal);
    // m_normals.push_back(normal);
    //
    for (int i=0, j=0;i<148;i+=4,j+=3) {
      // std::cout<<"Building color "<<i<<" "<<0<<" "<<255-i<<" "<<1<<std::endl;
      float z = m_positions[j+2];
      float color = 0;
      if (z > 0) {
        color = z / scale;
      }
      std::cout<<"Building color at "<<z<<std::endl;
      m_colors[i] = 1.2*color;
      m_colors[i+1] = 1.1*color;
      m_colors[i+2] = color;
      m_colors[i+3] = 1;
    }
    // m_colors.push_back(color);
    // m_colors.push_back(color);
    // m_colors.push_back(color);
    // m_colors.push_back(color);
    // m_colors.push_back(color);
    // m_colors.push_back(color);

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
    glcheck(glDrawArrays(GL_TRIANGLE_STRIP,0, 37));

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
