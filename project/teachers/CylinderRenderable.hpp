#ifndef TEACHERS_CYLINDER_RENDERABLE_HPP
#define TEACHERS_CYLINDER_RENDERABLE_HPP

#include "../include/HierarchicalRenderable.hpp"
#include <vector>
#include <glm/glm.hpp>

namespace teachers
{

class CylinderRenderable : public HierarchicalRenderable
{
  public:
    ~CylinderRenderable();
    CylinderRenderable(ShaderProgramPtr program, bool normalPerVertex = true, unsigned int nbSlices = 20,
                       glm::vec4 color = glm::vec4(-1.0));

  protected:
    void do_draw();
    void do_animate(float dTime, float time);

  private:
    bool m_normalPerVertex;
    void buildNormalPerVertex(unsigned int nbSlices);
    void buildNormalPerFace(unsigned int nbSlices);

    std::vector<glm::vec3> m_positions;
    std::vector<glm::vec4> m_colors;
    std::vector<glm::vec3> m_normals;
    std::vector<glm::ivec3> m_indices;

    glm::vec4 color;

    unsigned int m_pBuffer;
    unsigned int m_cBuffer;
    unsigned int m_nBuffer;
    unsigned int m_iBuffer;
};
}
// no Ptr typedef here, a bit too naughty in a namespace...
#endif
