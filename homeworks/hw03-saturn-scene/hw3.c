
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// View controls
int th = 0; // Azimuth
int ph = 0; // Elevation
int autoRotate = 0;
double sceneAngle = 0;

// State toggles
int moonsShown = 1;
int satellitesShown = 1;

// Orbit control
double orbitAngle = 0;

#define CLIP_SIZE 50
#define PI 3.14159265

#define Cos(x) (cos((x) * PI / 180))
#define Sin(x) (sin((x) * PI / 180))

void Vertex(double th, double ph, int colorize)
{
    if (colorize)
    {
        float r = 0.7f + 0.2f * (Cos(th) * Cos(th));
        float g = 0.6f + 0.25f * (Sin(ph) * Sin(ph));
        float b = 0.5f + 0.2f * (Sin(th) * Sin(th));
        glColor3f(r, g, b);
    }
    else
        glColor3f(0.7, 0.7, 0.7);

    glVertex3d(Sin(th) * Cos(ph), Sin(ph), Cos(th) * Cos(ph));
}

void drawSphere(double x, double y, double z, double r, int colorize)
{
    const int d = 15;
    glPushMatrix();
    glTranslated(x, y, z);
    glScaled(r, r, r);
    for (int ph = -90; ph < 90; ph += d)
    {
        glBegin(GL_QUAD_STRIP);
        for (int th = 0; th <= 360; th += d)
        {
            Vertex(th, ph, colorize);
            Vertex(th, ph + d, colorize);
        }
        glEnd();
    }
    glPopMatrix();
}

void drawMoon(double distance, double size, double orbitAngle)
{
    const int segments = 100;

    // Orbit ring
    glColor3f(0.4, 0.4, 0.4);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < segments; i++)
    {
        double theta = 2.0 * PI * i / segments;
        double x = distance * cos(theta);
        double z = distance * sin(theta);
        glVertex3f(x, 0.0, z);
    }
    glEnd();

    glPushMatrix();
    glRotated(orbitAngle, 0, 1, 0);
    glTranslated(distance, 0.0, 0.0);
    glColor3f(0.7, 0.7, 0.7);
    drawSphere(0, 0, 0, size, 0);
    glPopMatrix();
}

void drawCylinder(float radius, float height, int segments)
{
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= segments; ++i)
    {
        float angle = 2 * PI * i / segments;
        float x = radius * cos(angle);
        float z = radius * sin(angle);
        glVertex3f(x, 0, z);
        glVertex3f(x, height, z);
    }
    glEnd();
}

void drawLine(float x1, float y1, float z1, float x2, float y2, float z2)
{
    glBegin(GL_LINES);
    glVertex3f(x1, y1, z1);
    glVertex3f(x2, y2, z2);
    glEnd();
}

void drawMoons()
{
    drawMoon(16.0, 0.8, orbitAngle * 4);
    drawMoon(20.0, 1.2, orbitAngle * 3);
    drawMoon(24.0, 1.0, orbitAngle * 2);
    drawMoon(28.0, 1.4, orbitAngle * 1.5);
    drawMoon(35.0, 2.0, orbitAngle * 0.5);
    drawMoon(42.0, 2.5, orbitAngle * 0.3);
}

void drawSatellite(double bodySize, double panelLength, double panelHeight)
{
    double half = bodySize / 2.0;

    glColor3f(0.6, 0.6, 0.6); // Body color
    glBegin(GL_QUADS);
    // Cube body faces
    glVertex3f(-half, -half, half);
    glVertex3f(half, -half, half);
    glVertex3f(half, half, half);
    glVertex3f(-half, half, half); // Front
    glVertex3f(-half, -half, -half);
    glVertex3f(-half, half, -half);
    glVertex3f(half, half, -half);
    glVertex3f(half, -half, -half); // Back
    glVertex3f(-half, -half, -half);
    glVertex3f(-half, -half, half);
    glVertex3f(-half, half, half);
    glVertex3f(-half, half, -half); // Left
    glVertex3f(half, -half, -half);
    glVertex3f(half, half, -half);
    glVertex3f(half, half, half);
    glVertex3f(half, -half, half); // Right
    glVertex3f(-half, half, -half);
    glVertex3f(-half, half, half);
    glVertex3f(half, half, half);
    glVertex3f(half, half, -half); // Top
    glVertex3f(-half, -half, -half);
    glVertex3f(half, -half, -half);
    glVertex3f(half, -half, half);
    glVertex3f(-half, -half, half); // Bottom
    glEnd();

    // Panels
    glColor3f(0.2, 0.5, 1.0);
    glBegin(GL_QUADS);
    glVertex3f(-half - panelLength, panelHeight, 0.0);
    glVertex3f(-half, panelHeight, 0.0);
    glVertex3f(-half, -panelHeight, 0.0);
    glVertex3f(-half - panelLength, -panelHeight, 0.0);

    glVertex3f(half, panelHeight, 0.0);
    glVertex3f(half + panelLength, panelHeight, 0.0);
    glVertex3f(half + panelLength, -panelHeight, 0.0);
    glVertex3f(half, -panelHeight, 0.0);
    glEnd();
}

