#pragma once
#include "Cell.h"
#include "Particle.h"
#include <unordered_map>
#include <string>

using namespace std;

struct Hash
{
	std::size_t operator()(const vec3 &v) const
	{
		using std::size_t;
		using std::hash;
		using std::string;

		// Compute individual hash values for first,
		// second and third and combine them using XOR
		// and bit shifting:

		return (hash<string>()("(" + to_string(v[0]) + ", " + to_string(v[1]) + ", " + to_string(v[2]) + ") "));
	}
};

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
		initCellCoordMap();
		initParticles();
		initCells();
	};
	// initialize cellCoordMap
	void initCellCoordMap();
	// initialize all properties of particles
	void initParticles();
	// initialize all properties of cells, and populate with particles
	void initCells();
	// update the cell that the particle is in
	void updateParticleCell(Particle &p);
	// go forward one time step for all particles and cells
	void step();

private:
	float density, dt, viscosity;
	// map of cell coordinates to cell index
	unordered_map<vec3, int, Hash> cellCoordMap;

	int computeCellIdx(int cellx, int celly, int cellz);
};
