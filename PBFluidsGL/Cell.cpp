#include "Cell.h"

void Cell::updateNeighbors()
{
	this->neighborParticleIDs.clear();
	for (int neighborCellIdx : this->neighborCellIdxs) {
		Cell neighborCell = (*(this->gridCellsPtr))[neighborCellIdx];
		this->neighborParticleIDs.insert(this->neighborParticleIDs.end(), neighborCell.neighborParticleIDs.begin(), neighborCell.neighborParticleIDs.end());
	}
}
