#include "kernel.h"

vec3 kernel_spiky(vec3 p_i, vec3 p_j, float h)
{
	double r = (p_i - p_j).Length();
	vec3 result = vec3(0, 0, 0);
	if (r <= h and r >= NUMERICAL_EPS) { // Distance within kernel radius and non-negligble
		result = -(45.0 / (PI * pow(h, 6))) * (pow(h - r, 2) / r) * (p_i - p_j);
	}
	return result;
}

double kernel_poly6(vec3 p_i, vec3 p_j, float h)
{
	double result = 0;
	double r = (p_i - p_j).Length();
	if (r <= h) result = (315.0 / (64.0 * PI * pow(h, 9))) * pow((pow(h, 2) - pow(r, 2)), 3);
	return result;
}

double kernel_poly6(float r, float h)
{
	double result = 0;
	if (r <= h) result = (315.0 / (64.0 * PI * pow(h, 9))) * pow((pow(h, 2) - pow(r, 2)), 3);
	return result;
}
