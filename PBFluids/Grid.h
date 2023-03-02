#pragma once
#include "Cell.h"
#include "Particle.h"
static class Grid
{
public:
	Cell *gridCells;
	Particle *particles;
		
	Grid();
	Grid(int width, int height, float density, float viscosity, long numParticles, float dt, float radius):
		width(width), height(height), density(density), viscosity(viscosity), dt(dt), radius(radius), numParticles(numParticles)
	{
		Grid();
	};
	~Grid();
	void updateParticleCell(Particle *p);
	void initParticles();
	void initCells();
	void step();

private:
	int width, height;
	float density, dt, radius, viscosity;
	long numParticles;
};

