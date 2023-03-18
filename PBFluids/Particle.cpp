#include "Particle.h"

void Particle::step(float dt)
{}


Particle::Particle(long ID, int cellIdx, const vec3 &pos, const vec3 &vel, float density, float gradNorm, float lambda)
	: ID(ID)
	, cellIdx(cellIdx)
	, pos(pos)
	, vel(vel)
	, density(density)
	, gradNorm(gradNorm)
	, lambda(lambda)
{}
