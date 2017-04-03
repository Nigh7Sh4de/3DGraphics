#include "./../../include/lighting/Lights.hpp"
#include "./../../include/gl_helper.hpp"
#include "./../../include/log.hpp"
#include <glm/gtc/type_ptr.hpp>

DirectionalLight::~DirectionalLight()
{}

DirectionalLight::DirectionalLight()
{
    m_direction = glm::vec3(0.0);
    m_ambient = glm::vec3(0.0);
    m_diffuse = glm::vec3(0.0);
    m_specular = glm::vec3(0.0);
}

DirectionalLight::DirectionalLight(const DirectionalLight& directionalLight)
{
    m_direction = directionalLight.m_direction;
    m_ambient = directionalLight.m_ambient;
    m_diffuse = directionalLight.m_diffuse;
    m_specular = directionalLight.m_specular;
}

DirectionalLight::DirectionalLight(const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular)
{
    m_direction = direction;
    m_ambient = ambient;
    m_diffuse = diffuse;
    m_specular = specular;
}

const glm::vec3 &DirectionalLight::direction() const
{
    return m_direction;
}

void DirectionalLight::setDirection(const glm::vec3 &direction)
{
    m_direction = direction;
}

const glm::vec3& DirectionalLight::ambient() const
{
    return m_ambient;
}

void DirectionalLight::setAmbient(const glm::vec3 &ambient)
{
    m_ambient = ambient;
}

const glm::vec3 &DirectionalLight::diffuse() const
{
    return m_diffuse;
}

void DirectionalLight::setDiffuse(const glm::vec3 &diffuse)
{
    m_diffuse = diffuse;
}

const glm::vec3& DirectionalLight::specular() const
{
    return m_specular;
}

void DirectionalLight::setSpecular(const glm::vec3 &specular)
{
    m_specular = specular;
}

bool DirectionalLight::sendToGPU(const ShaderProgramPtr& program, const DirectionalLightPtr& light)
{
    bool success = true;
    int location = -1;

    if (program==nullptr || light==nullptr) {
        return false;
    }

    location = program->getUniformLocation(light->lightName()+"."+light->directionName());
    if (location!=ShaderProgram::null_location) {
        glcheck(glUniform3fv(location, 1, glm::value_ptr(light->direction())));
    } else {
        success = false;
    }

    location = program->getUniformLocation(light->lightName()+"."+light->ambientName());
    if (location!=ShaderProgram::null_location) {
        glcheck(glUniform3fv(location, 1, glm::value_ptr(light->ambient())));
    } else {
        success = false;
    }

    location = program->getUniformLocation(light->lightName()+"."+light->diffuseName());
    if (location!=ShaderProgram::null_location) {
        glcheck(glUniform3fv(location, 1, glm::value_ptr(light->diffuse())));
    } else {
        success = false;
    }

    location = program->getUniformLocation(light->lightName()+"."+light->specularName());
    if (location!=ShaderProgram::null_location) {
        glcheck(glUniform3fv(location, 1, glm::value_ptr(light->specular())));
    } else {
        success = false;
    }

    return success;
}

PointLight::~PointLight()
{
}

PointLight::PointLight()
{
    m_position = glm::vec3(0.0);
    m_ambient = glm::vec3(0.0);
    m_diffuse = glm::vec3(0.0);
    m_specular = glm::vec3(0.0);
    m_constant = 0.0;
    m_linear = 0.0;
    m_quadratic = 0.0;
}

PointLight::PointLight(const PointLight& pointLight)
{
    m_position = pointLight.m_position;
    m_ambient = pointLight.m_ambient;
    m_diffuse = pointLight.m_diffuse;
    m_specular = pointLight.m_specular;
    m_constant = pointLight.m_constant;
    m_linear = pointLight.m_linear;
    m_quadratic = pointLight.m_quadratic;
}

PointLight::PointLight(const glm::vec3 &position, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular,
           const float& constant, const float& linear, const float& quadratic)
{
    m_position = position;
    m_ambient = ambient;
    m_diffuse = diffuse;
    m_specular = specular;
    m_constant = constant;
    m_linear = linear;
    m_quadratic = quadratic;
}

