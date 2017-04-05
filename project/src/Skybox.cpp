#include "./../include/Skybox.hpp"
#include "./../include/gl_helper.hpp"
#include "./../include/log.hpp"
#include "./../include/Utils.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <iostream>
#include <sstream>

Skybox::Skybox(ShaderProgramPtr shaderProgram) : HierarchicalRenderable(shaderProgram)
{
    float scale = 100;

    GLfloat skyboxVertices[] = {
        // Positions
        -1.0f * scale, 1.0f * scale, -1.0f * scale,
        -1.0f * scale, -1.0f * scale, -1.0f * scale,
        1.0f * scale, -1.0f * scale, -1.0f * scale,
        1.0f * scale, -1.0f * scale, -1.0f * scale,
        1.0f * scale, 1.0f * scale, -1.0f * scale,
        -1.0f * scale, 1.0f * scale, -1.0f * scale,

        -1.0f * scale, -1.0f * scale, 1.0f * scale,
        -1.0f * scale, -1.0f * scale, -1.0f * scale,
        -1.0f * scale, 1.0f * scale, -1.0f * scale,
        -1.0f * scale, 1.0f * scale, -1.0f * scale,
        -1.0f * scale, 1.0f * scale, 1.0f * scale,
        -1.0f * scale, -1.0f * scale, 1.0f * scale,

        1.0f * scale, -1.0f * scale, -1.0f * scale,
        1.0f * scale, -1.0f * scale, 1.0f * scale,
        1.0f * scale, 1.0f * scale, 1.0f * scale,
        1.0f * scale, 1.0f * scale, 1.0f * scale,
        1.0f * scale, 1.0f * scale, -1.0f * scale,
        1.0f * scale, -1.0f * scale, -1.0f * scale,

        -1.0f * scale, -1.0f * scale, 1.0f * scale,
        -1.0f * scale, 1.0f * scale, 1.0f * scale,
        1.0f * scale, 1.0f * scale, 1.0f * scale,
        1.0f * scale, 1.0f * scale, 1.0f * scale,
        1.0f * scale, -1.0f * scale, 1.0f * scale,
        -1.0f * scale, -1.0f * scale, 1.0f * scale,

        -1.0f * scale, 1.0f * scale, -1.0f * scale,
        1.0f * scale, 1.0f * scale, -1.0f * scale,
        1.0f * scale, 1.0f * scale, 1.0f * scale,
        1.0f * scale, 1.0f * scale, 1.0f * scale,
        -1.0f * scale, 1.0f * scale, 1.0f * scale,
        -1.0f * scale, 1.0f * scale, -1.0f * scale,

        -1.0f * scale, -1.0f * scale, -1.0f * scale,
        -1.0f * scale, -1.0f * scale, 1.0f * scale,
        1.0f * scale, -1.0f * scale, -1.0f * scale,
        1.0f * scale, -1.0f * scale, -1.0f * scale,
        -1.0f * scale, -1.0f * scale, 1.0f * scale,
        1.0f * scale, -1.0f * scale, 1.0f * scale};

    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
    glBindVertexArray(0);

    createSkyBox();
}

void Skybox::createSkyBox()
{
    std::vector<const GLchar *> faces;
    faces.push_back("posx.jpg");
    faces.push_back("negx.jpg");
    faces.push_back("negy.jpg");
    faces.push_back("posy.jpg");
    faces.push_back("posz.jpg");
    faces.push_back("negz.jpg");
    loadCubemap(faces);
}

void Skybox::loadCubemap(std::vector<const GLchar *> faces)
{
    glGenTextures(1, &m_texId);
    glActiveTexture(GL_TEXTURE0);

    int width, height;

    glBindTexture(GL_TEXTURE_CUBE_MAP, m_texId);
    for (GLuint i = 0; i < faces.size(); i++)
    {
        std::stringstream ss;
        ss << "../textures/512/" << faces[i];

        sf::Image image;
        image.loadFromFile(ss.str());
        image.flipVertically();
        width = image.getSize().x;
        height = image.getSize().y;

        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
            GL_RGBA32F, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (const GLvoid*)image.getPixelsPtr());

    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

Skybox::~Skybox()
{
    glcheck(glDeleteBuffers(1, &skyboxVAO));
    glcheck(glDeleteBuffers(1, &skyboxVBO));

    glcheck(glDeleteTextures(1, &m_texId));
}

void Skybox::do_draw()
{
    // Draw skybox first
    glDepthMask(GL_FALSE); // Remember to turn depth writing off
    // skybox cube
    glBindVertexArray(skyboxVAO);
    
    int texSamplerLocation = m_shaderProgram->getUniformLocation("skybox");    

    if (texSamplerLocation != ShaderProgram::null_location)
    {
        glcheck(glActiveTexture(GL_TEXTURE0));
        glcheck(glBindTexture(GL_TEXTURE_CUBE_MAP, m_texId));

        // Tells the sampler to use the texture unit 0
        glcheck(glUniform1i(texSamplerLocation, 0));
    }

    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthMask(GL_TRUE);

    if (texSamplerLocation != ShaderProgram::null_location)
    {
        glcheck(glActiveTexture(GL_TEXTURE0));
        glcheck(glBindTexture(GL_TEXTURE_CUBE_MAP, 0)); // unbind the texture!
    }
}

void Skybox::do_animate(float time)
{
}
