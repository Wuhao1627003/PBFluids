#pragma once
#include "Cell.h"
#include "Particle.h"
#include <map>
using namespace std;
class Grid
{
public:
	vector<Cell> gridCells;
	vector<Particle> particles;
	int width, height;
	long numParticles;
	float radius;
		
	Grid() {};
	Grid(int width, int height, float density, float viscosity, long numParticles, float dt, float radius):
		width(width), height(height), density(density), viscosity(viscosity), dt(dt), radius(radius), numParticles(numParticles)
	{
		initParticles();
		initCells();
	};
	// initialize all properties of particles
	void initParticles();
	// initialize all properties of cells, and populate with particles
	void initCells();
	// update the cell that the particle is in
	void updateParticleCell(Particle *p);
	// go forward one time step for all particles and cells
	void step();

private:
	float density, dt, viscosity;
	// map of cell coordinates to cell index
	map<vec3, int> cellCoordMap;
};
