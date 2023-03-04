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
};

