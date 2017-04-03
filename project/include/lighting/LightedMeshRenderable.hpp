#ifndef LIGHTED_MESH_RENDERABLE_HPP
#define LIGHTED_MESH_RENDERABLE_HPP

#include "./../lighting/Materiable.hpp"
#include "./../../teachers/MeshRenderable.hpp"

#include <string>

class LightedMeshRenderable : public teachers::MeshRenderable, public Materiable
{
public:
    ~LightedMeshRenderable();
    LightedMeshRenderable(ShaderProgramPtr program,
        const std::string& filename, const MaterialPtr& material);

protected:
    void do_draw();
    void do_animate(float time);
};

typedef std::shared_ptr<LightedMeshRenderable> LightedMeshRenderablePtr;

#endif