const glm::vec3 &PointLight::position() const
{
    return m_position;
}

void PointLight::setPosition(const glm::vec3 &position)
{
    m_position = position;
}

const glm::vec3& PointLight::ambient() const
{
    return m_ambient;
}

void PointLight::setAmbient(const glm::vec3 &ambient)
{
    m_ambient = ambient;
}

const glm::vec3 &PointLight::diffuse() const
{
    return m_diffuse;
}

void PointLight::setDiffuse(const glm::vec3 &diffuse)
{
    m_diffuse = diffuse;
}

const glm::vec3& PointLight::specular() const
{
    return m_specular;
}

void PointLight::setSpecular(const glm::vec3 &specular)
{
    m_specular = specular;
}

const float &PointLight::constant() const
{
    return m_constant;
}

void PointLight::setConstant(float constant)
{
    m_constant = constant;
}

const float& PointLight::linear() const
{
    return m_linear;
}

void PointLight::setLinear(float linear)
{
    m_linear = linear;
}

const float &PointLight::quadratic() const
{
    return m_quadratic;
}

void PointLight::setQuadratic(float quadratic)
{
    m_quadratic = quadratic;
}

bool PointLight::sendToGPU(const ShaderProgramPtr& program, const PointLightPtr& light)
{
    bool success = true;
    int location = -1;

    if (program==nullptr || light==nullptr) {
        return false;
    }

    location = program->getUniformLocation(light->lightName()+"."+light->positionName());
    if (location!=ShaderProgram::null_location) {
        glcheck(glUniform3fv(location, 1, glm::value_ptr(light->position())));
    } else {
        success = false;
    }

    location = program->getUniformLocation(light->lightName()+"."+light->ambientName());
    if (location!=ShaderProgram::null_location) {
        glcheck(glUniform3fv(location, 1, glm::value_ptr(light->ambient())));
    } else {
        success = false;
    }

    location = program->getUniformLocation(light->lightName()+"."+light->diffuseName());
    if (location!=ShaderProgram::null_location) {
        glcheck(glUniform3fv(location, 1, glm::value_ptr(light->diffuse())));
    } else {
        success = false;
    }

    location = program->getUniformLocation(light->lightName()+"."+light->specularName());
    if (location!=ShaderProgram::null_location) {
        glcheck(glUniform3fv(location, 1, glm::value_ptr(light->specular())));
    } else {
        success = false;
    }

    location = program->getUniformLocation(light->lightName()+"."+light->constantName());
    if (location!=ShaderProgram::null_location) {
        glcheck(glUniform1f(location, light->constant()));
    } else {
        success = false;
    }

    location = program->getUniformLocation(light->lightName()+"."+light->linearName());
    if (location!=ShaderProgram::null_location) {
        glcheck(glUniform1f(location, light->linear()));
    } else {
        success = false;
    }

    location = program->getUniformLocation(light->lightName()+"."+light->quadraticName());
    if (location!=ShaderProgram::null_location) {
        glcheck(glUniform1f(location, light->quadratic()));
    } else {
        success = false;
    }

    return success;
}


