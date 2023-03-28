// UTILS FUNCTIONS
#include "utils.h"
#include <GL/glut.h>
#include <GL/freeglut.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

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

// random funcs
double random_int(int interval[2])
{
    int random = rand() % (interval[1] - interval[0]) + interval[0];
    return (double) random;
}

double random_double(double beg, double end)
{
    double random = (rand() % (int) ((end - beg)*1e2))*1e-2 + beg;
    return random;
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

void drawText(char* text, int x, int y) {
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluOrtho2D(0, 800, 0, 600); // definindo a janela de exibição do texto
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity(); // definindo a cor do texto
  glRasterPos2i(x, y); // definindo a posição na tela do texto

  // exibindo o texto na tela
  for (int i = 0; i < strlen(text); i++) {
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, text[i]);
  }

  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
}


void drawGround(double w, double h)
{
    glColor3f(0.5, 0.5, 0.5);
    glBegin(GL_LINE_LOOP);
    glVertex2f(w/2, h/2);
    glVertex2f(w/2, -h/2);
    glVertex2f(-w/2, -h/2);
    glVertex2f(-w/2, h/2);
    glEnd();
}

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

void drawHouseArea(double r2)
{
    // Purple
    glColor3f(1.0f, 0.0f, 0.4f);
    // draw house area
    drawCircle(r2, 0, 0, 0, 360);
    drawCircle(r2 - WALL, 0, 0, 0, 360);
}

void drawMainRoom(double r1)
{
    double area;
    char area_info[20];
    // draw main room area
    glColor3f(1.0f, 0.0f, 0.4f);
    drawCircle(r1, 0, 0, 0, 360);
    drawCircle(r1 - WALL, 0, 0, 0, 360);
    // draw main room text
    glColor3f(1,1,1);
    drawWord(0 - r1*1/4, 0, "Sala Estar/Jantar");
    area = r1*r1*M_PI;
    sprintf(area_info, "%.2fm2", area);
    drawWord(0 - r1*1/4 + 0.2, 0 - 0.3, area_info);
}
