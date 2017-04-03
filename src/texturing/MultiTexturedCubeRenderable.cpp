#include "./../../include/texturing/MultiTexturedCubeRenderable.hpp"
#include "./../../include/gl_helper.hpp"
#include "./../../teachers/Geometries.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <SFML/Graphics/Image.hpp>
#include <iostream>

MultiTexturedCubeRenderable::MultiTexturedCubeRenderable(
        ShaderProgramPtr shaderProgram,
        const std::string& textureFilename1,
        const std::string& textureFilename2)
    : HierarchicalRenderable(shaderProgram),
      m_pBuffer(0), m_nBuffer(0),
      m_tBuffer1(0), m_tBuffer2(0), m_blendingCoefficient(0.5f)
{
    //Initialize geometry
    std::vector<glm::vec2> tmp_texCoords;
    teachers::getUnitCube(m_positions, m_normals, tmp_texCoords);
    m_texCoords1 = tmp_texCoords;
    m_texCoords2 = tmp_texCoords;

    // === PART 1: Vertex attributes, except texture coordinates
    //Create buffers
    glcheck(glGenBuffers(1, &m_pBuffer)); //vertices
    glcheck(glGenBuffers(1, &m_nBuffer)); //normals

    //Activate buffer and send data to the graphics card
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_positions.size()*sizeof(glm::vec3), m_positions.data(), GL_STATIC_DRAW));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_nBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_normals.size()*sizeof(glm::vec3), m_normals.data(), GL_STATIC_DRAW));


    // === PART 2: Texture
    // texture coordinates: just like any other vertex attributes!
    glGenBuffers(1, &m_tBuffer1);
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_tBuffer1));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_texCoords1.size()*sizeof(glm::vec2), m_texCoords1.data(), GL_STATIC_DRAW));
    glGenBuffers(1, &m_tBuffer2);
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_tBuffer2));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_texCoords2.size()*sizeof(glm::vec2), m_texCoords2.data(), GL_STATIC_DRAW));

    // IMAGE 1
    // now handle the "texture image" itself
    // load the image (here using the sfml library)
    sf::Image image;
    image.loadFromFile(textureFilename1);
    // sfml inverts the v axis...
    // Hence, flip it to put the image in OpenGL convention: lower left corner is (0,0)
    image.flipVertically();

    // create a GPU buffer then bind the texture
    glcheck(glGenTextures(1, &m_texId1));
    glcheck(glBindTexture(GL_TEXTURE_2D, m_texId1));

    // texture options
    glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    // Transfer the texture image texture to the GPU
    glcheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F,
        image.getSize().x, image.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE,
        (const GLvoid*)image.getPixelsPtr()));

    // Release the texture
    glcheck(glBindTexture(GL_TEXTURE_2D, 0));


    // IMAGE 2: same thing!
    // now handle the "texture image" itself
    // load the image (here using the sfml library)
    image.loadFromFile(textureFilename2);
    // sfml inverts the v axis...
    // Hence, flip it to put the image in OpenGL convention: lower left corner is (0,0)
    image.flipVertically();

    // create a GPU buffer then bind the texture
    glcheck(glGenTextures(1, &m_texId2));
    glcheck(glBindTexture(GL_TEXTURE_2D, m_texId2));

    // texture options
    glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    // Transfer the texture image texture to the GPU
    glcheck(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F,
        image.getSize().x, image.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE,
        (const GLvoid*)image.getPixelsPtr()));

    // Release the texture
    glcheck(glBindTexture(GL_TEXTURE_2D, 0));
}

MultiTexturedCubeRenderable::~MultiTexturedCubeRenderable()
{
    glcheck(glDeleteBuffers(1, &m_pBuffer));
    glcheck(glDeleteBuffers(1, &m_tBuffer1));
    glcheck(glDeleteBuffers(1, &m_tBuffer2));
    glcheck(glDeleteBuffers(1, &m_nBuffer));

    glcheck(glDeleteTextures(1, &m_texId1));
    glcheck(glDeleteTextures(1, &m_texId2));
}

