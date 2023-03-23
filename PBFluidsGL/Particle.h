#pragma once
#include "Utils/vec.h"
using namespace std;

class Particle
{
public:
	long ID;
	int cellIdx;
	vec3 pos;
	vec3 vel = vec3(0., 0., 0.);
	float density, gradNorm, lambda;

	Particle() {};
	Particle(long ID, int cellIdx, const vec3 &pos, float density) : ID(ID), cellIdx(cellIdx), pos(pos), density(density) {};
	// update the particle's position and velocity
	void step(float dt);
};

