#include "kernel.h"

vec3 kernel_spiky(vec3 p_i, vec3 p_j)
{
	double r = (p_i - p_j).Length();
	vec3 result = vec3(0, 0, 0);
	if (r <= 1. and r >= NUMERICAL_EPS) { // Distance within kernel radius and non-negligble
		result = -(45.0 / PI) * (pow(1. - r, 2) / r) * (p_i - p_j);
	}
	return result;
}

double kernel_poly6(vec3 p_i, vec3 p_j)
{
	double r = (p_i - p_j).Length();
	return kernel_poly6(r);
}

double kernel_poly6(float r)
{
	double result = 0;
	if (r <= 1.) result = (315.0 / (64.0 * PI)) * pow((1. - pow(r, 2)), 3);
	return result;
}
