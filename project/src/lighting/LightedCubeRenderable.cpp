#include "../../include/lighting/LightedCubeRenderable.hpp"
#include "../../include/gl_helper.hpp"

#include <glm/gtc/type_ptr.hpp>

LightedCubeRenderable::LightedCubeRenderable(ShaderProgramPtr prog, const MaterialPtr& material)
    : CubeRenderable(prog), Materiable(material)
{
}

LightedCubeRenderable::~LightedCubeRenderable()
{
}

void LightedCubeRenderable::do_draw()
{
    //Send material to GPU as uniform
    Material::sendToGPU(m_shaderProgram, getMaterial());

    //NIT matrix
    int nitLocation = m_shaderProgram->getUniformLocation("NIT");
    if (nitLocation != ShaderProgram::null_location) {
        glcheck(glUniformMatrix3fv(nitLocation, 1, GL_FALSE,
            glm::value_ptr(glm::transpose(glm::inverse(glm::mat3(getModelMatrix()))))));
    }

    // Call the super method to bind other uniforms/attributes & draw
    teachers::CubeRenderable::do_draw();

    if (nitLocation != ShaderProgram::null_location) {
        glcheck(glDisableVertexAttribArray(nitLocation));
    }
}

void LightedCubeRenderable::do_animate(float time)
{
}
