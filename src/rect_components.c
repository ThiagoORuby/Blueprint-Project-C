/*
Rectangular Components
*/
#include "rect_components.h"
#include <GL/freeglut.h>

void drawRectangle(float x, float y, float width, float height)
{
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

void outlined_quad(float x, float y, float width, float height)
{
    int walk[4][2] = {{0, 0},{1, 0}, {0, -1}, {-1, 0}};
    glBegin(GL_LINE_LOOP);

    for(int i = 0; i < 4; i++)
    {
        x += walk[i][0]*(!(i % 2) ? height : width);
        y += walk[i][1]*(!(i % 2) ? height : width);
        glVertex2f(x, y);
    }

    glEnd();
}

void generate_window(float x, float y, char dir, float width)
{
    float length = 0.3, spc = 0.1, spch = 0.3;

    glColor3f(1.0f, 1.0f, 0.0f);
    if(dir == 'v') outlined_quad(x, y, length, -width);
    else outlined_quad(x, y, width, length);

    glColor3f(0.4f, 1.0f, 0.2f);
    if(dir == 'v')
    {
        outlined_quad(x, y, spch, spc);
        outlined_quad(x, y + width, spch, spc);
        outlined_quad(x + spc, y, spc, -(width - spc));
    }
    else
    {
        outlined_quad(x, y, spc, spch);
        outlined_quad(x + width - spc, y, spc, spch);
        outlined_quad(x + spc, y - spc, width - 2*spc, spc);
    }
}



/*
To rotate a figure in an angle
glTranslatef(0, 0, 0.0f);
glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
*/
