// UTILS FUNCTIONS
#include "utils.h"
#include <GL/freeglut.h>
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
    else if (zoom_k < 0) zoom_k = 0;
}

void zoom_out(float factor)
{
    if(zoom_k >= 0) zoom_k = zoom_k - factor;
    else if (zoom_k < 0) zoom_k = 0;
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
    float factor = 0.15;
    glRasterPos2f(x, y);
    
        glPushMatrix();
        glTranslatef(x, y, 1.0f);
        glScalef(0.0015, 0.0015, 1);
        for(int i = 0; i < strlen(string); i++) 
            glutStrokeCharacter(GLUT_STROKE_ROMAN, string[i]);
        glPopMatrix();
}


