#include "../../include/keyframes/KeyframedCylinderRenderable.hpp"
#include "../../include/keyframes/GeometricTransformation.hpp"

#include "../../include/gl_helper.hpp"
#include "../../include/Utils.hpp"
#include "../../teachers/Geometries.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

KeyframedCylinderRenderable::KeyframedCylinderRenderable(
        ShaderProgramPtr prog, const MaterialPtr& material)
    : LightedCylinderRenderable(prog, material)
{
}

KeyframedCylinderRenderable::~KeyframedCylinderRenderable()
{
}

void KeyframedCylinderRenderable::addParentTransformKeyframe(float time, const GeometricTransformation& transformation)
{
    m_parentKeyframes.add(time, transformation);
}

void KeyframedCylinderRenderable::addLocalTransformKeyframe(float time, const GeometricTransformation& transformation)
{
    m_localKeyframes.add(time, transformation);
}

void KeyframedCylinderRenderable::do_animate(float time)
{
    //Assign the interpolated transformations from the keyframes to the local/parent transformations.
    if (!m_localKeyframes.empty()) {
        setLocalTransform(m_localKeyframes.interpolateTransformation(time));
    }
    if (!m_parentKeyframes.empty()) {
        setParentTransform(m_parentKeyframes.interpolateTransformation(time));
    }
}
