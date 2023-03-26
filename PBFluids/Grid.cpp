#include "Grid.h"

void Grid::initCellCoordMap()
{
	this->gridCells.resize(width * width * height);
	int cellx, celly, cellz, cellIdx;
	for (cellx = 0; cellx < width; cellx++) {
		for (celly = 0; celly < width; celly++) {
			for (cellz = 0; cellz < height; cellz++) {
				cellIdx = computeCellIdx(cellx, celly, cellz);
				cellCoordMap.insert(pair<vec3, int>(vec3(cellx, celly, cellz), cellIdx));
				this->gridCells[cellIdx] = Cell(vec3(cellx, celly, cellz), &(this->gridCells));
			}
		}
	}
}

void Grid::initParticles()
{
	this->particles.resize(numParticles);
	float x, y, z;
	int cellIdx;
	for (long particleID = 0; particleID < numParticles; particleID++) {
		x = ((float) rand() * width) / RAND_MAX;
		y = ((float) rand() * width) / RAND_MAX;
		z = ((float) rand() * height) / RAND_MAX;
		cellIdx = cellCoordMap.find(vec3((int) x, (int) y, (int) z))->second;
		this->particles[particleID] = Particle(particleID, cellIdx, vec3(x, y, z), density);
		this->gridCells[cellIdx].particleIDs.push_back(particleID);
	}
}

void Grid::initCells()
{
	int cellx, celly, cellz, cellIdx;
	unordered_map<vec3, int>::iterator it[6];
	for (cellx = 0; cellx < width; cellx++) {
		for (celly = 0; celly < width; celly++) {
			for (cellz = 0; cellz < height; cellz++) {
				cellIdx = cellCoordMap.find(vec3(cellx, celly, cellz))->second;
				// check if this cell is a boundary cell
				if (cellx == 0 || cellx == width - 1 || celly == 0 || celly == width - 1 || cellz == 0 || cellz == height - 1) {
					this->gridCells[cellIdx].isBoundary = true;
				}
				else {
					this->gridCells[cellIdx].isBoundary = false;
				}

				it[0] = cellCoordMap.find(vec3(cellx, celly, cellz + 1));
				it[1] = cellCoordMap.find(vec3(cellx, celly, cellz - 1));
				it[2] = cellCoordMap.find(vec3(cellx, celly + 1, cellz));
				it[3] = cellCoordMap.find(vec3(cellx, celly - 1, cellz));
				it[4] = cellCoordMap.find(vec3(cellx + 1, celly, cellz));
				it[5] = cellCoordMap.find(vec3(cellx - 1, celly, cellz));

				for (int i = 0; i < 6; i++) {
					if (it[i] != cellCoordMap.end()) {
						this->gridCells[cellIdx].neighborCellIdxs.push_back(it[i]->second);
						auto neighborParticleIDs = this->gridCells[it[i]->second].particleIDs;
						this->gridCells[cellIdx].neighborParticleIDs.insert(this->gridCells[cellIdx].neighborParticleIDs.end(), neighborParticleIDs.begin(), neighborParticleIDs.end());
					}
				}
			}
		}
	}
}

void Grid::updateParticleCell(Particle &p)
{}

void Grid::step()
{

}

int Grid::computeCellIdx(int cellx, int celly, int cellz)
{
	return cellx + celly * width + cellz * width * width;
}