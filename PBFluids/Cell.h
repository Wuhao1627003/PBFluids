#pragma once
#include <vector>
#include <tuple>
using namespace std;

typedef tuple<int, int, int> coord;

class Cell
{
public:
	coord cellCoord;
	vector<long> particleIDs;
	vector<int> neighborCellIdxs;
	bool isBoundary;

	Cell();
};

