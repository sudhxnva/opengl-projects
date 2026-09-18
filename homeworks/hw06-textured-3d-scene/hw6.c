#include "CSCIx229.h"

int mode = 1;       // 0 = Orthogonal, 1 = Perspective, 2 = First-Person
int axes = 1;       // Display axes
int objectMode = 0; // 0 = All objects, 1 = UFO, 2 = Rocket, 3 = Hourglass

int th = 0;
int ph = 0;
int fov = 55; //  Field of view (for perspective)
double asp = 1;
double dim = 3;

int move = 1;      //  Move light
int light = 1;     // Lighting
int shininess = 0; // Shininess (power of two)
int emission = 0;
float shiny = 1;
int ambient = 10;
int diffuse = 50;
int specular = 0;
int zh = 90;
int distance = 5; // Light distance
float ylight = 0; // Elevation of light
int smooth = 1;   // Smooth/Flat shading

int ballInc = 10;

unsigned int texture[7]; //  Texture names
int texEnabled = 1;      // Texture enabled

// camera
double camX = 0, camY = 1, camZ = 5; // FPV camera position
double camAngle = 0;                 // Horizontal angle
double camPitch = 0;                 // Vertical pitch

#define PI 3.14159265

void DebugNormal(double x, double y, double z, double nx, double ny, double nz)
{
    glBegin(GL_LINES);
    glVertex3d(x, y, z);
    glVertex3d(x + 0.2 * nx, y + 0.2 * ny, z + 0.2 * nz);
    glEnd();
}

/*
 *  Draw a ball
 *     at (x,y,z)
 *     radius (r)
 */
static void ball(double x, double y, double z, double r)
{
    if (texEnabled)
        glDisable(GL_TEXTURE_2D);
    //  Save transformation
    glPushMatrix();
    //  Offset, scale and rotate
    glTranslated(x, y, z);
    glScaled(r, r, r);
    //  White ball with yellow specular
    float yellow[] = {1.0, 1.0, 0.0, 1.0};
    float Emission[] = {0.0, 0.0, 0.01 * emission, 1.0};
    glColor3f(1, 1, 1);
    glMaterialf(GL_FRONT, GL_SHININESS, shiny);
    glMaterialfv(GL_FRONT, GL_SPECULAR, yellow);
    glMaterialfv(GL_FRONT, GL_EMISSION, Emission);
    //  Bands of latitude
    for (int ph = -90; ph < 90; ph += ballInc)
    {
        glBegin(GL_QUAD_STRIP);
        for (int th = 0; th <= 360; th += 2 * ballInc)
        {
            Vertex(th, ph, 0);
            Vertex(th, ph + ballInc, 0);
        }
        glEnd();
    }
    //  Undo transformations
    glPopMatrix();
    if (texEnabled)
        glEnable(GL_TEXTURE_2D);
}

void SlopedVertex(double th, double r, double y, double slope, int flip)
{
    double x = r * Cos(th);
    double z = r * Sin(th);

    double nx = slope * x;
    double ny = flip ? -1.0 : 1.0;
    double nz = slope * z;

    glNormal3d(nx, ny, nz);
    // Wrap texture horizontally around the saucer, and map vertically from center to edge
    glTexCoord2d((double)th / 360, (double)(y + 1) / 2);
    glVertex3d(x, y, z);
    // DebugNormal(x, y, z, nx, ny, nz);
}

void drawSaucerPan(double baseRadius, double height, int rings, int slices, int flip)
{
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    double stepR = baseRadius / rings;
    double stepY = height / rings;
    double slope = height / baseRadius;

    for (int i = 0; i < rings; ++i)
    {
        double r1 = baseRadius - i * stepR;
        double r2 = baseRadius - (i + 1) * stepR;

        double y1 = i * stepY;
        double y2 = (i + 1) * stepY;

        if (flip)
        {
            y1 = -y1;
            y2 = -y2;
        }

        glBegin(GL_QUAD_STRIP);
        for (int th = 0; th <= 360; th += 360 / slices)
        {
            SlopedVertex(th, r1, y1, slope, flip);
            SlopedVertex(th, r2, y2, slope, flip);
        }
        glEnd();
    }
}

