#include "Render.h"

void Render::setGrid(const Grid &inGrid)
{
    grid = inGrid;
}

void Render::render()
{
    glClearColor(0.02f, 0.01f, 0.01f, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, kViewWidth, 0, kViewHeight, 0, 1);

    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (size_t i = 0; i < kParticleCount; ++i) {
        vec3 normalizedVel = grid.particles[i].vel.Normalize();
        colors[i].r = 0.5f + 0.5f * normalizedVel[0];
        colors[i].g = 0.5f + 0.5f * normalizedVel[1];
        colors[i].b = 0.5f + 0.5f * normalizedVel[2];
        colors[i].a = 1.f;
        positions[i].x = grid.particles[i].pos[1] / grid.width * kViewWidth;
        if (i == 0) {
			auto test = positions[i].x;
		}
        positions[i].y = grid.particles[i].pos[2] / grid.height * kViewHeight;
    }

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    glPointSize(2.5f * kParticleRadius * kScreenWidth / kViewWidth);

    glColorPointer(4, GL_FLOAT, sizeof(Rgba), colors);
    glVertexPointer(2, GL_FLOAT, sizeof(Position), positions);
    glDrawArrays(GL_POINTS, 0, kParticleCount);

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    glutSwapBuffers();
}
