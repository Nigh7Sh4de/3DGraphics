#ifndef RAY_CANEVAS_HPP
#define RAY_CANEVAS_HPP

#include "HierarchicalRenderable.hpp"
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

class Skybox : public HierarchicalRenderable
{
    public:
        ~Skybox();
        Skybox(ShaderProgramPtr program);

    private:
        void do_draw();
        void do_animate(float time);

        GLuint skyboxVAO, skyboxVBO;
        
        unsigned int m_texId;
    	float m_animTime;

        void createSkyBox();
        void loadCubemap(std::vector<const GLchar *>);
};


#endif //RAY_CANEVAS_HPP