void drawBeamFrustum(double baseRadius, double topRadius, double height, int slices)
{
    glBindTexture(GL_TEXTURE_2D, texture[5]);

    glColor3f(1.0, 1.0, 0.0);                // Yellow surface color
    float emission[] = {0.2, 0.2, 0.0, 1.0}; // Yellow glow
    glMaterialfv(GL_FRONT, GL_EMISSION, emission);

    glBegin(GL_QUAD_STRIP);
    for (int th = 0; th <= 360; th += 360 / slices)
    {
        double angle = th * 3.1415926 / 180;
        double x1 = baseRadius * cos(angle);
        double z1 = baseRadius * sin(angle);
        double x2 = topRadius * cos(angle);
        double z2 = topRadius * sin(angle);

        double nx = x2 - x1;
        double ny = height;
        double nz = z2 - z1;

        glTexCoord2f((double)th / 45, 1);
        glNormal3d(nx, ny, nz);
        glVertex3d(x1, 0, z1);

        glTexCoord2f((double)th / 45, 0);
        glNormal3d(nx, ny, nz);
        glVertex3d(x2, -height, z2);
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0.0, -1.0, 0.0);

    glTexCoord2f(0.5, 0.5);
    glVertex3f(0.0, -height, 0.0);
    for (int th = 0; th <= 360; th += 360 / slices)
    {
        double x = topRadius * Cos(th);
        double z = topRadius * Sin(th);
        // Circular mapping for the cap
        glTexCoord2f(0.5 + 0.5 * Cos(th), 0.5 + 0.5 * Sin(th));
        glVertex3f(x, -height, z);
    }
    glEnd();

    // Turn off emission after drawing
    float noEmission[] = {0.0, 0.0, 0.0, 1.0};
    glMaterialfv(GL_FRONT, GL_EMISSION, noEmission);
}

void drawUFO()
{
    glColor3f(0.6, 0.6, 0.6);

    // Top Saucer
    drawSaucerPan(1.0, 0.3, 10, 40, 0);

    // Bottom Saucer (flipped)
    drawSaucerPan(1.0, 0.3, 10, 40, 1);

    // Windshield
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glColor3f(0.2, 0.4, 1.0);
    glPushMatrix();
    glTranslatef(0.0, 0.3, 0.0);
    glScalef(0.3, 0.2, 0.3);
    for (int ph = -90; ph < 90; ph += 10)
    {
        glBegin(GL_QUAD_STRIP);
        for (int th = 0; th <= 360; th += 20)
        {
            Vertex(th, ph, 1);
            Vertex(th, ph + 10, 1);
        }
        glEnd();
    }
    glPopMatrix();

    // Beam (frustum)
    glPushMatrix();
    glTranslated(0.0, -0.2, 0.0);
    drawBeamFrustum(0.3, 0.7, 1.0, 40);
    glPopMatrix();
}

void drawBooster(double x, double y, double z, double scale)
{
    const int segments = 20;
    double radius = 0.1 * scale;
    double height = 0.6 * scale;
    double coneHeight = 0.35 * scale;

    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glPushMatrix();
    glTranslated(x, y + height, z);
    glScalef(radius, radius, radius);
    glScalef(1, 0.4, 1);
    glColor3f(0.6, 0.6, 0.6); // Light gray
    for (int ph = -90; ph < 90; ph += ballInc)
    {
        glBegin(GL_QUAD_STRIP);
        for (int th = 0; th <= 360; th += 2 * ballInc)
        {
            Vertex(th, ph, 1);
            Vertex(th, ph + ballInc, 1);
        }
        glEnd();
    }
    glPopMatrix();

    glPushMatrix();
    glTranslated(x, y, z);

    // Body
    glColor3f(0.6, 0.6, 0.6); // Light gray
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= segments; i++)
    {
        double th = 2 * M_PI * i / segments;
        double px = radius * cos(th);
        double pz = radius * sin(th);

        glNormal3f(px, 0, pz);
        glTexCoord2f((double)i / segments, 0);
        glVertex3f(px, 0, pz);
        glTexCoord2f((double)i / segments, height);
        glVertex3f(px, height, pz);
    }
    glEnd();

    // Exhaust
    glBindTexture(GL_TEXTURE_2D, texture[4]);
    glTranslated(0, 0, 0);
    glColor3f(1.0, 0.8, 0.0); // Bright yellow/orange
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0, -1, 0);
    glVertex3f(0, -coneHeight, 0); // Tip

    for (int i = 0; i <= segments; i++)
    {
        double th = 2 * M_PI * i / segments;
        double px = radius * cos(th);
        double pz = radius * sin(th);
        glNormal3f(px, -coneHeight, pz);
        glTexCoord2f((double)i / segments, 1);
        glVertex3f(px, 0, pz);
    }
    glEnd();
    glPopMatrix();
}

