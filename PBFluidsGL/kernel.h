#include <cmath>
#include "Utils/vec.h"

#define PI 3.14159265358979323846 
#define NUMERICAL_EPS 0.00000001

// Compute the spiky kernel between two particles for gradient estimation.
vec3 kernel_spiky(vec3 p_i, vec3 p_j);

// Compute the Poly6 kernel between two particles for density estimation.
double kernel_poly6(vec3 p_i, vec3 p_j);

// Compute the Poly6 kernel.
double kernel_poly6(float r);