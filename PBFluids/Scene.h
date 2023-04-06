#pragma once
#include "Utils/Triangle.h"
#include <vector>
class Scene
{
public:
	vector<Triangle> triangles;
	Scene() {};

	void bounce(vec3 &center, float radius, vec3 &vel, float dt)
	{
		for (int i = 0; i < triangles.size(); i++) {
			bool hit = triangles[i].bounce(center, radius, vel, dt);
			if (hit) {
				return;
			}
		}
	}
};

