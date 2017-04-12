#ifndef SPHERE_RENDERABLE_HPP
#define SPHERE_RENDERABLE_HPP

#include "./HierarchicalRenderable.hpp"
#include <vector>
#include <glm/glm.hpp>

/* Stand-alone class, without inheritance from existing objects */

class SphereRenderable : public HierarchicalRenderable
{
public :
    SphereRenderable(ShaderProgramPtr shaderProgram, glm::vec4 color, int strips, int slices);
    ~SphereRenderable();

private:
    void do_draw();
    void do_animate(float dTime, float time);

    std::vector< glm::vec3 > m_positions;
    std::vector< glm::vec3 > m_normals;
    std::vector< glm::vec4 > m_colors;

    unsigned int m_pBuffer;
    unsigned int m_nBuffer;
    unsigned int m_cBuffer;
};

typedef std::shared_ptr<SphereRenderable> SphereRenderablePtr;

#endif
