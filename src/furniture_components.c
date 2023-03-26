#include "furniture_components.h"
#include <stdio.h>
#include <stdlib.h>
#include <GL/freeglut.h>
#define _USE_MATH_DEFINES
#include <math.h>

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