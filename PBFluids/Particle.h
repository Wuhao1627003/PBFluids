#pragma once
#include "Utils/vec.h"
using namespace std;

class Particle
{
public:
	long ID;
	int cellIdx;
	vec3 pos, vel;
	float density, gradNorm, lambda;

	Particle(long ID, int cellIdx, const vec3 &pos, const vec3 &vel, float density) {};
	// update the particle's position and velocity
	void step(float dt);
};

