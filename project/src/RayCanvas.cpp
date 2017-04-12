#include "./../include/RayCanvas.hpp"
#include "./../include/gl_helper.hpp"
#include "./../include/log.hpp"
#include "./../include/Utils.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <iostream>
#include <sstream>

RayCanvas::RayCanvas(ShaderProgramPtr shaderProgram, int cube_map_resol) :
    HierarchicalRenderable(shaderProgram),
    m_pBuffer(0),  m_iBuffer(0), m_animTime(0)
{

    m_positions.push_back(glm::vec2(1.0f, 1.0f));
    m_positions.push_back(glm::vec2(1.0f, -1.0f));
    m_positions.push_back(glm::vec2(-1.0f, -1.0f));
    m_positions.push_back(glm::vec2(-1.0f, 1.0f));

    m_indices = { 0, 1, 2, 0, 2, 3 };

    //Create buffers
    glGenBuffers(1, &m_pBuffer);  // positions
    glGenBuffers(1, &m_iBuffer);  // indices

    //Activate buffer and send data to the graphics card
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_positions.size()*sizeof(glm::vec2), m_positions.data(), GL_STATIC_DRAW));
    glcheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iBuffer));
    glcheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size()*sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW));
    
    createHDRskyBox(cube_map_resol);

    glcheck(glGenTextures(1, &m_texScratchId));
    glBindTexture(GL_TEXTURE_2D, m_texScratchId);

    // texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    glcheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

    sf::Image image;
    image.loadFromFile("../textures/scratchmap.png");

    glTexStorage2D(GL_TEXTURE_2D, 11, GL_RGBA32F, image.getSize().x, image.getSize().y);
    glcheck(glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0,
                            image.getSize().x, image.getSize().y, GL_RGBA, GL_UNSIGNED_BYTE,
                            (const GLvoid*)image.getPixelsPtr ()));

    glGenerateMipmap(GL_TEXTURE_2D);

    // Release the texture
    glcheck(glBindTexture(GL_TEXTURE_2D, 0));

}

static float smoothstep(float e0, float e1, float x)
{
    float t = std::min(1.0f, std::max(0.0f, (x-e0) / (e1-e0)));
    return t*t*(3.0f-2.0f*t);
}

void RayCanvas::createHDRskyBox(int cube_map_resol)
{
    // Cubemap
    glcheck(glGenTextures(1, &m_texId));
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_texId);

    // texture options
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    std::vector<std::string> cm_name;
    cm_name.push_back("posx.jpg");
    cm_name.push_back("negx.jpg");
    cm_name.push_back("posy.jpg");
    cm_name.push_back("negy.jpg");
    cm_name.push_back("posz.jpg");
    cm_name.push_back("negz.jpg");

    max_mipmap = 12;

    switch (cube_map_resol) {
    case 2048:
        break;
    case 1024:
        max_mipmap = 11;
        break;
    case 512:
        max_mipmap = 10;
        break;
    default:
        std::cerr << "Warning: cubemap resolution for RayCanvas is either 2048, 1024 or 512\n";
        cube_map_resol = 2048;
        break;
    }

    using FloatImage = std::vector<float>;
    using SkyBox = std::vector<FloatImage>;
    using MMSkyBox = std::vector<SkyBox>;

    MMSkyBox skybox;
    skybox.resize(max_mipmap);
    for(int i = 0; i<max_mipmap; ++i)
        skybox[i].resize(6);

    uint top_width, top_height;

    // construct skybox with hdr hack
    for(int i = 0; i< 6; ++i)
    {
        std::stringstream ss;
        ss << "../textures/" << cube_map_resol << '/' << cm_name[i];

        sf::Image image;
        image.loadFromFile(ss.str());

        top_width = image.getSize().x;
        top_height = image.getSize().y;

        skybox[0][i].resize(top_width*top_height*4);

        for(int y = 0; y<top_height; ++y)
            for(int x = 0; x<top_width; ++x)
            {
                sf::Color c = image.getPixel(x, y);
                float r = float(c.r) / 255.0f;
                float g = float(c.g) / 255.0f;
                float b = float(c.b) / 255.0f;
                float intensity = r+g+b;
                float hdr = smoothstep(2.8f, 3.0f, intensity);
                r = r + r *hdr *5.0f;
                g = g + g *hdr *5.0f;
                b = b + b *hdr *5.0f;
                skybox[0][i][4*(x+y*top_width) + 0] = r;
                skybox[0][i][4*(x+y*top_width) + 1] = g;
                skybox[0][i][4*(x+y*top_width) + 2] = b;
                skybox[0][i][4*(x+y*top_width) + 3] = 1.0f;
            }
    }

    glTexStorage2D(GL_TEXTURE_CUBE_MAP, max_mipmap, GL_RGBA32F, top_width, top_height);

    for(int i = 0; i<6; ++i)
    {

        // Transfer the texture image texture to the GPU
        glcheck(glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, 0, 0,
                                top_width, top_height, GL_RGBA, GL_FLOAT,
                                (const GLvoid*)skybox[0][i].data()));
    }

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    // Release the texture
    glcheck(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}

