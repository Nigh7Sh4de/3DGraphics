#ifndef TEXTURED_MESH_RENDERABLE_HPP
#define TEXTURED_MESH_RENDERABLE_HPP

#include "./../HierarchicalRenderable.hpp"
#include "./../lighting/Material.hpp"
#include "../lighting/LightedMeshRenderable.hpp"

#include <string>
#include <vector>
#include <glm/glm.hpp>

/* Stand-alone class, without inheritance from existing objects */

class TexturedMeshRenderable : public HierarchicalRenderable
{
public:
    ~TexturedMeshRenderable();
    TexturedMeshRenderable(ShaderProgramPtr program,
        const std::string& meshFilename,
        const std::string& textureFilename);
    void setMaterial(const MaterialPtr& material);

protected:
    void do_draw();
    void do_animate(float time);

    std::vector<glm::vec3> m_positions;
    std::vector<glm::vec3> m_normals;
    std::vector<unsigned int> m_indices;
    std::vector<glm::vec2> m_texCoords;

    unsigned int m_pBuffer;
    unsigned int m_nBuffer;
    unsigned int m_iBuffer;
    unsigned int m_tBuffer;

    unsigned int m_texId;

    MaterialPtr m_material;
};

typedef std::shared_ptr<TexturedMeshRenderable> TexturedMeshRenderablePtr;

#endif
