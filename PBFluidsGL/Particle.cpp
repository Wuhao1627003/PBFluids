#include "Particle.h"

void Particle::preprocess(float dt)
{
	this->posPredicted = this->pos + this->vel * dt;
	if (this->ID == 0) {
		auto test = this->posPredicted;
	}
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
	this->pos = this->posPredicted;
}