void drawRocket(double x, double y, double z)
{
    const int segments = 24;
    double radius = 0.2;
    double height = 2.0;
    double coneHeight = 0.4;

    glBindTexture(GL_TEXTURE_2D, texture[6]);

    glPushMatrix();
    glTranslated(x, y, z);

    // ---- Rocket Body ----
    glColor3f(1, 0, 0);
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= segments; i++)
    {
        double angle = 2 * M_PI * i / segments;
        double nx = cos(angle);
        double nz = sin(angle);
        double px = radius * nx;
        double pz = radius * nz;

        glNormal3f(nx, 0, nz);
        glVertex3f(px, 0, pz); // Bottom circle
        glTexCoord2f((double)i / segments, 0);
        glVertex3f(px, height, pz); // Top circle
        glTexCoord2f((double)i / segments, 1);
    }
    glEnd();

    // Nose Cone
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    glColor3f(1.0, 0.5, 0.1);
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0, 1, 0);
    glVertex3f(0, height + coneHeight, 0); // Tip
    glTexCoord2f(0.5, height + coneHeight);

    for (int i = 0; i <= segments; i++)
    {
        double angle = 2 * M_PI * i / segments;
        double px = radius * cos(angle);
        double pz = radius * sin(angle);
        glNormal3f(px, coneHeight, pz); // Slope outward
        glTexCoord2f((double)i / segments, height);
        glVertex3f(px, height, pz);
    }
    glEnd();

    glPopMatrix();

    // Boosters
    drawBooster(x + 0.2, y, z, 1.0);
    drawBooster(x - 0.2, y, z, 1.0);
    drawBooster(x, y, z + 0.2, 1.0);
    drawBooster(x, y, z - 0.2, 1.0);

    // exhaust
    glPushMatrix();
    glTranslated(x, y, z);
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1, 0.8, 0);
    glVertex3f(0, 0, 0);
    for (int i = 0; i <= segments; i++)
    {
        double th = 2 * M_PI * i / segments;
        glNormal3f(0, -1, 0);
        glVertex3f(radius * cos(th), 0, radius * sin(th));
    }
    glEnd();
    glPopMatrix();
}

void drawRoundedPlate(double y, double radius, double height, int segments)
{
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glPushMatrix();
    glColor3f(0.47, 0.23, 0);
    glTranslated(0, y, 0);

    // wall
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= segments; i++)
    {
        double angle = 2 * M_PI * i / segments;
        double x = radius * cos(angle);
        double z = radius * sin(angle);

        glNormal3f(x, 0, z);
        // Load texture coordinates
        glTexCoord2f((double)i / segments, 0);
        glVertex3f(x, 0, z);
        glTexCoord2f((double)i / segments, height);
        glVertex3f(x, height, z);
    }
    glEnd();

    // face
    glBegin(GL_TRIANGLE_FAN);
    glNormal3f(0, height, 0);
    glVertex3f(0, height, 0);
    for (int i = 0; i <= segments; i++)
    {
        double angle = 2 * M_PI * i / segments;
        double x = radius * cos(angle);
        double z = radius * sin(angle);
        // circular face texture mapping
        glTexCoord2f(0.5 + (x / (2 * radius)), 0.5 + (z / (2 * radius)));
        glVertex3f(x, height, z);
    }
    glEnd();
    glPopMatrix();
}

