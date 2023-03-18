// Currently copied from example
#include "Render.h"

int main(int argc, char **argv)
{
    glutInitWindowSize(kScreenWidth, kScreenHeight);
    glutInit(&argc, argv);
    glutInitDisplayString("samples stencil>=3 rgb double depth");
    glutCreateWindow("SPH");
    glutDisplayFunc(Render);
    glutIdleFunc(Update);

    memset(particles, 0, kParticleCount * sizeof(Particle));
    UpdateGrid();

    glutMainLoop();

    return 0;
}