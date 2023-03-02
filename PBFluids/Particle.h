#pragma once
#include <tuple>
using namespace std;
class Particle
{
public:
	long ID;
	tuple<int> pos, vel, cellIdx;
	float density, gradNorm, lambda;
	void step(float dt);
};