bool PointLight::sendToGPU(const ShaderProgramPtr& program, const std::vector<PointLightPtr>& lights)
{
    bool success = true;
    int location = -1;

    if (program==nullptr || lights.empty()) {
        return false;
    }

    for (size_t i=0; i<lights.size(); ++i) {
        if (lights[i]==nullptr) {
            return false;
        }

        location = program->getUniformLocation(lights[i]->numberOfLightsName());
        if (location!=ShaderProgram::null_location) {
            glcheck(glUniform1i(location, (int)lights.size()));
        } else {
            success = false;
        }

        location = program->getUniformLocation(lights[i]->lightName()+std::string("[")+std::to_string(i)+std::string("]")+"."+lights[i]->positionName());
        if (location!=ShaderProgram::null_location) {
            glcheck(glUniform3fv(location, 1, glm::value_ptr(lights[i]->position())));
        } else {
            success = false;
        }

        location = program->getUniformLocation(lights[i]->lightName()+std::string("[")+std::to_string(i)+std::string("]")+"."+lights[i]->ambientName());
        if (location!=ShaderProgram::null_location) {
            glcheck(glUniform3fv(location, 1, glm::value_ptr(lights[i]->ambient())));
        } else {
            success = false;
        }

        location = program->getUniformLocation(lights[i]->lightName()+std::string("[")+std::to_string(i)+std::string("]")+"."+lights[i]->diffuseName());
        if (location!=ShaderProgram::null_location) {
            glcheck(glUniform3fv(location, 1, glm::value_ptr(lights[i]->diffuse())));
        } else {
            success = false;
        }

        location = program->getUniformLocation(lights[i]->lightName()+std::string("[")+std::to_string(i)+std::string("]")+"."+lights[i]->specularName());
        if (location!=ShaderProgram::null_location) {
            glcheck(glUniform3fv(location, 1, glm::value_ptr(lights[i]->specular())));
        } else {
            success = false;
        }

        location = program->getUniformLocation(lights[i]->lightName()+std::string("[")+std::to_string(i)+std::string("]")+"."+lights[i]->constantName());
        if (location!=ShaderProgram::null_location) {
            glcheck(glUniform1f(location, lights[i]->constant()));
        } else {
            success = false;
        }

        location = program->getUniformLocation(lights[i]->lightName()+std::string("[")+std::to_string(i)+std::string("]")+"."+lights[i]->linearName());
        if (location!=ShaderProgram::null_location) {
            glcheck(glUniform1f(location, lights[i]->linear()));
        } else {
            success = false;
        }

        location = program->getUniformLocation(lights[i]->lightName()+std::string("[")+std::to_string(i)+std::string("]")+"."+lights[i]->quadraticName());
        if (location!=ShaderProgram::null_location) {
            glcheck(glUniform1f(location, lights[i]->quadratic()));
        } else {
            success = false;
        }
    }
    return success;
}

SpotLight::~SpotLight()
{
}

SpotLight::SpotLight()
{
    m_position = glm::vec3(0.0);
    m_spotDirection = glm::vec3(0.0);
    m_ambient = glm::vec3(0.0);
    m_diffuse = glm::vec3(0.0);
    m_specular = glm::vec3(0.0);
    m_constant = 0.0;
    m_linear = 0.0;
    m_quadratic = 0.0;
    m_cosInnerCutOff = 0.0;
    m_cosOuterCutOff = 0.0;
}

SpotLight::SpotLight(const SpotLight& spotLight)
{
    m_position = spotLight.m_position;
    m_spotDirection = spotLight.m_spotDirection;
    m_ambient = spotLight.m_ambient;
    m_diffuse = spotLight.m_diffuse;
    m_specular = spotLight.m_specular;
    m_constant = spotLight.m_constant;
    m_linear = spotLight.m_linear;
    m_quadratic = spotLight.m_quadratic;
    m_cosInnerCutOff = spotLight.m_cosInnerCutOff;
    m_cosOuterCutOff = spotLight.m_cosOuterCutOff;
}

SpotLight::SpotLight(const glm::vec3& position, const glm::vec3& spotDirection,
          const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular,
          const float& constant, const float& linear, const float& quadratic,
          const float& cosInnerCutOff, const float& cosOuterCutOff)
{
    m_position = position;
    m_spotDirection = spotDirection;
    m_ambient = ambient;
    m_diffuse = diffuse;
    m_specular = specular;
    m_constant = constant;
    m_linear = linear;
    m_quadratic = quadratic;
    m_cosInnerCutOff = cosInnerCutOff;
    m_cosOuterCutOff = cosOuterCutOff;
}

const glm::vec3 &SpotLight::position() const
{
    return m_position;
}

void SpotLight::setPosition(const glm::vec3 &position)
{
    m_position = position;
}

const glm::vec3 &SpotLight::spotDirection() const
{
    return m_spotDirection;
}

void SpotLight::setSpotDirection(const glm::vec3 &spotDirection)
{
    m_spotDirection = spotDirection;
}

