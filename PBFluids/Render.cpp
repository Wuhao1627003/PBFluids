// Currently copied from example
#include "Render.h"
#include "Grid.h"

void Render(Grid grid)
{
    glClearColor(0.02f, 0.01f, 0.01f, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, kViewWidth, 0, kViewHeight, 0, 1);

    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (size_t i = 0; i < particleCount; ++i) {
        const Particle pi = grid.particles[i];
        const Material &material = particleMaterials[i];

        Rgba &rgba = shadedParticleColours[i];
        rgba = material.colour;
        rgba.r *= material.bias + material.scale * pi.P;
        rgba.g *= material.bias + material.scale * pi.P;
        rgba.b *= material.bias + material.scale * pi.P;
    }

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glPointSize(2.5f * kParticleRadius * kScreenWidth / kViewWidth);

    glColorPointer(4, GL_FLOAT, sizeof(Rgba), shadedParticleColours);
    glVertexPointer(2, GL_FLOAT, sizeof(Particle), particles);
    glDrawArrays(GL_POINTS, 0, particleCount);

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    glutSwapBuffers();
}