void drawHourglass(double x, double y, double z, double height)
{
    int segments = 48;
    int curveSamples = 32;
    double coneHeight = height / 2;
    double radius = 1.0;
    double plateHeight = 0.1;

    glPushMatrix();
    glTranslated(x, y, z);

    // top plate
    glColor3f(0.5, 0.5, 0.5);
    drawRoundedPlate(coneHeight, radius + 0.1, plateHeight, segments);

    // Bezier control points
    Point2D p0 = {0.1, 0.0};
    Point2D p1 = {radius * 0.3, coneHeight * 0.2};
    Point2D p2 = {radius * 0.9, coneHeight * 0.5};
    Point2D p3 = {radius, coneHeight};

    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glColor3f(0.514, 0.855, 0.98);

    // top half
    for (int i = 0; i < curveSamples; i++)
    {
        double t1 = (double)i / curveSamples;
        double t2 = (double)(i + 1) / curveSamples;

        Point2D a1 = cubicBezier(p0, p1, p2, p3, t1);
        Point2D a2 = cubicBezier(p0, p1, p2, p3, t2);

        Point2D d1 = cubicBezierDerivative(p0, p1, p2, p3, t1);
        Point2D d2 = cubicBezierDerivative(p0, p1, p2, p3, t2);

        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= segments; j++)
        {
            double theta = 2 * M_PI * j / segments;
            double cosT = cos(theta), sinT = sin(theta);

            // Point at t1
            double x1 = a1.x * cosT;
            double z1 = a1.x * sinT;

            // Point at t2
            double x2 = a2.x * cosT;
            double z2 = a2.x * sinT;

            glNormal3f(d1.y * cosT, -d1.x, d1.y * sinT);
            glTexCoord2f((double)j / segments, t1);
            glVertex3f(x1, a1.y, z1);

            glNormal3f(d2.y * cosT, -d2.x, d2.y * sinT);
            glTexCoord2f((double)j / segments, t2);
            glVertex3f(x2, a2.y, z2);
        }
        glEnd();
    }

    // bottom half
    glPushMatrix();
    glScaled(1.0, -1.0, 1.0);
    for (int i = 0; i < curveSamples; i++)
    {
        double t1 = (double)i / curveSamples;
        double t2 = (double)(i + 1) / curveSamples;

        Point2D a1 = cubicBezier(p0, p1, p2, p3, t1);
        Point2D a2 = cubicBezier(p0, p1, p2, p3, t2);

        Point2D d1 = cubicBezierDerivative(p0, p1, p2, p3, t1);
        Point2D d2 = cubicBezierDerivative(p0, p1, p2, p3, t2);

        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= segments; j++)
        {
            double theta = 2 * M_PI * j / segments;
            double cosT = cos(theta), sinT = sin(theta);

            double x1 = a1.x * cosT;
            double z1 = a1.x * sinT;

            double x2 = a2.x * cosT;
            double z2 = a2.x * sinT;

            glNormal3f(d1.y * cosT, -d1.x, d1.y * sinT);
            glTexCoord2f((double)j / segments, t1);
            glVertex3f(x1, a1.y, z1);

            glNormal3f(d2.y * cosT, -d2.x, d2.y * sinT);
            glTexCoord2f((double)j / segments, t2);
            glVertex3f(x2, a2.y, z2);
        }
        glEnd();
    }
    glPopMatrix();

    // bottom plate
    glColor3f(0.5, 0.5, 0.5);
    drawRoundedPlate(-coneHeight, radius + 0.1, -plateHeight, segments);

    glPopMatrix();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glLoadIdentity();

    if (mode == 1)
    {
        double Ex = -2 * dim * Sin(th) * Cos(ph);
        double Ey = +2 * dim * Sin(ph);
        double Ez = +2 * dim * Cos(th) * Cos(ph);
        gluLookAt(Ex, Ey, Ez, 0, 0, 0, 0, Cos(ph), 0);
    }
    else if (mode == 2)
    {
        double pitchRad = camPitch * M_PI / 180;
        double yawRad = camAngle * M_PI / 180;

        double dx = cos(pitchRad) * sin(yawRad);
        double dy = sin(pitchRad);
        double dz = -cos(pitchRad) * cos(yawRad);

        gluLookAt(camX, camY, camZ,
                  camX + dx, camY + dy, camZ + dz,
                  0, 1, 0);
    }
    else
    {
        glRotatef(ph, 1, 0, 0);
        glRotatef(th, 0, 1, 0);
    }

    glShadeModel(smooth ? GL_SMOOTH : GL_FLAT);

    //  Draw scene
    //  Select texture if textures are on
    if (texEnabled == 0)
        glDisable(GL_TEXTURE_2D);
    else
    {
        glEnable(GL_TEXTURE_2D);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    }

    if (light)
    {
        //  Translate intensity to color vectors
        //* make sure you use vec 4 for below
        float Ambient[] = {0.01 * ambient, 0.01 * ambient, 0.01 * ambient, 1.0};
        float Diffuse[] = {0.01 * diffuse, 0.01 * diffuse, 0.01 * diffuse, 1.0};
        float Specular[] = {0.01 * specular, 0.01 * specular, 0.01 * specular, 1.0};
        //  Light position
        float Position[] = {distance * Cos(zh), ylight, distance * Sin(zh), 1.0};
        //  Draw light position as ball (still no lighting here)
        glColor3f(1, 1, 1);
        ball(Position[0], Position[1], Position[2], 0.1);
        //  OpenGL should normalize normal vectors
        glEnable(GL_NORMALIZE);
        //  Enable lighting
        glEnable(GL_LIGHTING);
        //  Location of viewer for specular calculations
        // glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
        //  glColor sets ambient and diffuse color materials
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
        // * GL_COLOR only sets color on objects, but doesn't define the emisive and specular properties
        // * so  we use GL_COLOR_MATERIAL to tell OpenGL to use glColor to set the ambient and diffuse properties of the material
        glEnable(GL_COLOR_MATERIAL);
        //  Enable light 0
        glEnable(GL_LIGHT0);
        //  Set ambient, diffuse, specular components and position of light 0
        glLightfv(GL_LIGHT0, GL_AMBIENT, Ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, Diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, Specular);
        glLightfv(GL_LIGHT0, GL_POSITION, Position);
    }
    else
    {
        glDisable(GL_LIGHTING);
    }

    // Draw objects based on mode
    if (objectMode == 0)
    {
        drawUFO();
        drawRocket(-2, -1, 0);
        drawHourglass(2.5, 0, 0, 2);
    }
    else if (objectMode == 1)
    {
        drawUFO();
    }
    else if (objectMode == 2)
    {
        drawRocket(0, 0, 0);
    }
    else if (objectMode == 3)
    {
        drawHourglass(0, 0, 0, 2);
    }

    glDisable(GL_LIGHTING);
    if (axes)
    {
        glDisable(GL_TEXTURE_2D);
        drawAxes();
        if (texEnabled)
            glEnable(GL_TEXTURE_2D);
    }

    ErrCheck("display");
    glFlush();
    glutSwapBuffers();
}

