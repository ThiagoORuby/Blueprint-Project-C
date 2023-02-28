#include <stdio.h>
#include <stdlib.h>
#include <GL/freeglut.h>
#include <GL/glu.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "src/rect_components.h"
#include "src/utils.h"


// Draw a circle
void drawCircle(float raio, float centroX, float centroY, int bg_angle, int end_angle)
{
    glPushMatrix();
    glTranslatef(centroX, centroY, 0.0);

    glBegin(GL_LINE_STRIP);
    for (int i = bg_angle; i < end_angle; i++)
    {
        float degInRad = i * M_PI / 180;
        glVertex2f(cos(degInRad) * raio, sin(degInRad) * raio);
    }
    if(end_angle == 360) glVertex2f(cos(0) * raio, sin(0) * raio);
    glEnd();



    glPopMatrix();
}

// Draw House's Room
void drawRoom(float raio, float centroX, float centroY, int bg_angle, int end_angle)
{
    glPushMatrix();
    glTranslatef(centroX, centroY, 0.0);

    glBegin(GL_LINE_LOOP);
    glVertex2f(0, 0);
    for (int i = bg_angle; i < end_angle; i++)
    {
        float degInRad = i * M_PI / 180;
        glVertex2f(cos(degInRad) * raio, sin(degInRad) * raio);
    }
    glVertex2f(0, 0);
    glEnd();

    glPopMatrix();
}

// Draw house's hall
void drawHall(float radius, float size)
{
    float f = 0.15;
    float walls[4] = {-size/2 - f, -size/2, size/2, size/2 + f};

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
void drawCompassRose(float x0, float y0, float angle)
{
    float length = 0.5;
    float rad = angle * M_PI / 180;

    // m1 = b/a -> V = (a, b)
    float m1 = tan(rad);
    float mod_V1 = sqrt(pow(1, 2) + pow(m1, 2));
    float m2 = -1/m1;
    float mod_V2 = sqrt(pow(1, 2) + pow(m2, 2));

    // original coordenates
    // x, y = (x0 + u * a/|V|, y0 + u * b/|V|)
    float x = 1/mod_V1*length;
    float y = m1/mod_V1*length;

    // perpendicular coordenates
    float px = 1/mod_V2*length;
    float py = m2/mod_V2*length;

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

    float wall = 0.15;

    // Purple
    glColor3f(1.0f, 0.0f, 0.4f);

    drawCircle(6.2, 0, 0, 0, 360);
    drawCircle(6.2 - wall, 0, 0, 0, 360);
    //drawRoom(6.2 - wall, 0, 0, 90, 90 + 30);

    glColor3f(1.0f, 0.0f, 0.4f);
    drawCircle(2.8, 0, 0, 0, 360);
    drawCircle(2.8 - wall, 0, 0, 0, 360);

    // Draw Hall
    drawHall(6.2, 1.5);

    // Draw Compass
    drawCompassRose(3, 4, 30);
    drawCompassRose(-3, 4, 45);
    drawCompassRose(-3, -4, 110);
    //drawCompassRose(3, -4, 80);
    drawCompassRose(3, -4, 194);
    drawCompassRose(3, -6, 290);
    //drawCompassRose(-3, -4, 90);

    //glRasterPos2i(10, 10);
    //glutBitmapString(GLUT_BITMAP_8_BY_13, "Hello, World!");

    // Yellow
    glColor3f(1.0f, 1.0f, 0.0f);
    drawCircle(6.2, 0, 0, 0, 30);
    drawCircle(6.2 - wall, 0, 0, 0, 30);

    // Green
    glColor3f(0.4f, 1.0f, 0.2f);
    drawCircle(6.2 - wall/3, 0, 0, 0, 30);
    drawCircle(6.2 - wall/3*2, 0, 0, 0, 30);
    drawCircle(0.3, 2, 2, 0, 360);

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
    //GLfloat aspectRatio;
    float aspectRatio = (float)w / h;
    float sx = aspectRatio > 1.0f ? aspectRatio : 1.0f;
    float sy = aspectRatio > 1.0f ? 1.0f : 1.0f/aspectRatio;
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
