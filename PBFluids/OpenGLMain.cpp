#include "Render.h"
#include <functional>

Grid grid;
Render r;

void Display()
{
    r.render();
}

void Step()
{
    grid.step();
}

int main(int argc, char **argv)
{
    grid = Grid(200, 15, 0.5, 0, kParticleCount, 0.5, kParticleRadius);
    r.setGrid(grid);

    glutInitWindowSize(kScreenWidth, kScreenHeight);
    glutInit(&argc, argv);
    glutCreateWindow("SPH");
    glutDisplayFunc(Display);
    glutIdleFunc(Step);

    glutMainLoop();

    return 0;
}