void reshape(int width, int height)
{
    asp = (height > 0) ? (double)width / height : 1;
    glViewport(0, 0, width, height);
    Project(mode ? fov : 0, asp, dim);
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key, int x, int y)
{
    if (mode == 2)
    {
        // First-Person camera look control
        if (key == GLUT_KEY_RIGHT)
            camAngle += 5;
        else if (key == GLUT_KEY_LEFT)
            camAngle -= 5;
        else if (key == GLUT_KEY_UP)
            camPitch += 5;
        else if (key == GLUT_KEY_DOWN)
            camPitch -= 5;

        // Clamp pitch to avoid flipping
        if (camPitch > 89)
            camPitch = 89;
        if (camPitch < -89)
            camPitch = -89;

        camAngle = fmod(camAngle + 360, 360);
    }
    else
    {
        // Azimuth and elevation control for other views
        if (key == GLUT_KEY_RIGHT)
            th += 5;
        else if (key == GLUT_KEY_LEFT)
            th -= 5;
        else if (key == GLUT_KEY_UP)
            ph += 5;
        else if (key == GLUT_KEY_DOWN)
            ph -= 5;
    }

    // Common controls (for all modes)
    if (key == GLUT_KEY_PAGE_DOWN)
        dim += 0.1;
    else if (key == GLUT_KEY_PAGE_UP && dim > 1)
        dim -= 0.1;
    else if (key == GLUT_KEY_F1)
        smooth = 1 - smooth;
    else if (key == GLUT_KEY_F3)
        distance = (distance == 1) ? 5 : 1;
    else if (key == GLUT_KEY_F8)
        ballInc = (ballInc == 10) ? 3 : 10;

    // Keep angles clean
    th %= 360;
    ph %= 360;

    Project(mode ? fov : 0, asp, dim);
    glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
    //  Elapsed time in seconds
    double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    zh = fmod(90 * t, 360.0);
    //  Tell GLUT it is necessary to redisplay the scene
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{

    if (mode == 2)
    {
        double speed = 0.5;

        double pitchRad = camPitch * PI / 180;
        double yawRad = camAngle * PI / 180;

        // Calculate 3D direction vector
        double dx = cos(pitchRad) * sin(yawRad);
        double dy = sin(pitchRad);
        double dz = -cos(pitchRad) * cos(yawRad);

        // Move forward/backward based on pitch and yaw
        if (key == 'w' || key == 'W')
        {
            camX += dx * speed;
            camY += dy * speed;
            camZ += dz * speed;
        }
        else if (key == 's' || key == 'S')
        {
            camX -= dx * speed;
            camY -= dy * speed;
            camZ -= dz * speed;
        }

        // Strafe left/right
        double strafeX = cos(yawRad);
        double strafeZ = sin(yawRad);

        if (key == 'a' || key == 'A')
        {
            camX -= strafeX * speed;
            camZ += strafeZ * speed;
        }
        else if (key == 'd' || key == 'D')
        {
            camX += strafeX * speed;
            camZ -= strafeZ * speed;
        }

        glutPostRedisplay();
    }

    //  Exit on ESC
    if (key == 27)
        exit(0);
    //  Reset view angle
    else if (key == '0')
        th = ph = 0;
    //  Toggle axes
    else if (key == 'x' || key == 'X')
        axes = 1 - axes;
    //  Toggle lighting
    else if (key == 'l' || key == 'L')
        light = 1 - light;
    //  Toggle light movement
    else if (key == 'm' || key == 'M')
        move = 1 - move;
    //  Move light
    else if (key == 'n' || key == 'N')
        mode = (mode + 1) % 3;
    else if (key == '<')
        zh += 1;
    else if (key == '>')
        zh -= 1;
    //  change field of view angle
    else if (key == '-' && key > 1)
        fov--;
    else if (key == '+' && key < 179)
        fov++;
    //  Light elevation
    else if (key == '[')
        ylight -= 0.1;
    else if (key == ']')
        ylight += 0.1;
    //  Ambient level
    else if (key == 'a' && ambient > 0)
        ambient -= 5;
    else if (key == 'A' && ambient < 100)
        ambient += 5;
    //  Diffuse level
    else if (key == 'd' && diffuse > 0)
        diffuse -= 5;
    else if (key == 'D' && diffuse < 100)
        diffuse += 5;
    //  Specular level
    else if (key == 's' && specular > 0)
        specular -= 5;
    else if (key == 'S' && specular < 100)
        specular += 5;
    //  Emission level
    else if (key == 'e' && emission > 0)
        emission -= 5;
    else if (key == 'E' && emission < 100)
        emission += 5;
    //  Shininess level
    else if (key == 'n' && shininess > -1)
        shininess -= 1;
    else if (key == 'N' && shininess < 7)
        shininess += 1;
    else if (key == 'o' || key == 'O')
    {
        objectMode = (objectMode + 1) % 4;
    }
    else if (key == 't' || key == 'T')
        texEnabled = 1 - texEnabled;

    //  Translate shininess power to value (-1 => 0)
    shiny = shininess < 0 ? 0 : pow(2.0, shininess);
    //  Reproject
    Project(mode ? fov : 0, asp, dim);
    //  Animate if requested
    glutIdleFunc(move ? idle : NULL);
    //  Tell GLUT it is necessary to redisplay the scene
    glutPostRedisplay();
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Homework 6: Sudhanva Manjunath");

    // callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(special);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);

    texture[0] = LoadTexBMP("glass-2.bmp");
    texture[1] = LoadTexBMP("wood.bmp");
    texture[2] = LoadTexBMP("metal.bmp");
    texture[3] = LoadTexBMP("metal-2.bmp");
    texture[4] = LoadTexBMP("fire.bmp");
    texture[5] = LoadTexBMP("shimmer.bmp");
    texture[6] = LoadTexBMP("rocket.bmp");

    ErrCheck("init");

    glutMainLoop();
    return 0;
}