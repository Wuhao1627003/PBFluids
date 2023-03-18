#pragma once
#include <GL/glut.h>

#define kScreenWidth 640
#define kScreenHeight 480
#define kViewWidth 10.0f
#define kViewHeight (kScreenHeight*kViewWidth/kScreenWidth)

void Render(Grid grid);