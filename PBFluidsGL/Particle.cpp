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

void Particle::postprocess(float dt, vector<long> neighborIDs, vector<Particle>& particles)
{
	this->vel = (this->posPredicted - this->pos) / dt;

	// TODO: vorticity & viscosity

	// ----------- Computed Vorticity ---------
    vec3 v_ji, vorticity_f;

    this->omega = vec3(0., 0., 0.);
    this->eta = vec3(0., 0., 0.);
    this->N = vec3(0., 0., 0.);

    // Compute curl
    for (int p_j : neighborIDs) {
        vec3 ker_res = kernel_spiky(this->posPredicted, particles[p_j].posPredicted);
        v_ji = particles[p_j].vel - this->vel;
        this->omega += v_ji.Cross(ker_res);
    }

    // Compute differential operator norm
    for (int p_j : neighborIDs) {
        vec3 ker_res = kernel_spiky(this->posPredicted, particles[p_j].posPredicted);
        this->eta += particles[p_j].omega.Length() * ker_res;
    }
    if (this->eta.Length() > 0) {
        this->N = this->eta.Normalize();
    }
    vorticity_f = 0.0001 * (this->N).Cross(this->omega);

    // Update with force due to vorticity
    this->vel += dt * vorticity_f;

	// ----------- Computed Viscosity ---------
    vec3 v_new = this->vel;
    for (int p_j : neighborIDs) {
        v_new += 0.0001 * kernel_poly6(this->posPredicted, particles[p_j].posPredicted) * (particles[p_j].vel - this->vel);
    }
    this->vel = v_new;

	this->pos = this->posPredicted;
}