const glm::vec3 &SpotLight::ambient() const
{
    return m_ambient;
}

void SpotLight::setAmbient(const glm::vec3 &ambient)
{
    m_ambient = ambient;
}

const glm::vec3& SpotLight::diffuse() const
{
    return m_diffuse;
}

void SpotLight::setDiffuse(const glm::vec3 &diffuse)
{
    m_diffuse = diffuse;
}

const glm::vec3 &SpotLight::specular() const
{
    return m_specular;
}

void SpotLight::setSpecular(const glm::vec3 &specular)
{
    m_specular = specular;
}

const float& SpotLight::constant() const
{
    return m_constant;
}

void SpotLight::setConstant(float constant)
{
    m_constant = constant;
}

const float &SpotLight::linear() const
{
    return m_linear;
}

void SpotLight::setLinear(float linear)
{
    m_linear = linear;
}

const float& SpotLight::quadratic() const
{
    return m_quadratic;
}

void SpotLight::setQuadratic(float quadratic)
{
    m_quadratic = quadratic;
}

const float &SpotLight::cosInnerCutOff() const
{
    return m_cosInnerCutOff;
}

void SpotLight::setCosInnerCutOff(float cosInnerCutOff)
{
    m_cosInnerCutOff = cosInnerCutOff;
}

const float& SpotLight::cosOuterCutOff() const
{
    return m_cosOuterCutOff;
}

void SpotLight::setCosOuterCutOff(float cosOuterCutOff)
{
    m_cosOuterCutOff = cosOuterCutOff;
}

bool SpotLight::sendToGPU(const ShaderProgramPtr& program, const SpotLightPtr& light)
{
    bool success = true;
    int location = -1;

    if (program==nullptr || light==nullptr) {
        return false;
    }

    location = program->getUniformLocation(light->lightName()+"."+light->positionName());
    if (location!=ShaderProgram::null_location) {
        glcheck(glUniform3fv(location, 1, glm::value_ptr(light->position())));
    } else {
        success = false;
    }

    location = program->getUniformLocation(light->lightName()+"."+light->spotDirectionName());
    if (location!=ShaderProgram::null_location) {
        glcheck(glUniform3fv(location, 1, glm::value_ptr(light->spotDirection())));
    } else {
        success = false;
    }

    location = program->getUniformLocation(light->lightName()+"."+light->ambientName());
    if (location!=ShaderProgram::null_location) {
        glcheck(glUniform3fv(location, 1, glm::value_ptr(light->ambient())));
    } else {
        success = false;
    }

    location = program->getUniformLocation(light->lightName()+"."+light->diffuseName());
    if (location!=ShaderProgram::null_location) {
        glcheck(glUniform3fv(location, 1, glm::value_ptr(light->diffuse())));
    } else {
        success = false;
    }

    location = program->getUniformLocation(light->lightName()+"."+light->specularName());
    if (location!=ShaderProgram::null_location) {
        glcheck(glUniform3fv(location, 1, glm::value_ptr(light->specular())));
    } else {
        success = false;
    }

    location = program->getUniformLocation(light->lightName()+"."+light->constantName());
    if (location!=ShaderProgram::null_location) {
        glcheck(glUniform1f(location, light->constant()));
    } else {
        success = false;
    }

    location = program->getUniformLocation(light->lightName()+"."+light->linearName());
    if (location!=ShaderProgram::null_location) {
        glcheck(glUniform1f(location, light->linear()));
    } else {
        success = false;
    }

    location = program->getUniformLocation(light->lightName()+"."+light->quadraticName());
    if (location!=ShaderProgram::null_location) {
        glcheck(glUniform1f(location, light->quadratic()));
    } else {
        success = false;
    }

    location = program->getUniformLocation(light->lightName()+"."+light->cosInnerCutOffName());
    if (location!=ShaderProgram::null_location) {
        glcheck(glUniform1f(location, light->cosInnerCutOff()));
    } else {
        success = false;
    }

    location = program->getUniformLocation(light->lightName()+"."+light->cosOuterCutOffName());
    if (location!=ShaderProgram::null_location) {
        glcheck(glUniform1f(location, light->cosOuterCutOff()));
    } else {
        success = false;
    }

    return success;
}

