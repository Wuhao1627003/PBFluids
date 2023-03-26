#include "Cell.h"

void Cell::updateNeighbors(const vector<Cell> &gridCellsPtr)
{
	this->neighborParticleIDs.clear();
	for (int neighborCellIdx : this->neighborCellIdxs) {
		Cell neighborCell = gridCellsPtr[neighborCellIdx];
		this->neighborParticleIDs.insert(this->neighborParticleIDs.end(), neighborCell.particleIDs.begin(), neighborCell.particleIDs.end());
	}
}
