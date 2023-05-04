#pragma once
#include "Utils/Triangle.h"
#include <vector>
class Scene
{
public:
	vector<Triangle> sceneTriangles;
	Scene() {};

	void addTriangles(const vector<vec3> &triangles)
	{
		sceneTriangles.clear();
		for (long numTriangles = 0; numTriangles < triangles.size() / 3; numTriangles++) {
			Triangle triangle(triangles[numTriangles * 3], triangles[numTriangles * 3 + 1], triangles[numTriangles * 3 + 2]);
			sceneTriangles.push_back(triangle);
		}
	}

	bool bounce(vec3 &center, float radius, vec3 &vel, float dt)
	{
		for (Triangle triangle : sceneTriangles) {
			bool hit = triangle.bounce(center, radius, vel, dt);
			if (hit) {
				return true;
			}
		}
		return false;
	}
};

