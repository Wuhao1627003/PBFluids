#pragma once
#include "Cell.h"
#include "Particle.h"
#include "kernel.h"
#include <unordered_map>
#include <string>
#include "Scene.h"
#include <wof_api_functions.h>

using namespace std;

#define numIter 10
#define cfmEpsilon 60.0    // Constraint Force Mixing Relaxation
#define kTensile 0.1
#define nTensile 4
#define cellSize 0.5

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
	vector<vector<long>> allNeighborIDs;
	int width, height, worldWidth, worldHeight;
	long numParticles;
	float radius, particleMass;
	std::vector<GEOM_WOF::Point3> particleCenters;
		
	Grid() {};
	Grid(int inWidth, int inHeight, float mass, float density, float viscosity, long numParticles, float dt, float radius):
		worldWidth(inWidth), worldHeight(inHeight), width((int) (inWidth / cellSize)), height((int) (inHeight / cellSize)), particleMass(mass), density(density), viscosity(viscosity), dt(dt), radius(radius), numParticles(numParticles)
	Grid(int width, int height, float mass, float viscosity, long numParticles, float dt, float radius):
		width(width), height(height), particleMass(mass), viscosity(viscosity), dt(dt), radius(radius), numParticles(numParticles)
	{
		gridCells.clear();
		particles.clear();
		allNeighborIDs.clear();
		cellCoordMap.clear();

		initCellCoordMap();
		initParticles();
		initCells();
	};
	Grid(int inWidth, int inHeight, float mass, float density, float viscosity, float dt, float radius, const std::vector<GEOM_WOF::Point3> &points) :
		worldWidth(inWidth), worldHeight(inHeight), width((int) (inWidth / cellSize)), height((int) (inHeight / cellSize)), particleMass(mass), density(density), viscosity(viscosity), dt(dt), radius(radius), particleCenters(points)
	Grid(int width, int height, float mass, float viscosity, float dt, float radius, const std::vector<GEOM_WOF::Point3> &points) :
		width(width), height(height), particleMass(mass), viscosity(viscosity), dt(dt), radius(radius), particleCenters(points)
	{
		gridCells.clear();
		particles.clear();
		allNeighborIDs.clear();
		cellCoordMap.clear();

		initCellCoordMap();
		numParticles = particleCenters.size();
		initParticlesFromMesh();
		initCells();
	};
	void operator=(const Grid &g)
	{
		this->width = g.width;
		this->height = g.height;
		this->worldHeight = g.worldHeight;
		this->worldWidth = g.worldWidth;
		this->particleMass = g.particleMass;
		this->density = g.density;
		this->viscosity = g.viscosity;
		this->dt = g.dt;
		this->radius = g.radius;
		this->numParticles = g.numParticles;
		this->gridCells = g.gridCells;
		this->particles = g.particles;
		this->allNeighborIDs = g.allNeighborIDs;
		this->cellCoordMap = g.cellCoordMap;
	}
	// initialize cellCoordMap
	void initCellCoordMap();
	// initialize all properties of particles
	void initParticles();
	void initParticlesFromMesh();
	// initialize all properties of cells, and populate with particles
	void initCells();
	// update the cell that the particle is in
	void updateParticleCell(Particle &p);
	// go forward one time step for all particles and cells
	void step();
	// add container to grid
	void addContainer(const vector<vec3> &triangles);

private:
	float density, dt, viscosity;
	// map of cell coordinates to cell index
	unordered_map<vec3, int, Hash> cellCoordMap;
	const float tensileStabilityDenom = pow(kernel_poly6(0.2), nTensile);
	Scene scene;

	int computeCellIdx(int cellx, int celly, int cellz);
};
