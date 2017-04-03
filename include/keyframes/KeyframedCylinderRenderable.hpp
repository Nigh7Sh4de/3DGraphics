#ifndef KEYFRAMED_CYLINDER_RENDERABLE_HPP
#define KEYFRAMED_CYLINDER_RENDERABLE_HPP

#include "../lighting/LightedCylinderRenderable.hpp"
#include "KeyframeCollection.hpp"

#include <glm/glm.hpp>

class GeometricTransformation;

class KeyframedCylinderRenderable : public LightedCylinderRenderable
{
public:
    KeyframedCylinderRenderable(ShaderProgramPtr program,
        const MaterialPtr& material = Material::Pearl());
    ~KeyframedCylinderRenderable();

    /**
    * \brief Add a keyframe for the parent transformation of the renderable.
    *
    * Add a keyframe to m_parentKeyframes described by a geometric transformation and a time.
    * \param time The time of the keyframe.
    * \param transformation The geometric transformation of the keyframe.
    */
    void addParentTransformKeyframe(float time, const GeometricTransformation& transformation);

    /**
     * \brief Add a keyframe for the local transformation of the renderable.
     *
     * Add a keyframe to m_localKeyframes described by a geometric transformation and a time.
     * \param time The time of the keyframe.
     * \param transformation The geometric transformation of the keyframe.
     */
    void addLocalTransformKeyframe(float time, const GeometricTransformation& transformation);

protected:
    // void do_draw();
    void do_animate(float time);

private:
    KeyframeCollection m_localKeyframes; /*!< A collection of keyframes for the local transformation of renderable. */
    KeyframeCollection m_parentKeyframes; /*!< A collection of keyframes for the parent transformation of renderable. */
};

typedef std::shared_ptr<KeyframedCylinderRenderable> KeyframedCylinderRenderablePtr;

#endif
