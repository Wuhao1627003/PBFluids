#pragma once
#include "Utils/vec.h"
#include "Cell.h"
#include "kernel.h"
#include <vector>
using namespace std;

class Particle
{
public:
	long ID;
	int cellIdx, numBounces = 0;
	vec3 pos, posPredicted, deltaP;
	vec3 vel = vec3(0., 0., 0.);
	float density, gradNorm, lambda, constraint;
	vec3 omega, eta, N;

	Particle() {};
	Particle(long ID, int cellIdx, const vec3 &pos) : ID(ID), cellIdx(cellIdx), pos(pos) {};

	// external force and predict position
	void preprocess(float dt);
	// find neighbor particles
	vector<long> findNeighborIDs(const Cell &c, const vector<Particle> &particles);
	// reset intermediate values
	void reset();
	// update velocity and position
	void postprocess(float dt, vector<long> neighborIDs, vector<Particle>& particles);
};

