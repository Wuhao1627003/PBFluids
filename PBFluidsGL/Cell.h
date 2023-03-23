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
	vector<Cell>* gridCellsPtr;

	Cell() {};
	Cell(const vec3 &cellCoord, vector<Cell> *gridCellsPtr) : cellCoord(cellCoord), gridCellsPtr(gridCellsPtr) {};
	// update the neighbor particles of this cell
	void updateNeighbors();
};

