# include "../../include/keyframes/KeyframeCollection.hpp"
# include <glm/gtx/compatibility.hpp>

void KeyframeCollection::add(float time, const GeometricTransformation& transformation)
{
    m_keyframes.insert(std::make_pair(time, transformation));
}

glm::mat4 KeyframeCollection::interpolateTransformation(float time) const
{
    //TODO: Complete the interpolation framework
    if (!m_keyframes.empty()) {
        //Handle the case where the time parameter is outside the keyframes time scope.
        std::map<float, GeometricTransformation>::const_iterator itFirstFrame = m_keyframes.begin();
        std::map<float, GeometricTransformation>::const_reverse_iterator itLastFrame = m_keyframes.rbegin();
        if (time <= itFirstFrame->first)
            return itFirstFrame->second.toMatrix();
        if (time >= itLastFrame->first)
            return itLastFrame->second.toMatrix();

        //Get keyframes surrounding the time parameter
        std::array<Keyframe, 2> result = getBoundingKeyframes(time);
        float time0 = result[0].first;
        float time1 = result[1].first;
        GeometricTransformation transform0 = result[0].second;
        GeometricTransformation transform1 = result[1].second;

        // TODO:
        // Interpolate the transformation given (time0, transform0)
        // and (time1, transform1).
        // Build and return the mat4 matrix.

        //TODO: Compute the interpolating factor based on the time parameter and the surrounding keyframes times.
        //        float factor = 0.0;
        float factor = (time - time0) / (time1 - time0);

        //TODO: Interpolate each transformation component of the surrounding keyframes: orientation, translation, scale
        //      Use spherical linear interpolation for the orientation interpolation, glm::slerp(value1, value2, factor);
        //      Use linear interpolation for the translation and scale, glm::lerp(value1, value2, factor);
        //        glm::quat orientation = glm::quat(1, 0, 0, 0);
        //        glm::vec3 translation = glm::vec3(0,0,0);
        //        glm::vec3 scale = glm::vec3(1,1,1);
        glm::quat orientation = glm::slerp(transform0.getOrientation(), transform1.getOrientation(), factor);
        glm::vec3 translation = glm::lerp(transform0.getTranslation(), transform1.getTranslation(), factor);
        glm::vec3 scale = glm::lerp(transform0.getScale(), transform1.getScale(), factor);

        //Build a matrix transformation from the orientation, translation and scale components
        return GeometricTransformation(translation, orientation, scale).toMatrix();
    } else {
        return glm::mat4(1.0);
    }
}

bool KeyframeCollection::empty() const
{
    return m_keyframes.empty();
}

std::array< KeyframeCollection::Keyframe, 2 > KeyframeCollection::getBoundingKeyframes(float time) const
{
    std::array<KeyframeCollection::Keyframe, 2> result {
        std::make_pair(0, GeometricTransformation()),
        std::make_pair(0, GeometricTransformation()) };
    std::map<float, GeometricTransformation>::const_iterator upper = m_keyframes.upper_bound(time);
    std::map<float, GeometricTransformation>::const_iterator lower = std::prev(upper);
    std::map<float, GeometricTransformation>::const_iterator end = m_keyframes.end();
    if (upper != end && lower != end) {
        result[0] = *lower;
        result[1] = *upper;
    }
    return result;
}
