#include "./../../include/dynamics/ParticlePlaneCollision.hpp"

ParticlePlaneCollision::ParticlePlaneCollision(ParticlePtr particle, PlanePtr plane, float restitution) :
    Collision(restitution)
{
    m_particle = particle;
    m_plane = plane;
}

ParticlePlaneCollision::~ParticlePlaneCollision()
{}

void ParticlePlaneCollision::do_solveCollision()
{
    //Don't process fixed particles (ground plane is assumed fixed)
    if (m_particle->isFixed())
        return;

    //TODO: Solve ParticlePlane collisions,
    // update particle position and velocity after collision
    //
    //Functions to use:
    //glm::dot(v1, v2): Return the dot product of two vector.
    //Plane::distanceToOrigin(): Return the distance to origin from the plane
    //Plane::normal(): Return the normal of the plane
    //Particle::getRadius(), Particle::getPosition(), Particle::getVelocity(), Particle::setPosition(), Particle::setVelocity()

    float d2Plane = glm::length(glm::dot((m_particle->getPosition() - m_plane->projectOnPlane(m_particle->getPosition())), m_plane->normal()));

    if (d2Plane <= m_particle->getRadius()) {
      m_particle->setPosition( m_particle->getPosition() - (d2Plane - m_particle->getRadius()) * m_plane->normal() );
      glm::vec3 prev_v = m_particle->getVelocity();
      m_particle->setVelocity( prev_v - (1 + m_restitution) * glm::dot(prev_v, m_plane->normal()) * m_plane->normal() );
    }
}



bool testParticlePlane(const ParticlePtr &particle, const PlanePtr &plane)
{
    /* Equation of a plane passing through A and normal to n:
   * dot( p - A, n ) = dot( p, n ) - dot( A, n ) = 0
   * dot( A, n ) is stored in the "distanceToOrigin" of the plane.
   *
   * Equation of a particle of radius r centered in c:
   * dot( p - c, p - c ) = r²
   *
   * distance( plane, particle )
   *   = min( distance( plane, c ) - r, 0 )              //definition
   *   = min( abs( dot( c - A, n ) ) - r, 0 )
   *   = min( abs( dot( c, n ) - dot( A, n ) ) - r, 0 )
   *
   * So, there is intersection if distance( plane, particle ) = 0
   * <=> abs( dot( c, n ) - dot( A, n ) ) - r <= 0
   * <=> abs( dot( c, n ) - dot( A, n ) ) <= r
   */

    //TODO: Test collision between particle and plane
    //Functions to use:
    //glm::dot(v1, v2): Return the dot product of two vector.
    //Plane::distanceToOrigin(): Return the distance to origin from the plane
    //Plane::normal(): Return the normal of the plane
    //Particle::getRadius(), Particle::getPosition()

    return abs( glm::dot(particle->getPosition(), plane->normal()) - plane->distanceToOrigin() ) <= particle->getRadius();
}
