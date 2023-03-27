#include "Render.h"
#include <functional>

Render r;

void Display()
{
    r.render();
}

void Step()
{
    r.grid->step();
}

int main(int argc, char **argv)
{
    r.setGrid();

    glutInitWindowSize(kScreenWidth, kScreenHeight);
    glutInit(&argc, argv);
    glutCreateWindow("PBFluids");
    glutDisplayFunc(Display);
    glutIdleFunc(Step);

    glutMainLoop();

    return 0;
}