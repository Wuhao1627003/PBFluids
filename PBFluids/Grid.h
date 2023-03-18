#pragma once
#include "Cell.h"
#include "Particle.h"
#include <map>
using namespace std;
static class Grid
{
public:
	vector<Cell> gridCells;
	vector<Particle> particles;
	int width, height;
	long numParticles;
	float radius;
		
	Grid(int width, int height, float density, float viscosity, long numParticles, float dt, float radius):
		width(width), height(height), density(density), viscosity(viscosity), dt(dt), radius(radius), numParticles(numParticles)
	{};
	~Grid();
	void updateParticleCell(Particle *p);
	void initParticles();
	void initCells();
	void step();

private:
	float density, dt, viscosity;
	map<coord, int> cellCoordMap;

	void initGrid();
};

