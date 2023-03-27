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
	float x, y, z;
	int cellIdx;
	for (long particleID = 0; particleID < numParticles; particleID++) {
		x = (float)rand() / RAND_MAX * width;
		y = (float)rand() / RAND_MAX * width;
		z = (float)rand() / RAND_MAX * height;
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
		particles[i].findNeighbors(this->gridCells[particles[i].cellIdx], particles);
	}

	// TODO: main solver loop
	for (size_t iter = 0; iter < numIter; iter++) {
		// calculate lambdas
		for (int i = 0; i < jacobiIterations; i++) {

			// reset jacobi state
			density.setZero();
			lambda.setZero();
			c_grad_norm.setZero();
			dP.setZero();

			// ----------- Computed Constraints ---------
			for (int p_i = 0; p_i < particles.size(); p_i++) {
				for (int p_j = 0; p_j < particles[p_i].neighborParticles.size(); p_j++) {
					density[p_i] += particleMass * kernel_poly6(x_new.row(p_i), x_new.row(p_j), kernel_h);

					// accumulate gradient norm
					c_grad_temp.setZero();
					if (p_i == p_j) {
						for (auto p_k : neighbours[p_i]) {
							kernel_spiky(ker_res, x_new.row(p_i), x_new.row(p_k), kernel_h);
							c_grad_temp += ker_res;
						}
					}
					else {
						kernel_spiky(ker_res, x_new.row(p_i), x_new.row(p_j), kernel_h);
						c_grad_temp = ker_res;
					}

					c_grad_norm[p_i] += (c_grad_temp / rho).norm();
				}

				// Compute constraint and lambda
				c[p_i] = (density[p_i] / rho) - 1;
				lambda[p_i] = -c[p_i] / (c_grad_norm[p_i] + cfm_epsilon);
			}

		// update delta p and detect collision

		// update positions

	}

#pragma omp parallel for
	for (int i = 0; i < particles.size(); i++) {
		particles[i].postprocess(dt);
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