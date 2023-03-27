#include "Particle.h"

void Particle::preprocess(float dt)
{
	this->vel += vec3(0., 0., -2) * dt;
	this->posPredicted = this->pos + this->vel * dt;
}

vector<long> Particle::findNeighborIDs(const Cell &c, const vector<Particle> &particles)
{
	return c.neighborParticleIDs;
}

void Particle::postprocess(float dt)
{
	this->vel = (this->posPredicted - this->pos) / dt;

	// TODO: vorticity & viscosity

	this->pos = this->posPredicted;
}