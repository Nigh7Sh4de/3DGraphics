#include "../../include/texturing/TexturedPlaneRenderable.hpp"
#include "../../include/gl_helper.hpp"
#include "../../include/log.hpp"
#include "../../teachers/Geometries.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <SFML/Graphics/Image.hpp>
#include <iostream>

TexturedPlaneRenderable::TexturedPlaneRenderable(
        ShaderProgramPtr shaderProgram, const std::string& textureFilename)
    : HierarchicalRenderable(shaderProgram),
      m_pBuffer(0), m_nBuffer(0), m_tBuffer(0), m_texId(0),
      m_wrapOption(0), m_filterOption(0)
{
    // Initialize geometry
    teachers::getUnitPlane(m_positions, m_normals, m_origTexCoords);
    m_texCoords = m_origTexCoords;

    // === PART 1: Vertex attributes, except texture coordinates
    //Create buffers
    glGenBuffers(1, &m_pBuffer); //vertices
    glGenBuffers(1, &m_nBuffer); //normals

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

    // now handle the "texture image" itself
    // load the image (here using the sfml library)
    sf::Image image;
    image.loadFromFile(textureFilename);
    // sfml inverts the v axis...
    // Hence, flip it to put the image in OpenGL convention: lower left corner is (0,0)
    image.flipVertically();

    // create a GPU buffer then bind the texture
    glcheck(glGenTextures(1, &m_texId));
    glcheck(glBindTexture(GL_TEXTURE_2D, m_texId));

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

TexturedPlaneRenderable::~TexturedPlaneRenderable()
{
    glcheck(glDeleteBuffers(1, &m_pBuffer));
    glcheck(glDeleteBuffers(1, &m_nBuffer));
    glcheck(glDeleteBuffers(1, &m_tBuffer));

    glcheck(glDeleteTextures(1, &m_texId));
}

void TexturedPlaneRenderable::do_draw()
{
    //Locations
    int modelLocation = m_shaderProgram->getUniformLocation("modelMat");
    int nitLocation = m_shaderProgram->getUniformLocation("NIT");
    int texSamplerLocaction = m_shaderProgram->getUniformLocation("texSampler");

    int positionLocation = m_shaderProgram->getAttributeLocation("vPosition");
    int normalLocation = m_shaderProgram->getAttributeLocation("vNormal");
    int texCoordLocation = m_shaderProgram->getAttributeLocation("vTexCoord");

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
        glcheck(glUniform1i(texSamplerLocaction, 0));

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

void TexturedPlaneRenderable::do_animate(float time)
{
}


void TexturedPlaneRenderable::updateTextureOption()
{
    //Resize texture coordinates factor
    float factor = 10.0;

    std::string text;

    //Bind the texture
    glBindTexture(GL_TEXTURE_2D, m_texId);

    //Textured options
    switch(m_wrapOption)
    {
    case 0:
        for(size_t i=0; i<m_texCoords.size(); ++i)
        {
            m_texCoords[i] = m_origTexCoords[i];
        }
        glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        text.append("Wrapping: CLAMP_TO_EDGE");
        break;

    case 1:
        for(size_t i=0; i<m_texCoords.size(); ++i)
        {
            m_texCoords[i] = factor*m_origTexCoords[i];
        }
        glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
        glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
        text.append("Wrapping: REPEAT");
        break;

    case 2:
        for(size_t i=0; i<m_texCoords.size(); ++i)
        {
            m_texCoords[i] = factor*m_origTexCoords[i];
        }
        glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT));
        glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT));
        text.append("Wrapping: MIRRORED_REPEAT");
        break;

    case 3:
        for(size_t i=0; i<m_texCoords.size(); ++i)
        {
            m_texCoords[i] = factor*m_origTexCoords[i] - glm::vec2(factor/2.0, factor/2.0);
        }
        glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        text.append("Wrapping: CLAMP_TO_EDGE");
        break;

    case 4:
        for(size_t i=0; i<m_texCoords.size(); ++i)
        {
            m_texCoords[i] = factor*m_origTexCoords[i] - glm::vec2(factor/2.0, factor/2.0);
        }
        float borderColor[] = { 1.0f, 0.0f, 1.0f, 1.0f };
        glcheck(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor));
        glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
        glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
        text.append("Wrapping: CLAMP_TO_BORDER");
        break;
    }


    switch(m_filterOption)
    {
    case 0:
        glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        text.append(", Filtering: NEAREST");
        break;

    case 1:
        glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        text.append(", Filtering: LINEAR");
        break;

//    case 2:
//        // generate mipmap images
//        glcheck(glGenerateMipmap(GL_TEXTURE_2D));
//        glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
//        glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
//        text.append(", Filtering: MIPMAP");
//        break;
    }

    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_tBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_texCoords.size()*sizeof(glm::vec2), m_texCoords.data(), GL_STATIC_DRAW));

    //Release the texture
    glBindTexture(GL_TEXTURE_2D, 0);

    displayTextInViewer(text);
}

void TexturedPlaneRenderable::do_keyPressedEvent( sf::Event& e )
{
    if (e.key.code == sf::Keyboard::F6) {
        m_wrapOption = m_wrapOption+1 > 4 ? 0 : m_wrapOption+1;
        updateTextureOption();
    }
    if (e.key.code == sf::Keyboard::F7) {
        m_filterOption = m_filterOption+1 > 1 ? 0 : m_filterOption+1;
        updateTextureOption();
    }
}

void TexturedPlaneRenderable::setMaterial(const MaterialPtr& material)
{
    m_material = material;
}
