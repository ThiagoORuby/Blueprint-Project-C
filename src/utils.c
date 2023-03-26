// UTILS FUNCTIONS
#include "utils.h"
#include <GL/freeglut.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>

float scale = 0.1f;
float zoom_k = 1.0f;

// Zoom functions
void enable_zoom()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glScalef(zoom_k, zoom_k, 1.0f);
}

void zoom_in(float factor)
{
    if(zoom_k >= 0) zoom_k = zoom_k + factor;
    else if (zoom_k < 0) zoom_k = 0.1;
}

void zoom_out(float factor)
{
    if(zoom_k >= 0) zoom_k = zoom_k - factor;
    else if (zoom_k < 0) zoom_k = 0.1;
}

// Util: draw a char
void drawChar(float x, float y, char c)
{
    glPushMatrix();
    glTranslatef(x, y, 1.0f);
    glScalef(0.002, 0.002, 1);
    glutStrokeCharacter(GLUT_STROKE_ROMAN, c);
    glPopMatrix();
}

// Util: draw a word
void drawWord(float x, float y, char * string)
{
    float factor = 0.0015;
    glRasterPos2f(x, y);
    glPushMatrix();
    glTranslatef(x, y, 1.0f);
    glScalef(factor, factor, 1);
    for(int i = 0; i < strlen(string); i++) 
        glutStrokeCharacter(GLUT_STROKE_ROMAN, string[i]);
    glPopMatrix();
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