void MultiTexturedCubeRenderable::do_draw()
{
    //Location
    int modelLocation = m_shaderProgram->getUniformLocation("modelMat");
    int nitLocation = m_shaderProgram->getUniformLocation("NIT");
    int texSamplerLocation1 = m_shaderProgram->getUniformLocation("texSampler1");
    int texSamplerLocation2 = m_shaderProgram->getUniformLocation("texSampler2");
    int blendingCoeffLocation = m_shaderProgram->getUniformLocation("blendingCoeff");

    int positionLocation = m_shaderProgram->getAttributeLocation("vPosition");
    int normalLocation = m_shaderProgram->getAttributeLocation("vNormal");
    int texCoordLocation1 = m_shaderProgram->getAttributeLocation("vTexCoord1");
    int texCoordLocation2 = m_shaderProgram->getAttributeLocation("vTexCoord2");




    //Send material as uniform to GPU
    Material::sendToGPU(m_shaderProgram, m_material);

    //Send uniforms to GPU
    if (modelLocation != ShaderProgram::null_location) {
        glcheck(glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(getModelMatrix())));
    }
    if (nitLocation != ShaderProgram::null_location) {
        glcheck(glUniformMatrix3fv(nitLocation, 1, GL_FALSE,
            glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(getModelMatrix()))))));
    }
    if (blendingCoeffLocation != ShaderProgram::null_location) {
        glcheck(glUniform1f(blendingCoeffLocation, m_blendingCoefficient));
    }

    //Send vertex attributes to GPU
    if (positionLocation != ShaderProgram::null_location) {
        glcheck(glEnableVertexAttribArray(positionLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer));
        glcheck(glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }
    if (normalLocation != ShaderProgram::null_location) {
        glcheck(glEnableVertexAttribArray(normalLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_nBuffer));
        glcheck(glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }

    // Texture and texture coordinates -- bind texture 1 to Textured Unit 0
    if (texCoordLocation1 != ShaderProgram::null_location) {
        // Bind texture on texture unit 0
        glcheck(glActiveTexture(GL_TEXTURE0));
        glcheck(glBindTexture(GL_TEXTURE_2D, m_texId1));

        // Tells the sampler to use the texture unit 0
        glcheck(glUniform1i(texSamplerLocation1, 0));

        // Send texture coordinates attributes
        glcheck(glEnableVertexAttribArray(texCoordLocation1));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_tBuffer1));
        glcheck(glVertexAttribPointer(texCoordLocation1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }

    // Texture and texture coordinates -- bind texture 2 to Textured Unit 1
    if (texCoordLocation2 != ShaderProgram::null_location) {
        // Bind texture on texture unit 1
        glcheck(glActiveTexture(GL_TEXTURE1));
        glcheck(glBindTexture(GL_TEXTURE_2D, m_texId2));

        // Tells the sampler to use the texture unit 1
        glcheck(glUniform1i(texSamplerLocation2, 1));

        // Send texture coordinates attributes
        glcheck(glEnableVertexAttribArray(texCoordLocation2));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_tBuffer2));
        glcheck(glVertexAttribPointer(texCoordLocation2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }


    //Draw triangles elements
    glcheck(glDrawArrays(GL_TRIANGLES,0, m_positions.size()));


    //Release everything
    if (positionLocation != ShaderProgram::null_location) {
        glcheck(glDisableVertexAttribArray(positionLocation));
    }
    if (normalLocation != ShaderProgram::null_location) {
        glcheck(glDisableVertexAttribArray(normalLocation));
    }

    if (texCoordLocation1 != ShaderProgram::null_location) {
        glcheck(glDisableVertexAttribArray(texCoordLocation1));
        glcheck(glBindTexture(GL_TEXTURE_2D, 0));   // unbind the texture!
    }
    if (texCoordLocation2 != ShaderProgram::null_location) {
        glcheck(glDisableVertexAttribArray(texCoordLocation2));
        glcheck(glBindTexture(GL_TEXTURE_2D, 1));   // unbind the texture!
    }

}

void MultiTexturedCubeRenderable::do_animate(float time)
{
    m_blendingCoefficient = std::sin(time);
}

void MultiTexturedCubeRenderable::setMaterial(const MaterialPtr& material)
{
    m_material = material;
}
