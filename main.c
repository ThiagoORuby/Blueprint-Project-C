#include <stdio.h>
#include <stdlib.h>
#include <GL/freeglut.h>
#include <GL/glu.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "src/rect_components.h"
#include "src/utils.h"
#include "src/SectorRoom.h"

// Draw a circle
void drawCircle(double raio, double centroX, double centroY, int bg_angle, int end_angle)
{
    glPushMatrix();
    glTranslatef(centroX, centroY, 0.0);

    glBegin(GL_LINE_STRIP);
    for (int i = bg_angle; i < end_angle; i++)
    {
        double degInRad = i * M_PI / 180;
        glVertex2f(cos(degInRad) * raio, sin(degInRad) * raio);
    }
    if(end_angle == 360) glVertex2f(cos(0) * raio, sin(0) * raio);
    glEnd();

    glPopMatrix();
}

// Draw house's hall
void drawHall(double radius, double size)
{
    double f = 0.15;
    double walls[4] = {-size/2 - f, -size/2, size/2, size/2 + f};

    // lines
    glColor3f(1.0f, 0.0f, 0.4f);

    for(int i = 0; i < 4; i++)
    {
        glPushMatrix();
        glTranslatef(walls[i], 0.0f, 1.0f);
        glBegin(GL_LINE_LOOP);
        glVertex2f(0, 0);
        glVertex2f(0, radius);
        glEnd();
        glPopMatrix();
    }
}

// draw the compass rose
void drawCompassRose(double x0, double y0, double angle)
{
    double length = 0.5;
    double rad = angle * M_PI / 180;

    // m1 = b/a -> V = (a, b)
    double m1 = tan(rad);
    double mod_V1 = sqrt(pow(1, 2) + pow(m1, 2));
    double m2 = -1/m1;
    double mod_V2 = sqrt(pow(1, 2) + pow(m2, 2));

    // original coordenates
    // x, y = (x0 + u * a/|V|, y0 + u * b/|V|)
    double x = 1/mod_V1*length;
    double y = m1/mod_V1*length;

    // perpendicular coordenates
    double px = 1/mod_V2*length;
    double py = m2/mod_V2*length;

    // set color to white
    glColor3f(1,1,1);

    glBegin(GL_LINE_LOOP);
    glVertex2f(x0 - x, y0 - y);
    glVertex2f(x0 + x, y0 + y);
    glEnd();

    // Draw the North indicate
    if (angle <= 90 || (angle >= 270 && angle <= 360))
        drawChar(x0 + x + 0.1, y0 + y + 0.1, 'N');
    else
        drawChar(x0 - x + 0.1, y0 - y + 0.1, 'N');

    glBegin(GL_LINE_LOOP);
    glVertex2f(x0 - px, y0 - py);
    glVertex2f(x0 + px, y0 + py);
    glEnd();
}

void RenderScene(void)
{
    // clear the window with current clearing color
    glClear(GL_COLOR_BUFFER_BIT);
    // Enable Zoom
    enable_zoom();

    double wall = 0.15;

    // Purple
    glColor3f(1.0f, 0.0f, 0.4f);

    drawCircle(6.2, 0, 0, 0, 360);
    drawCircle(6.2 - wall, 0, 0, 0, 360);
    //drawRoom(6.2 - wall, 0, 0, 90, 90 + 30);

    glColor3f(1.0f, 0.0f, 0.4f);
    drawCircle(2.8, 0, 0, 0, 360);
    drawCircle(2.8 - wall, 0, 0, 0, 360);

    // draw circular setor room
    SectorRoom * s1 = newSectorRoom("Kitchen", 15, 240);
    SectorRoom * s2 = newSectorRoom("WC", 10, -6);
    SectorRoom * s3 = newSectorRoom("Bedroom", 28, 70);

    s1->draw(s1, 2.8 - wall, 6.2);
    s1->put_windows(s1, 1, 1.8, 6.2);
    s2->draw(s2, 2.8 - wall, 6.2);
    s2->put_windows(s2, 1, 2.20, 6.2);
    s3->draw(s3, 2.8 - wall, 6.2);
    s3->put_windows(s3, 1, 2.20, 6.2);

    // Draw Hall
    //drawHall(6.2, 1.5);

    /*glLineStipple(2, 0x6DB6);
    glEnable(GL_LINE_STIPPLE);
    glBegin(GL_LINES);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(0.0f, 4.0f);
    glEnd();
    glDisable(GL_LINE_STIPPLE);*/

    // Draw Compass
    drawCompassRose(5, -6, 60);

    //people representation
    drawCircle(0.3, 4, -3, 0, 360);

    // flush drawing commands and swap
    glutSwapBuffers();
}

// press key function
void keyboard(unsigned char key, int x, int y)
{
    printf("Press key = %c\n", key);

    if(key == 'q')
    {
        exit(0);
    }
    if(key == '=')
    {
        zoom_in(scale);
        printf("%f\n", zoom_k);
    }
    if(key == '-')
    {
        zoom_out(scale);
        printf("%f\n", zoom_k);
    }

    glutPostRedisplay();
}

// Set up the rendering state
void SetupRC(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

}

void reshape(int w, int h)
{
    //GLdouble aspectRatio;
    double aspectRatio = (double)w / h;
    double sx = aspectRatio > 1.0f ? aspectRatio : 1.0f;
    double sy = aspectRatio > 1.0f ? 1.0f : 1.0f/aspectRatio;
    // set view port
    // view port will be reshape with the windows
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);

    // set projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(-sx * 16, sx * 16, -sy * 16, sy * 16);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char* argv[])
{
    // initial configs
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);

    // create windows
    glutInitWindowPosition(200, 100);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Casinha de ED");

    printf("Hello world!\n");

    // register GLUT callbacks
    glutDisplayFunc(RenderScene);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);

    // Render context
    SetupRC();

    // execute the loop
    glutMainLoop();
    return 0;
}
