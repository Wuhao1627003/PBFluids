#include "Particle.h"

void Particle::preprocess(float dt)
{
	this->vel += vec3(0., 0., -0.1) * dt;
	this->posPredicted = this->pos + this->vel * dt;
}

vector<long> Particle::findNeighborIDs(const Cell &c, const vector<Particle> &particles)
{
	return c.neighborParticleIDs;
}

void Particle::reset()
{
	this->density = 0.;
	this->gradNorm = 0.;
	this->lambda = 0.;
	this->deltaP = vec3(0., 0., 0.);
}

void Particle::postprocess(float dt)
{
	this->vel = (this->posPredicted - this->pos) / dt;

	// TODO: vorticity & viscosity

	// ----------- Computed Vorticity ---------
    Eigen::Vector3d ker_res, v_ji, N_i, omega_i; // for cross product

    int num_particles = x_new.rows();
    omega.setZero();
    eta.setZero();
    N.setZero();

    // Compute curl
    for (int p_i = 0; p_i < num_particles; p_i++) {
        for (int p_j : neighbours[p_i]) {
            kernel_spiky(ker_res, x_new.row(p_i), x_new.row(p_j), kernel_h);
            v_ji = v.row(p_j) - v.row(p_i);
            omega.row(p_i) += (v_ji).cross(ker_res);
        }
    }

    // Compute differential operator norm
    for (int p_i = 0; p_i < num_particles; p_i++) {
        for (int p_j : neighbours[p_i]) {
            kernel_spiky(ker_res, x_new.row(p_i), x_new.row(p_j), kernel_h);
            eta.row(p_i) += omega.row(p_j).norm() * ker_res;
        }
        if (eta.row(p_i).norm() > 0) {
            N.row(p_i) = eta.row(p_i) / eta.row(p_i).norm();
        }
        N_i = N.row(p_i);
        omega_i = omega.row(p_i);
        vorticity_f.row(p_i) = vorticity_epsilon * (N_i).cross(omega_i);
    }

    // Update with force due to vorticity
    this->vel += dt * vorticity_f;

	// ----------- Computed Viscosity ---------

	this->pos = this->posPredicted;
}