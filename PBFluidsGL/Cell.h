#pragma once
#include <vector>
#include <tuple>
#include "Utils/vec.h"

using namespace std;

class Cell
{
public:
	vec3 cellCoord;
	vector<long> particleIDs;
	vector<long> neighborParticleIDs;
	vector<int> neighborCellIdxs;
	// whether this cell is a boundary cell
	bool isBoundary;

	Cell() {};
	Cell(const vec3 &cellCoord) : cellCoord(cellCoord) {};
	// update the neighbor particles of this cell
	void updateNeighbors(const vector<Cell> &gridCellsPtr);
};

