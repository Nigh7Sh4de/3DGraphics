#include "./../../include/dynamics/SpringForceField.hpp"

SpringForceField::SpringForceField(const ParticlePtr p1, const ParticlePtr p2,
                                   float stiffness, float equilibriumLength,
                                   float damping) :
    m_p1(p1),
    m_p2(p2),
    m_stiffness(stiffness),
    m_equilibriumLength(equilibriumLength),
    m_damping(damping)
{}

void SpringForceField::do_addForce()
{
    //TODO: Implement a damped spring
    //Functions to use:
    //glm::length( vec3 ): Return the length of a vector
    //glm::normalize( vec3 ): Return the normalization of a vector
    //Particle::getVelocity(), Particle::getForce(), Particle::setForce()
    //
    //Nb:   Compute force ONLY IF the displacement length is above std::numeric_limits<float>::epsilon()
    //      Otherwise the computation is useless

    ParticlePtr p1 = getParticle1();
    ParticlePtr p2 = getParticle2();

    float length = glm::length(p1->getPosition() - p2->getPosition());

    if (length > std::numeric_limits<float>::epsilon()){
      glm::vec3 norm = normalize(p1->getPosition() - p2->getPosition());

      glm::vec3 force_ideal = -m_stiffness * (length - m_equilibriumLength) * norm;
      glm::vec3 force_damped = -m_damping * glm::dot( p1->getVelocity() - p2->getVelocity(), norm ) * norm;
      glm::vec3 force_total = force_ideal + force_damped;

      p1->incrForce(force_total);
      p2->incrForce(-force_total);
    }
}

ParticlePtr SpringForceField::getParticle1() const
{
    return m_p1;
}

ParticlePtr SpringForceField::getParticle2() const
{
    return m_p2;
}
