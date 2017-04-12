#ifndef RAY_CANEVAS_HPP
#define RAY_CANEVAS_HPP

#include "HierarchicalRenderable.hpp"
#include <vector>
#include <glm/glm.hpp>


class RayCanvas : public HierarchicalRenderable
{
    public:
        ~RayCanvas();
        RayCanvas(ShaderProgramPtr programe, int cube_map_resol = 2048);

    private:
        void do_draw();
        void do_animate(float dTime, float time);

        std::vector<glm::vec2> m_positions;
        std::vector<unsigned int> m_indices;

        unsigned int m_pBuffer;
        unsigned int m_iBuffer;
        
        unsigned int m_texId;
        unsigned int m_texScratchId;
        int max_mipmap;
	float m_animTime;

        void createHDRskyBox(int);
};


#endif //RAY_CANEVAS_HPP
