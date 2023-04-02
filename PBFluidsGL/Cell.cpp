#include "Cell.h"

void Cell::updateNeighbors(const vector<Cell> &gridCellsPtr)
{
	this->neighborParticleIDs.clear();
	this->neighborParticleIDs.insert(this->neighborParticleIDs.end(), this->particleIDs.begin(), this->particleIDs.end());
	for (int neighborCellIdx : this->neighborCellIdxs) {
		this->neighborParticleIDs.insert(this->neighborParticleIDs.end(), gridCellsPtr[neighborCellIdx].particleIDs.begin(), gridCellsPtr[neighborCellIdx].particleIDs.end());
	}
}
