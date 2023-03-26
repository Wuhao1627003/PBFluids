#include "Particle.h"

void Particle::preprocess(float dt)
{
	this->vel += vec3(0., -9.8, 0.) * dt;
	this->posPredicted = this->pos + this->vel * dt;
}

void Particle::findNeighbors(const Cell &c, const vector<Particle> &particles)
{
	this->neighborParticles.clear();
	this->neighborParticles.resize(c.neighborParticleIDs.size());
	for (int i = 0; i < c.neighborParticleIDs.size(); i++) {
		this->neighborParticles[i] = particles[c.neighborParticleIDs[i]];
	}
}

void Particle::postprocess(float dt)
{
	this->vel = (this->posPredicted - this->pos) / dt;

	// TODO: vorticity & viscosity

	this->pos = this->posPredicted;
}