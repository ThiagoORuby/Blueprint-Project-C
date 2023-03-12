#include "SectorRoom.h"
#include "utils.h"
#include <GL/freeglut.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>
#include <stdio.h>


// Constructor
SectorRoom * newSectorRoom(char * name, double area, double bg_angle)
{
    SectorRoom * obj = malloc(sizeof(SectorRoom));
    strcpy(obj->name, name);
    obj->area = area;
    obj->delta = -1;
    obj->bg_angle = bg_angle;
    obj->draw = drawSector;
    obj->put_windows = putSectorWindows;
    obj->put_doors = putSectorDoors;
    return obj;
}

// DRAW_ARC
void _draw_arc(double r, double bg_angle, double end_angle, double pass)
{
    glBegin(GL_LINE_STRIP);
    for (double i = bg_angle; i < end_angle; i+=pass)
    {
        glVertex2f(cos(i) * r, sin(i) * r);
    }
    if(end_angle == 360) glVertex2f(cos(0) * r, sin(0) * r);
    glEnd();
}

// MIDLE MIDLE-ANGLE POINT 
void _midle_angle_point(double text_pos[], double bg_angle, double delta_angle, double r1, double r2)
{
    double deg = (bg_angle + delta_angle/2) * M_PI / 180;
    text_pos[0] = (cos(deg) * (r1 + r2))/2;
    text_pos[1] = (sin(deg) * (r1 + r2))/2;
}

// draw a Sector
void drawSector(SectorRoom * obj, double r1, double r2)
{
    int factor, f, pos = 0;
    double cte = 0.15;
    double dangle = 0, deg, text_pos[2], angle;
    double m, b, x1, x2, y1, y2;
    char info[10];

    // draw 2 walls
    angle = obj->bg_angle;
    for(int i = -1; i < 2; i+=2)
    {
        // set wall color
        glColor3f(1.0f, 0.0f, 0.4f);
        // draw intern line
        angle += dangle;
        deg = angle * M_PI / 180;

        // edge points of the room
        obj->edge_points[pos][0] = cos(deg) * r1; obj->edge_points[pos++][1] = sin(deg) * r1;
        obj->edge_points[pos][0] = cos(deg) * r2; obj->edge_points[pos++][1] = sin(deg) * r2;

        glBegin(GL_LINE_LOOP);
        glVertex2f(cos(deg) * r1, sin(deg) * r1);
        glVertex2f(cos(deg) * r2, sin(deg) * r2);
        glEnd();

        // draw extern line
        factor = (cos(deg) * r1 >= 0) ? 1 : -1;
        m = (sin(deg) * r2 - sin(deg) * r1)/(cos(deg) * r2 - cos(deg) * r1);
        b = sin(deg) * r1 - m * cos(deg) * r1;
        b = b + i*factor*(cte*(sqrt(1 + m*m)));
        // find the (x,y) intersections of line and circles
        x1 = (-2*m*b + factor*sqrt(4*m*m*b*b - 4*(1 + m*m)*(b*b - r1*r1)))/(2*(1 + m*m));
        y1 = m * x1 + b;
        x2 = (-2*m*b + factor*sqrt(4*m*m*b*b - 4*(1 + m*m)*(b*b - r2*r2)))/(2*(1 + m*m));
        y2 = m * x2 + b;
        // draw
        //glColor3f(1.0f, 0.0f, 0.8f);
        glBegin(GL_LINE_LOOP);
        glVertex2f(x1, y1);
        glVertex2f(x2, y2);
        glEnd();

        // find delta angle
        if(i == -1)
        {
            dangle = (360 * obj->area)/(M_PI * (r2 - r1) * (r2 + r1));
            obj->delta = dangle;
        }
    }

    // draw info text
    _midle_angle_point(text_pos, obj->bg_angle, dangle, r1, r2);
    glColor3f(1, 1, 1);
    sprintf(info, "%.2fm2", obj->area);
    drawWord(text_pos[0], text_pos[1], info); // draw area
    drawWord(text_pos[0], text_pos[1] + 0.3, obj->name); // draw name
}

// draw windows
void putSectorWindows(SectorRoom * obj, int qnt, double length, double r2)
{
    double arc, k, deltab, points[4][2], angles[2];
    if(obj->delta == -1) return (void) printf("Please, draw first");

    arc = (obj->delta * M_PI/180)*r2;
    k = (arc - length)/2;
    deltab = k/r2;
    glColor3f(0.4f, 1.0f, 0.2f); // green color

    angles[0] = obj->bg_angle*M_PI/180;
    points[0][0] = cos(angles[0] + deltab)*r2; points[0][1] = sin(angles[0] + deltab)*r2;
    points[1][0] = cos(angles[0] + deltab)*(r2 - WALL); points[1][1] = sin(angles[0] + deltab)*(r2 - WALL);

    glBegin(GL_LINE_LOOP);
    glVertex2f(points[0][0], points[0][1]);
    glVertex2f(points[1][0], points[1][1]);
    glEnd();

    angles[1] = (obj->bg_angle+obj->delta)*M_PI/180;
    points[2][0] = cos(angles[1] - deltab)*r2; points[2][1] = sin(angles[1] - deltab)*r2;
    points[3][0] = cos(angles[1] - deltab)*(r2 - WALL); points[3][1] = sin(angles[1] - deltab)*(r2 - WALL);

    glBegin(GL_LINE_LOOP);
    glVertex2f(points[2][0], points[2][1]);
    glVertex2f(points[3][0], points[3][1]);
    glEnd();

    // draw the conected arcs
    glColor3f(1.0f, 1.0f, 0.0f); // yellow color
    _draw_arc(r2, angles[0] + deltab, angles[1] - deltab, 0.01);
    _draw_arc(r2 - WALL, angles[0] + deltab, angles[1] - deltab, 0.01);
    glColor3f(0.4f, 1.0f, 0.2f); // green color
    _draw_arc(r2 - WALL/3, angles[0] + deltab, angles[1] - deltab, 0.01);
    _draw_arc(r2 - WALL/3*2, angles[0] + deltab, angles[1] - deltab, 0.01);

    return;
}

void putSectorDoors(SectorRoom * obj, int qnt, double length, double r1)
{
    return;
}