RayCanvas::~RayCanvas()
{
    glcheck(glDeleteBuffers(1, &m_pBuffer));
    glcheck(glDeleteBuffers(1, &m_iBuffer));
    
    glcheck(glDeleteTextures(1, &m_texId));
    glcheck(glDeleteTextures(1, &m_texScratchId));

}

void RayCanvas::do_draw()
{
    int texSamplerLocation = m_shaderProgram->getUniformLocation("background");
    int texSamplerScratchLocation = m_shaderProgram->getUniformLocation("scratchTex");
    int texMipmapLevelLocation = m_shaderProgram->getUniformLocation("max_level");
    int timeLocation = m_shaderProgram->getUniformLocation("time");
    int positionLocation = m_shaderProgram->getAttributeLocation("vPosition");


    if(positionLocation != ShaderProgram::null_location) {
        glcheck(glEnableVertexAttribArray(positionLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_pBuffer));
        glcheck(glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }
    
    if(texSamplerLocation != ShaderProgram::null_location) {
        glcheck(glActiveTexture(GL_TEXTURE0));
        glcheck(glBindTexture(GL_TEXTURE_CUBE_MAP, m_texId));

        // Tells the sampler to use the texture unit 0
        glcheck(glUniform1i(texSamplerLocation, 0));
    }

    if(texSamplerScratchLocation != ShaderProgram::null_location) {
        glcheck(glActiveTexture(GL_TEXTURE1));
        glcheck(glBindTexture(GL_TEXTURE_2D, m_texScratchId));

        // Tells the sampler to use the texture unit 0
        glcheck(glUniform1i(texSamplerScratchLocation, 1));
    }

    if(texMipmapLevelLocation != ShaderProgram::null_location) {
        glcheck(glUniform1f(texMipmapLevelLocation, float(max_mipmap)));
    }

    if (timeLocation != ShaderProgram::null_location) {
        glcheck(glUniform1f(timeLocation, float(m_animTime)));
    }

    //Draw triangles elements
    glcheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iBuffer));
    glcheck(glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, (void*)0));

    if(positionLocation != ShaderProgram::null_location) {
        glcheck(glDisableVertexAttribArray(positionLocation));
    }
    
    if(texSamplerLocation != ShaderProgram::null_location) {
        glcheck(glActiveTexture(GL_TEXTURE0));
        glcheck(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));   // unbind the texture!
    }

    if(texSamplerScratchLocation != ShaderProgram::null_location) {
        glcheck(glActiveTexture(GL_TEXTURE1));
        glcheck(glBindTexture(GL_TEXTURE_2D, 0));   // unbind the texture!
    }
}

void RayCanvas::do_animate(float dTime, float time)
{
    //m_animTime = time;
}
