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
	vector<int> neighborCellIdxs;
	// whether this cell is a boundary cell
	bool isBoundary;

	Cell(const vec3 &cellCoord) : cellCoord(cellCoord) {};
	// update the neighbor cells of this cell
	void updateNeighbors(const vector<int> &neighborCellIdxs);
};

