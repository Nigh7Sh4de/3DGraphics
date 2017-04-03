#include "./../../include/lighting/Materiable.hpp"

Materiable::Materiable(const MaterialPtr& material)
{
    setMaterial(material);
}

Materiable::~Materiable()
{
}

void Materiable::setMaterial(const MaterialPtr& material)
{
    m_material = material;
}

const MaterialPtr& Materiable::getMaterial() const
{
    return m_material;
}