bool SpotLight::sendToGPU(const ShaderProgramPtr& program, const std::vector<SpotLightPtr>& lights)
{
    bool success = true;
    int location = -1;

    if (program==nullptr || lights.empty()) {
        return false;
    }

    for (size_t i=0; i<lights.size(); ++i) {
        if (lights[i]==nullptr) {
            return false;
        }

        location = program->getUniformLocation(lights[i]->numberOfLightsName());
        if (location!=ShaderProgram::null_location) {
            glcheck(glUniform1i(location, (int)lights.size()));
        } else {
            success = false;
        }

        location = program->getUniformLocation(lights[i]->lightName()+std::string("[")+std::to_string(i)+std::string("]")+"."+lights[i]->positionName());
        if (location!=ShaderProgram::null_location) {
            glcheck(glUniform3fv(location, 1, glm::value_ptr(lights[i]->position())));
        } else {
            success = false;
        }

        location = program->getUniformLocation(lights[i]->lightName()+std::string("[")+std::to_string(i)+std::string("]")+"."+lights[i]->spotDirectionName());
        if (location!=ShaderProgram::null_location) {
            glcheck(glUniform3fv(location, 1, glm::value_ptr(lights[i]->spotDirection())));
        } else {
            success = false;
        }

        location = program->getUniformLocation(lights[i]->lightName()+std::string("[")+std::to_string(i)+std::string("]")+"."+lights[i]->ambientName());
        if (location!=ShaderProgram::null_location) {
            glcheck(glUniform3fv(location, 1, glm::value_ptr(lights[i]->ambient())));
        } else {
            success = false;
        }

        location = program->getUniformLocation(lights[i]->lightName()+std::string("[")+std::to_string(i)+std::string("]")+"."+lights[i]->diffuseName());
        if (location!=ShaderProgram::null_location) {
            glcheck(glUniform3fv(location, 1, glm::value_ptr(lights[i]->diffuse())));
        } else {
            success = false;
        }

        location = program->getUniformLocation(lights[i]->lightName()+std::string("[")+std::to_string(i)+std::string("]")+"."+lights[i]->specularName());
        if (location!=ShaderProgram::null_location) {
            glcheck(glUniform3fv(location, 1, glm::value_ptr(lights[i]->specular())));
        } else {
            success = false;
        }

        location = program->getUniformLocation(lights[i]->lightName()+std::string("[")+std::to_string(i)+std::string("]")+"."+lights[i]->constantName());
        if (location!=ShaderProgram::null_location) {
            glcheck(glUniform1f(location, lights[i]->constant()));
        } else {
            success = false;
        }

        location = program->getUniformLocation(lights[i]->lightName()+std::string("[")+std::to_string(i)+std::string("]")+"."+lights[i]->linearName());
        if (location!=ShaderProgram::null_location) {
            glcheck(glUniform1f(location, lights[i]->linear()));
        } else {
            success = false;
        }

        location = program->getUniformLocation(lights[i]->lightName()+std::string("[")+std::to_string(i)+std::string("]")+"."+lights[i]->quadraticName());
        if (location!=ShaderProgram::null_location) {
            glcheck(glUniform1f(location, lights[i]->quadratic()));
        } else {
            success = false;
        }

        location = program->getUniformLocation(lights[i]->lightName()+std::string("[")+std::to_string(i)+std::string("]")+"."+lights[i]->cosInnerCutOffName());
        if (location!=ShaderProgram::null_location) {
            glcheck(glUniform1f(location, lights[i]->cosInnerCutOff()));
        } else {
            success = false;
        }

        location = program->getUniformLocation(lights[i]->lightName()+std::string("[")+std::to_string(i)+std::string("]")+"."+lights[i]->cosOuterCutOffName());
        if (location!=ShaderProgram::null_location) {
            glcheck(glUniform1f(location, lights[i]->cosOuterCutOff()));
        } else {
            success = false;
        }
    }
    return success;
}
