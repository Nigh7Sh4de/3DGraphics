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
    void do_animate(float dTime, float time);

    float calculateTerrainHeight(int x, int y);
    glm::vec3 calculateTerrainNormal(int x, int y);

    static const int detail = 15;
    static const int detail_v = (detail*2-1)*(detail-1)+1;
    static const int detail_g = detail*2-1;
    static const int detail_t = detail/2;
    static const int detail_s = 8;


    float m_positions[detail_v*3];
    float m_normals[detail_v*3];
    float m_colors[detail_v*4];
    // std::vector<glm::vec3> m_positions;
    // std::vector<glm::vec4> m_colors;
    // std::vector<glm::vec3> m_normals;

    unsigned int m_pBuffer;
    unsigned int m_cBuffer;
    unsigned int m_nBuffer;
};

typedef std::shared_ptr<TerrainRenderable> TerrainRenderablePtr;

#endif //PLANE_RENDERABLE_HPP
