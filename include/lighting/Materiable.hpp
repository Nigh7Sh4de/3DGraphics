#ifndef MATERIABLE_HPP
#define MATERIABLE_HPP

#include "Material.hpp"
#include <vector>
#include <glm/glm.hpp>

class Materiable
{
public:
    ~Materiable();
    Materiable(const MaterialPtr& material);

    void setMaterial(const MaterialPtr& material);
    const MaterialPtr& getMaterial() const;

private:
    MaterialPtr m_material;
};

#endif
