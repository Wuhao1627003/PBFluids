#pragma once
#include "vec.h"

class Triangle
{
public:
	vec3 p1, p2, p3;
	vec3 e1, e2;
	vec3 normal;

	Triangle(vec3 p1, vec3 p2, vec3 p3) : p1(p1), p2(p2), p3(p3) {
		e1 = p2 - p1;
		e2 = p3 - p1;
		normal = (e2 ^ e1).Normalize();
	};

    // dt passed in here should be divided by numIter first
	bool bounce(vec3 &center, float radius, vec3 &vel, float dt)
	{
        vec3 velNorm = vel.Normalize();
        vec3 h = velNorm ^ e2;
        float a = Dot(e1, h);
        if (a > -EPSILON && a < EPSILON) {
            return false;    // This ray is parallel to this triangle.
        }
        float f = 1. / a;
        vec3 s = center - p1;
        float u = f * Dot(s, h);
        if (u < -0.01 || u > 1.01)
            return false;
        vec3 q = s ^ e1;
        float v = f * Dot(velNorm, q);
        if (v < -0.01 || u + v > 1.01) {
            return false;
        }
        // At this stage we can compute t to find out where the intersection point is on the line.
        float t = f * Dot(e2, q);
        if (Dot(vel, normal) < 0 && t <= min(dt, radius)) {
            center += velNorm * (t - radius);
            if (vel.Length() < 3) {
                vel = vec3(0, 0, 0);
                return true;
            }
            vel -= 2 * Dot(vel, normal) * normal;
            vel *= 1.5;
            center += vel * dt;
            return true;
        }
        else // This means that there is a line intersection but not a ray intersection.
            return false;
	}
};

