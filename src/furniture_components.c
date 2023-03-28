#include "furniture_components.h"
#include <stdio.h>
#include <stdlib.h>
#include <GL/freeglut.h>
#define _USE_MATH_DEFINES
#include <math.h>

#include "utils.h"

void drawEllipse(double x0, double y0, double a, double b, double bg_angle, double end_angle, int factor)
{
    double x, y;

    glBegin(GL_LINE_STRIP);
    for(double t = bg_angle; t <= end_angle; t += 0.01*factor)
    {
        x = x0 + a * cos(t);
        y = y0 + b * sin(t);
        glVertex2d(x, y);
    }
    glEnd();
}

void draw_toilet(double x0, double y0)
{
    double w = 0.40, h = 0.4, b = 0.15, f = 0.8, beg = - M_PI - M_PI/12, end = M_PI/12;
    drawEllipse(x0, y0, w/3, h/2, 0, 2*M_PI, 1);
    drawEllipse(x0, y0 + h/2*f, w/2, h, beg, end, 1);
    glBegin(GL_LINE_LOOP);
    glVertex2d(x0 + w/2*cos(beg), y0 + h/2*f + h*sin(beg));
    glVertex2d(x0 + w/2*cos(end), y0 + h/2*f + h*sin(end));
    glVertex2d(x0 + w/2*cos(end), y0 + h/2*f + h*sin(end) + w/3);
    glVertex2d(x0 + w/2*cos(beg), y0 + h/2*f + h*sin(beg) + w/3);
    glEnd();
}


void draw_plant(double x0, double y0)
{
    glColor3f(0.1, 0.6, 0.3);
    glPushMatrix();
    for(double i = 0; i <= 360; i += 360.0/8)
    {
        glTranslatef(1, -1, 0);
        glRotatef(i, 0, 0, 1);
        glTranslatef(-1, 1, 0);
        glBegin(GL_LINE_LOOP);
        glVertex2d(1, -1);
        glVertex2d(1 + 0.05, -1 + 0.05);
        glVertex2d(1 + 0.3, -1);
        glVertex2d(1 + 0.05, -1 - 0.05);
        glEnd();
    }
    glPopMatrix();
}

void draw_vase(double x0, double y0)
{
    // draw plants
    double s = 0.3;
    glColor3f(0.2, 0.5, 0.1);
    for(double i = 0; i <= 2*M_PI; i += M_PI/12)
    {
        glBegin(GL_LINE_LOOP);
        glVertex2d(0, -1);
        glVertex2d(0 + s*cos(i), -1 + s*sin(i));
        glEnd();
        if(s == 0.3) s = 0.23;
        else s = 0.3;
    }

    glColor3f(1, 1, 1);
    drawCircle(0.2, 0, -1, 0, 360);
    drawCircle(0.15, 0, -1, 0, 360);
}