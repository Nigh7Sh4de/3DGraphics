#ifndef TEACHERS_INDEXED_CUBE_RENDERABLE_HPP
#define TEACHERS_INDEXED_CUBE_RENDERABLE_HPP

#include "../include/HierarchicalRenderable.hpp"
#include <vector>
#include <glm/glm.hpp>

namespace teachers {

class IndexedCubeRenderable : public HierarchicalRenderable
{
    public:
        ~IndexedCubeRenderable();
        IndexedCubeRenderable();
        IndexedCubeRenderable(ShaderProgramPtr program);

    protected:
        void do_draw();
        void do_animate(float time);

    private:
        std::vector<glm::vec3> m_positions;
        std::vector<glm::vec4> m_colors;
        std::vector<glm::vec3> m_normals;
        std::vector<glm::ivec3> m_indices;

        unsigned int m_pBuffer;
        unsigned int m_cBuffer;
        unsigned int m_nBuffer;
        unsigned int m_iBuffer;
};

}
// no Ptr typedef here, a bit too naughty in a namespace...
#endif