void drawSatelliteV2(float scale, float panelLength, float r, float g, float b)
{
    glPushMatrix();
    glTranslatef(25.0f, 5.0f, 0.0f);
    glScalef(scale, scale, scale);

    // parabolic dish
    glPushMatrix();
    glColor3f(r, g, b);
    int segments = 24;
    float dishRadius = 1.0;
    float dishDepth = 0.35f;
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0, -dishDepth, 0); // Tip of dish
    for (int i = 0; i <= segments; ++i)
    {
        float theta = 2.0 * M_PI * i / segments;
        float x = dishRadius * cos(theta);
        float z = dishRadius * sin(theta);
        glVertex3f(x, dishDepth, z);
    }
    glEnd();
    glPopMatrix();

    // Antenna stand
    glPushMatrix();
    glTranslatef(0, -0.5f, 0);
    glColor3f(0.3f, 0.3f, 0.3f);
    drawCylinder(0.2f, 1.0f, 20);
    glPopMatrix();

    // Solar panels
    float panelWidth = 0.3;
    glColor3f(0.2, 0.5, 1.0);
    glBegin(GL_QUADS);
    glVertex3f(-0.2 - panelLength, -0.4, panelWidth);
    glVertex3f(-0.2, -0.4, panelWidth);
    glVertex3f(-0.2, -0.4, -panelWidth);
    glVertex3f(-0.2 - panelLength, -0.4, -panelWidth);
    glEnd();

    // long rod with weight at end
    glPushMatrix();
    glColor3f(0.4f, 0.4f, 0.1f);
    drawLine(0, -0.5f, 0, 2.0f, -1.5f, 0);

    glTranslatef(2.0f, -1.5f, 0);
    drawCylinder(0.1f, 0.4f, 10);
    glPopMatrix();

    // long antenna rod from top of dish
    glColor3f(1.0f, 1.0f, 1.0f);
    drawLine(0, 0, 0, 0, 3.0f, 0);

    // Antenna support struts
    glColor3f(0.46f, 0.46f, 0.46f);
    // thicker lines for struts
    glLineWidth(2.0f);
    drawLine(0, -0.5f, 0, 0.8f, 0.2f, 0.0f);
    drawLine(0, -0.5f, 0, -0.8f, 0.2f, 0.0f);
    drawLine(0, -0.5f, 0, 0.0f, 0.2f, 0.8f);
    drawLine(0, -0.5f, 0, 0.0f, 0.2f, -0.8f);

    glPopMatrix();
}

void drawSatellites()
{
    // Static satellite
    glPushMatrix();
    glTranslated(-20, 5, -10);
    glRotated(30, 1, 0, 0);
    glRotated(45, 0, 1, 0);
    glScaled(1.5, 1.5, 1.5);
    drawSatellite(1.0, 1.0, 0.3);
    glPopMatrix();

    // Orbiting satellite
    glPushMatrix();
    glRotated(45, 1, 0, 0);
    glRotated(orbitAngle * 3, 0, 1, 0);
    glTranslated(10, 0, 0);
    glRotated(orbitAngle * 2, 1, 0, 0);
    drawSatelliteV2(3.0f, 4.0f, 0.9f, 0.9f, 0.7f);
    glPopMatrix();

    // Orbiting satellite 2
    glPushMatrix();
    glRotated(-135, 0, 0, 1);
    glRotated(orbitAngle * 3, 0, 1, 0);
    glTranslated(10, 0, 0);
    glRotated(orbitAngle * 2, 1, 0, 0);
    drawSatelliteV2(1.5f, 6.0f, 0.7f, 1.0f, 0.0f);
    glPopMatrix();
}

void reshape(int width, int height)
{
    float aspect = (float)width / height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-CLIP_SIZE * aspect, CLIP_SIZE * aspect, -CLIP_SIZE, CLIP_SIZE, -CLIP_SIZE, CLIP_SIZE);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void drawAxes()
{
    glColor3f(1, 1, 1);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(45, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 45, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 45);
    glEnd();

    glRasterPos3f(47, 0, 0);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'X');
    glRasterPos3f(0, 47, 0);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'Y');
    glRasterPos3f(0, 0, 47);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'Z');
}

void special(int key, int x, int y)
{
    if (key == GLUT_KEY_RIGHT)
        th += 5;
    else if (key == GLUT_KEY_LEFT)
        th -= 5;
    else if (key == GLUT_KEY_UP)
        ph += 5;
    else if (key == GLUT_KEY_DOWN)
        ph -= 5;
    th %= 360;
    ph %= 360;
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    if (key == 'm' || key == 'M')
        moonsShown = !moonsShown;
    else if (key == 't' || key == 'T')
        satellitesShown = !satellitesShown;
    else if (key == 'r' || key == 'R')
        autoRotate = !autoRotate;
    else if (key == 27)
        exit(0);
    glutPostRedisplay();
}

void idle()
{
    orbitAngle += 0.2;
    if (orbitAngle > 360)
        orbitAngle -= 360;
    if (autoRotate)
    {
        sceneAngle += 0.1;
        if (sceneAngle > 360)
            sceneAngle -= 360;
    }
    glutPostRedisplay();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glRotated(ph, 1, 0, 0);
    glRotated(th, 0, 1, 0);
    if (autoRotate)
        glRotated(sceneAngle, 1, 1, 0);
    drawAxes();
    drawSphere(0, 0, 0, 10, 1);
    if (moonsShown)
        drawMoons();
    if (satellitesShown)
        drawSatellites();

    glPushMatrix();
    glTranslated(15, 5, -25);
    glScaled(3, 3, 3);
    glPopMatrix();
    glFlush();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(900, 900);
    glutCreateWindow("Homework 3: Sudhanva Manjunath");
    glEnable(GL_DEPTH_TEST);
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutSpecialFunc(special);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}