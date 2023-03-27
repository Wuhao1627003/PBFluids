#pragma once
#include <GL/glut.h>
#include "Grid.h"

#define kScreenWidth 640
#define kScreenHeight 480
#define kViewWidth 10.0f
#define kViewHeight (kScreenHeight*kViewWidth/kScreenWidth)
#define kParticleCount 3000
#define kParticleRadius 0.05f

class Render
{
public:
	struct Rgba
	{
		float r, g, b, a;
	};
	Rgba colors[kParticleCount];
	
	struct Position
	{
		float x, y;
	};
	Position positions[kParticleCount];

	Grid grid;

	Render() {};
	void render();
	void setGrid(const Grid &inGrid);
};