#include "Cell.h"

// used for newly created cells, update neighborCellIdxs for itself and its neighbors
Cell::Cell(const coord &cellCoord, const vector<long> &particleIDs, const vector<int> &neighborCellIdxs, bool isBoundary)
	: cellCoord(cellCoord)
	, particleIDs(particleIDs)
	, neighborCellIdxs(neighborCellIdxs)
	, isBoundary(isBoundary)
{}
