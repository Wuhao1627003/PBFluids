#include <cmath>

#define PI 3.14159265358979323846 
#define NUMERICAL_EPS 0.00000001

/*
Compute the spiky kernel between two particles for gradient estimation.
Input:
	vec3 p_i: A particle in the fluid simulation (x, y, z) coordinates.
	vec3 p_j: Another particle in the fluid simulation (x, y, z) coordinates.
	double h: Kernel Radius.
Modifies:
	vec3& result: The output 3-vector of the spiky kernel based on the position of p_i, p_j.
*/
void kernel_spiky(vec3& result, vec3 p_i, vec3 p_j, double h);

/*
Compute the Poly6 kernel between two particles for density estimation.
Input:
	vec3 p_i: A particle in the fluid simulation (x, y, z) coordinates.
	vec3 p_j: Another particle in the fluid simulation (x, y, z) coordinates.
	double h: Kernel Radius.
Output:
	double result: The output of the Poly6 kernel based on the position of p_i, p_j.
*/
double kernel_poly6(vec3& p_i, vec3& p_j, double h);


/*
Compute the Poly6 kernel given an explicit radius r from origin. Used for tensile stability.
Input:
	double r: radius of interaction
	double h: Kernel Radius.
Output:
	double result: The output of the Poly6 kernel based on the radius r.
*/
double kernel_poly6(double r, double h);