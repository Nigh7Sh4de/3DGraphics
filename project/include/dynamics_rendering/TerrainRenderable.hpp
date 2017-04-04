#ifndef TERRAIN_RENDERABLE_HPP
#define TERRAIN_RENDERABLE_HPP

#include "../HierarchicalRenderable.hpp"
#include "../dynamics/Plane.hpp"

#include <vector>
#include <glm/glm.hpp>

/**@brief Render terrain on the screen. */
class TerrainRenderable : public HierarchicalRenderable
{
public:
    ~TerrainRenderable();
    TerrainRenderable(ShaderProgramPtr program);

private:
    void do_draw();
    void do_animate(float time);

    float m_positions[9];
    // std::vector<glm::vec3> m_positions;
    std::vector<glm::vec4> m_colors;
    std::vector<glm::vec3> m_normals;

    unsigned int m_pBuffer;
    unsigned int m_cBuffer;
    unsigned int m_nBuffer;
};

typedef std::shared_ptr<TerrainRenderable> TerrainRenderablePtr;

#endif //PLANE_RENDERABLE_HPP
