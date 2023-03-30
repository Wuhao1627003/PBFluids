#include "Grid.h"
#include <GL/freeglut_std.h>

void Grid::initCellCoordMap()
{
	this->gridCells.resize(width * width * height);
	int cellx, celly, cellz, cellIdx;
	for (cellx = 0; cellx < width; cellx++) {
		for (celly = 0; celly < width; celly++) {
			for (cellz = 0; cellz < height; cellz++) {
				cellIdx = computeCellIdx(cellx, celly, cellz);
				cellCoordMap.insert(pair<vec3, int>(vec3(cellx, celly, cellz), cellIdx));
				this->gridCells[cellIdx] = Cell(vec3(cellx, celly, cellz));
			}
		}
	}
}

void Grid::initParticles()
{
	this->particles.resize(numParticles);
	this->allNeighborIDs.resize(numParticles);
	float x, y, z;
	int cellIdx;
	for (long particleID = 0; particleID < numParticles; particleID++) {
		x = (float)rand() / RAND_MAX * width;
		y = (float)rand() / RAND_MAX * width;
		z = (float)rand() / RAND_MAX * height;
		cellIdx = cellCoordMap.find(vec3((int) x, (int) y, (int) z))->second;
		this->particles[particleID] = Particle(particleID, cellIdx, vec3(x, y, z));
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
{
	unordered_map<vec3, int>::iterator it = cellCoordMap.find(vec3((int) p.pos[0], (int) p.pos[1], (int) p.pos[2]));
	if (it == cellCoordMap.end()) {
		return;
	}
	int cellIdx = it->second;
	if (p.cellIdx != cellIdx) {
		// remove particle from old cell
		this->gridCells[p.cellIdx].particleIDs.erase(remove(this->gridCells[p.cellIdx].particleIDs.begin(), this->gridCells[p.cellIdx].particleIDs.end(), p.ID), this->gridCells[p.cellIdx].particleIDs.end());
		// add particle to new cell
		this->gridCells[cellIdx].particleIDs.push_back(p.ID);
		p.cellIdx = cellIdx;
	}
}

void Grid::step()
{
#pragma omp parallel for
	for (int i = 0; i < particles.size(); i++) {
		particles[i].preprocess(dt);
	}
#pragma omp parallel for
	for (int i = 0; i < particles.size(); i++) {
		allNeighborIDs[i] = particles[i].findNeighborIDs(this->gridCells[particles[i].cellIdx], particles);
	}

	// TODO: main solver loop
	for (size_t iter = 0; iter < numIter; iter++) {
		// calculate lambdas
#pragma omp parallel for
		for (int i = 0; i < particles.size(); i++) {
			particles[i].reset();
		}
			
		// ----------- Computed Constraints ---------
		for (long p_i = 0; p_i < particles.size(); p_i++) {
			vector<long> neighbours = allNeighborIDs[p_i];
			for (long p_j : neighbours) {
				particles[p_i].density += particleMass * kernel_poly6(particles[p_i].posPredicted, particles[p_j].posPredicted);

				// accumulate gradient norm
				vec3 gradTemp = vec3(0, 0, 0);
				if (p_i == p_j) {
					for (long p_k : neighbours) {
						gradTemp += kernel_spiky(particles[p_i].posPredicted, particles[p_k].posPredicted);
					}
				}
				else {
					gradTemp = kernel_spiky(particles[p_i].posPredicted, particles[p_j].posPredicted);
				}

				particles[p_i].gradNorm += (gradTemp / this->density).Length();
			}

			// Compute constraint and lambda
			double constraintVal = (particles[p_i].density / this->density) - 1;
			particles[p_i].lambda = -constraintVal / (particles[p_i].gradNorm + cfmEpsilon);
		}

		// ----------- Computed Position Correction ---------
		// update delta p
		for (int p_i = 0; p_i < particles.size(); p_i++) {
			for (int p_j : allNeighborIDs[p_i]) {
				vec3 spikyKernel = kernel_spiky(particles[p_i].posPredicted, particles[p_j].posPredicted);
				float s_corr = -kTensile * pow(kernel_poly6(particles[p_i].posPredicted, particles[p_j].posPredicted), nTensile) / tensileStabilityDenom;
				particles[p_i].deltaP += (particles[p_i].lambda + particles[p_j].lambda + s_corr) * spikyKernel;
			}
			particles[p_i].deltaP /= this->density;
			particles[p_i].posPredicted += 0.005 * particles[p_i].deltaP;
		}

		// TODO: collision detection

	}

#pragma omp parallel for
	for (int i = 0; i < particles.size(); i++) {
		particles[i].postprocess(dt, allNeighborIDs[i], particles);
	}

	for (int i = 0; i < particles.size(); i++) {
		updateParticleCell(particles[i]);
	}

#pragma omp parallel for
	for (int i = 0; i < gridCells.size(); i++) {
		gridCells[i].updateNeighbors(gridCells);
	}

	glutPostRedisplay();
}

int Grid::computeCellIdx(int cellx, int celly, int cellz)
{
	return cellx + celly * width + cellz * width * width;
}