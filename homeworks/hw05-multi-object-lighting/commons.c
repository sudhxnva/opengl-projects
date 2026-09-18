#include "CSCIx229.h"

/*
 *  Draw vertex in polar coordinates with normal
 */
void Vertex(double th, double ph)
{
    double x = Sin(th) * Cos(ph);
    double y = Cos(th) * Cos(ph);
    double z = Sin(ph);
    //  For a sphere at the origin, the position
    //  and normal vectors are the same
    glNormal3d(x, y, z);
    glVertex3d(x, y, z);
}

void drawAxes()
{
    glColor3f(1, 1, 1);
    const double len = 2.0;
    glBegin(GL_LINES);
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(len, 0.0, 0.0);
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(0.0, len, 0.0);
    glVertex3d(0.0, 0.0, 0.0);
    glVertex3d(0.0, 0.0, len);
    glEnd();

    glRasterPos3d(len, 0.0, 0.0);
    Print("X");
    glRasterPos3d(0.0, len, 0.0);
    Print("Y");
    glRasterPos3d(0.0, 0.0, len);
    Print("Z");
}