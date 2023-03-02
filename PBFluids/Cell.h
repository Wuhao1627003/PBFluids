#pragma once
#include <vector>
using namespace std;
class Cell
{
public:
	vector<long> particleIDs;
	vector<int> neighborCellIdxs;
	bool isBoundary;
};

