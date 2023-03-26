#pragma once
#include "Utils/vec.h"
#include "Cell.h"
#include <vector>
using namespace std;

class Particle
{
public:
	long ID;
	int cellIdx;
	vec3 pos, posPredicted;
	vec3 vel = vec3(0.1, 0.1, 0.);
	float density, gradNorm, lambda;
	vector<Particle> neighborParticles;

	Particle() {};
	Particle(long ID, int cellIdx, const vec3 &pos, float density) : ID(ID), cellIdx(cellIdx), pos(pos), density(density) {};

	// external force and predict position
	void preprocess(float dt);
	// find neighbor particles
	void findNeighbors(const Cell &c, const vector<Particle> &particles);
	// update velocity and position
	void postprocess(float dt);
};

