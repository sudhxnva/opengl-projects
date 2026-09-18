#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdio.h>
#include <math.h>

/*  Lorenz Parameters  */
double s = 10;
double b = 2.6666;
double r = 28;

// View controls
int th = 0; // Azimuth
int ph = 0; // Elevation

// Ball state
int ballStep = 0;
int ballShown = 0;

int colorToggle = 1;

#define CLIP_SIZE 80

// Lorenz path state
#define MAX_POINTS 50000
float lorenzX[MAX_POINTS];
float lorenzY[MAX_POINTS];
float lorenzZ[MAX_POINTS];

// fn to draw the attractor as a line strip
int drawLorenz()
{
    glColor3f(1, 1, 1);
    glLineWidth(1.3);
    glBegin(GL_LINE_STRIP);
    int i;
    for (i = 0; i < MAX_POINTS; i++)
    {
        if (colorToggle)
        {
            float t = (float)i / MAX_POINTS;
            glColor3f(1.0 - t, 0.2 + t, t);
        }
        else
            glColor3f(1, 1, 1);

        glVertex3f(lorenzX[i], lorenzY[i], lorenzZ[i]);
    }
    glEnd();
    return 0;
}

// fn to generate the lorenz points and store it as arrays of coordinates
int generateLorenzPoints()
{
    int i;
    /*  Coordinates  */
    double x = 1;
    double y = 1;
    double z = 1;
    /*  Time step  */
    double dt = 0.001;
    /*
     *  Integrate 50,000 steps (50 time units with dt = 0.001)
     *  Explicit Euler integration
     */
    for (i = 0; i < MAX_POINTS; i++)
    {
        double dx = s * (y - x);
        double dy = x * (r - z) - y;
        double dz = x * y - b * z;
        x += dt * dx;
        y += dt * dy;
        z += dt * dz;
        // printf("%5d %8.3f %8.3f %8.3f\n", i + 1, x, y, z);
        lorenzX[i] = x;
        lorenzY[i] = y;
        lorenzZ[i] = z;
    }
    return 0;
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

    glColor3f(1, 1, 1);
    glRasterPos3f(47, 0, 0);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'X');
    glRasterPos3f(0, 47, 0);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'Y');
    glRasterPos3f(0, 0, 47);
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, 'Z');
}

void drawBall()
{
    glColor3f(1, 1, 1);
    glPushMatrix();
    glTranslatef(lorenzX[ballStep], lorenzY[ballStep], lorenzZ[ballStep]);
    glutSolidSphere(1, 10, 10); // radius, slices, stacks
    glPopMatrix();
}

// 11/Sep - In openGL, you can only use convex polygons. If you try to draw a non-convex polygon, it will f up anyway it wants it to

/*
 *GLUT calls this routine when an arrow key is pressed
 */
void special(int key, int x, int y)
{
    //  Right arrow key - increase angle by 5 degrees
    if (key == GLUT_KEY_RIGHT)
        th += 5;
    //  Left arrow key - decrease angle by 5 degrees
    else if (key == GLUT_KEY_LEFT)
        th -= 5;
    //  Up arrow key - increase elevation by 5 degrees
    else if (key == GLUT_KEY_UP)
        ph += 5;
    //  Down arrow key - decrease elevation by 5 degrees
    else if (key == GLUT_KEY_DOWN)
        ph -= 5;
    //  Keep angles to +/-360 degrees
    th %= 360;
    ph %= 360;
    //  Tell GLUT it is necessary to redisplay the scene
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    // toggle ball visibility
    if (key == 'b' || key == 'B')
        ballShown = !ballShown;

    // modify lorenz parameters
    else if (key == 's')
        s -= 1;
    else if (key == 'S')
        s += 1;

    else if (key == 'r')
        r -= 1;
    else if (key == 'R')
        r += 1;

    else if (key == 'q')
        b -= 0.1;
    else if (key == 'Q')
        b += 0.1;

    // toggle color of lorenz attractor
    else if (key == 'c' || key == 'C')
        colorToggle = !colorToggle;

    // After any parameter change, regenerate the attractor
    generateLorenzPoints();
    ballStep = 0;

    glutPostRedisplay();
}

void idle()
{
    if (ballShown)
        ballStep = (ballStep + 1) % MAX_POINTS;
    glutPostRedisplay();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glRotated(ph, 1, 0, 0);
    glRotated(th, 0, 1, 0);
    drawAxes();
    drawLorenz();
    if (ballShown)
        drawBall();
    glFlush();
}

int main(int argc, char *argv[])
{
    generateLorenzPoints();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);

    glutInitWindowSize(900, 900);
    glutCreateWindow("Homework 2: Sudhanva Manjunath");
    glEnable(GL_DEPTH_TEST);
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutSpecialFunc(special);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}