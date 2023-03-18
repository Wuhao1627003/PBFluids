#pragma once
#include <tuple>
using namespace std;

typedef tuple<float, float, float> vec3;

class Particle
{
public:
	long ID;
	int cellIdx;
	vec3 pos, vel;
	float density, gradNorm, lambda;

	void step(float dt);
	Particle(long ID, int cellIdx, const vec3 &pos, const vec3 &vel, float density, float gradNorm, float lambda);
};

