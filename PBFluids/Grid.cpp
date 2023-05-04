#include "Grid.h"

#define initCondition 2

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
		if (initCondition == 0) {
			// Random Init
			x = (float) rand() / RAND_MAX * worldWidth;
			y = (float) rand() / RAND_MAX * worldWidth;
			z = (float) rand() / RAND_MAX * worldHeight;
		}
		else if (initCondition == 1) {
			// Block Init
			x = (1 + 2 * (float) rand() / RAND_MAX) * worldWidth / 4;
			y = (1 + 2 * (float) rand() / RAND_MAX) * worldWidth / 4;
			z = (3 + (float) rand() / RAND_MAX) * worldHeight / 4;
		}
		else if (initCondition == 2) {
			// Diagonal Init
			x = (float) rand() / RAND_MAX * worldWidth;
			y = (float) rand() / RAND_MAX * worldWidth;
			z = (float) rand() / RAND_MAX * min(y,(float) worldHeight);
		}

		cellIdx = cellCoordMap.find(vec3((int) x == worldWidth ? width - 1: (int) (x / cellSize), (int) y == worldWidth ? width - 1 : (int) (y / cellSize), (int) z == worldHeight ? height - 1 : (int) (z / cellSize)))->second;
		this->particles[particleID] = Particle(particleID, cellIdx, vec3(x - worldWidth / 2, y - worldWidth / 2, z));
		this->gridCells[cellIdx].particleIDs.push_back(particleID);
	}
}

void Grid::initParticlesFromMesh()
{
	this->particles.resize(numParticles);
	this->allNeighborIDs.resize(numParticles);
	int cellIdx;
	double x, y, z;

	for (long particleID = 0; particleID < numParticles; particleID++) {
		GEOM_WOF::Point3 p = particleCenters[particleID];
		p.xyz(x, y, z);
		x += worldWidth / 2;
		y += worldWidth / 2;
		cellIdx = cellCoordMap.find(vec3((int) x == worldWidth ? width - 1 : (int) (x / cellSize), (int) y == worldWidth ? width - 1 : (int) (y / cellSize), (int) z == worldHeight ? height - 1 : (int) (z / cellSize)))->second;
		this->particles[particleID] = Particle(particleID, cellIdx, vec3(x - worldWidth / 2, y - worldWidth / 2, z));
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
	unordered_map<vec3, int>::iterator it = cellCoordMap.find(vec3((int) (p.pos[0] / cellSize + width / 2), (int) (p.pos[1] / cellSize + width / 2), (int) (p.pos[2] / cellSize)));
	if (it == cellCoordMap.end()) {
		// Out of boundary
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

	// main solver loop
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
				particles[p_i].density += particleMass * kernel_poly6(particles[p_i].posPredicted, particles[p_j].posPredicted, cellSize);

				// accumulate gradient norm
				vec3 gradTemp = vec3(0, 0, 0);
				if (p_i == p_j) {
					for (long p_k : neighbours) {
						gradTemp += kernel_spiky(particles[p_i].posPredicted, particles[p_k].posPredicted, cellSize);
					}
				}
				else {
					gradTemp = kernel_spiky(particles[p_i].posPredicted, particles[p_j].posPredicted, cellSize);
				}

				if (neighbours.size() > 0) {
					particles[p_i].gradNorm += (gradTemp / this->density).Length();
				}
			}

			// Compute constraint and lambda
			double constraintVal = (particles[p_i].density / this->density) - 1;
			particles[p_i].lambda = -constraintVal / (particles[p_i].gradNorm + cfmEpsilon);
		}

		// ----------- Computed Position Correction ---------
		// update delta p
#pragma omp parallel for
		for (int p_i = 0; p_i < particles.size(); p_i++) {
			for (int p_j : allNeighborIDs[p_i]) {
				vec3 spikyKernel = kernel_spiky(particles[p_i].posPredicted, particles[p_j].posPredicted, cellSize);
				float s_corr = -kTensile * pow(kernel_poly6(particles[p_i].posPredicted, particles[p_j].posPredicted, cellSize), nTensile) / tensileStabilityDenom;
				particles[p_i].deltaP += (particles[p_i].lambda + particles[p_j].lambda + s_corr) * spikyKernel;
			}
			if (allNeighborIDs[p_i].size() > 0) {
				particles[p_i].deltaP /= this->density;
				particles[p_i].posPredicted += 0.005 * particles[p_i].deltaP * dt;
			}
		}

		// --------- Collision with Container
		// collision with custom container
		if (this->scene.sceneTriangles.size() > 0) {
#pragma omp parallel for
			for (int i = 0; i < particles.size(); i++) {
				this->scene.bounce(particles[i].posPredicted, radius, particles[i].vel, dt / numIter);
			}
		}
		// collision with default box
		float velMultiplier = -0.9;
#pragma omp parallel for
		for (int i = 0; i < particles.size(); i++) {
			bool hit = false;
			if (particles[i].posPredicted[0] < -worldWidth / 2.0 && particles[i].vel[0] < 0) {
				particles[i].posPredicted[0] = -worldWidth / 2.0;
				hit = true;
				particles[i].vel[0] *= velMultiplier;
			}
			if (particles[i].posPredicted[0] > worldWidth / 2.0 && particles[i].vel[0] > 0) {
				particles[i].posPredicted[0] = worldWidth / 2.0;
				hit = true;
				particles[i].vel[0] *= velMultiplier;
			}
			if (particles[i].posPredicted[1] < -worldWidth / 2.0 && particles[i].vel[1] < 0) {
				particles[i].posPredicted[1] = -worldWidth / 2.0;
				hit = true;
				particles[i].vel[1] *= velMultiplier;
			}
			if (particles[i].posPredicted[1] > worldWidth / 2.0 && particles[i].vel[1] > 0) {
				particles[i].posPredicted[1] = worldWidth / 2.0;
				hit = true;
				particles[i].vel[1] *= velMultiplier;
			}
			if (particles[i].posPredicted[2] < 0 && particles[i].vel[2] < 0) {
				particles[i].posPredicted[2] = 0;
				hit = true;
				particles[i].vel[2] *= velMultiplier;
			}
			if (particles[i].posPredicted[2] > worldHeight && particles[i].vel[2] > 0) {
				particles[i].posPredicted[2] = worldHeight;
				hit = true;
				particles[i].vel[2] *= velMultiplier;
			}

			if (hit) {
				if (particles[i].vel.Length() < 3) {
					particles[i].vel = vec3(0, 0, 0);
				}
				particles[i].posPredicted += 7 * particles[i].vel * dt / numIter;
			}
		}
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
}

void Grid::addContainer(const vector<vec3> &triangles)
{
	this->scene.addTriangles(triangles);
}

int Grid::computeCellIdx(int cellx, int celly, int cellz)
{
	return cellx + celly * width + cellz * width * width;
}