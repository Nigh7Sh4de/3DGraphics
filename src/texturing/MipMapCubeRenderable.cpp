#include "./../../include/texturing/MipMapCubeRenderable.hpp"
#include "./../../include/gl_helper.hpp"
#include "./../../teachers/Geometries.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <SFML/Graphics/Image.hpp>
#include <iostream>

MipMapCubeRenderable::MipMapCubeRenderable(
        ShaderProgramPtr shaderProgram,
        std::vector<std::string> &textureFilenames)    // a list of filenames!
    : HierarchicalRenderable(shaderProgram),
      m_pBuffer(0), m_nBuffer(0), m_tBuffer(0),
      m_mipmapOption(0)
{
    //Initialize geometry
    teachers::getUnitCube(m_positions, m_normals, m_texCoords);

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
    glGenBuffers(1, &m_tBuffer); //texture coords
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_tBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_texCoords.size()*sizeof(glm::vec2), m_texCoords.data(), GL_STATIC_DRAW));

    //Load imageS from files
    std::vector<sf::Image> images;
    sf::Vector2u imageSize(0,0);
    images.resize(textureFilenames.size());
    for (int i=0; i<images.size(); ++i) {
        images[i].loadFromFile(textureFilenames[i]);
        images[i].flipVertically(); // sfml inverts the v axis... put the image in OpenGL convention: lower left corner is (0,0)
        if (i == 0)
            imageSize = images[i].getSize();
    }

    // create a GPU buffer then bind the texture
    // Even if there are multiple sub-images, there is still ONE texture only!
    glcheck(glGenTextures(1, &m_texId));
    glcheck(glBindTexture(GL_TEXTURE_2D, m_texId));

    glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    // texture options
    glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    // specific mipmap options
    glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
    glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    // NOTHING CHANGED AFTERWARDS!

    // Transfer ALL texture sub-images to the GPU
    glcheck(glTexStorage2D(GL_TEXTURE_2D, images.size(), GL_RGBA32F, imageSize.x, imageSize.y));
    for (int i=0; i<images.size(); ++i) {
        glcheck(glTexSubImage2D(GL_TEXTURE_2D, i, 0, 0,
            images[i].getSize().x, images[i].getSize().y, GL_RGBA, GL_UNSIGNED_BYTE,
            (const GLvoid*)images[i].getPixelsPtr()));
    }

    // Release the texture
    glcheck(glBindTexture(GL_TEXTURE_2D, 0));
}

MipMapCubeRenderable::~MipMapCubeRenderable()
{
    glcheck(glDeleteBuffers(1, &m_pBuffer));
    glcheck(glDeleteBuffers(1, &m_tBuffer));
    glcheck(glDeleteBuffers(1, &m_nBuffer));

    glcheck(glDeleteTextures(1, &m_texId)); // even with several subimages, there is still a single texture!
}

void MipMapCubeRenderable::do_draw()
{
    // NOTHING HAS CHANGED FROM TexturedCubeRenderable!!

    //Locations
    int modelLocation = m_shaderProgram->getUniformLocation("modelMat");
    int nitLocation = m_shaderProgram->getUniformLocation("NIT");
    int texSamplerLocation = m_shaderProgram->getUniformLocation("texSampler");

    int positionLocation = m_shaderProgram->getAttributeLocation("vPosition");
    int normalLocation = m_shaderProgram->getAttributeLocation("vNormal");
    int texCoordLocation = m_shaderProgram->getAttributeLocation("vTexCoord");

    //Send material uniform to GPU
    Material::sendToGPU(m_shaderProgram, m_material);

    //Send uniforms to GPU
    if (modelLocation != ShaderProgram::null_location) {
        glcheck(glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(getModelMatrix())));
    }
    if (nitLocation != ShaderProgram::null_location) {
        glcheck(glUniformMatrix3fv(nitLocation, 1, GL_FALSE,
            glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(getModelMatrix()))))));
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

    // Texture and texture coordinates
    if (texCoordLocation != ShaderProgram::null_location) {
        // Bind texture on texture unit 0
        glcheck(glActiveTexture(GL_TEXTURE0));
        glcheck(glBindTexture(GL_TEXTURE_2D, m_texId));

        // Tells the sampler to use the texture unit 0
        glcheck(glUniform1i(texSamplerLocation, 0));

        // Send texture coordinates attributes
        glcheck(glEnableVertexAttribArray(texCoordLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_tBuffer));
        glcheck(glVertexAttribPointer(texCoordLocation, 2, GL_FLOAT, GL_FALSE, 0, (void*)0));
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

    if (texCoordLocation != ShaderProgram::null_location) {
        glcheck(glDisableVertexAttribArray(texCoordLocation));
        glcheck(glBindTexture(GL_TEXTURE_2D, 0));   // unbind the texture!
    }
}

void MipMapCubeRenderable::do_animate(float time)
{

}

void MipMapCubeRenderable::updateTextureOption()
{
    std::string text;

    //Bind the texture
    glBindTexture(GL_TEXTURE_2D, m_texId);

    // Here multiple texture files are loaded
    // Otherwise, generate multiresolution images with:
    //     glcheck(glGenerateMipmap(GL_TEXTURE_2D));

    //Textured options
    switch (m_mipmapOption) {
    case 0:
        glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
        glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        text.append("Filtering: LINEAR_MIPMAP_LINEAR");
        break;

    case 1:
        glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST));
        glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        text.append("Filtering: LINEAR_MIPMAP_NEAREST");
        break;

    case 2:
        glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST));
        glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        text.append("Filtering: NEAREST_MIPMAP_NEAREST");
        break;

    case 3:
        glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR));
        glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        text.append("Filtering: NEAREST_MIPMAP_LINEAR");
        break;

    }

    //Release the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    displayTextInViewer(text);
}

void MipMapCubeRenderable::do_keyPressedEvent(sf::Event& e)
{
    if (e.key.code == sf::Keyboard::F8) {
        m_mipmapOption = m_mipmapOption+1 > 3 ? 0 : m_mipmapOption+1;
        updateTextureOption();
    }
}


void MipMapCubeRenderable::setMaterial(const MaterialPtr& material)
{
    m_material = material;
}
