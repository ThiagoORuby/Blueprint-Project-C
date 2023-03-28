#include "SectorRoom.h"
#include "utils.h"
#include "vector.h"
#include <GL/freeglut.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <string.h>
#include <stdio.h>


// Constructor
SectorRoom * newSectorRoom(char * name, double area, double wall_area, double bg_angle)
{
    SectorRoom * obj = malloc(sizeof(SectorRoom));
    strcpy(obj->name, name);
    obj->area = area;
    obj->true_area = wall_area;
    obj->delta = -1;
    obj->bg_angle = bg_angle;
    obj->end_angle = -1;
    obj->draw = drawSector;
    obj->put_windows = putSectorWindows;
    obj->put_doors = putSectorDoors;
    return obj;
}

// DRAW_ARC
void _draw_arc(double r, double cx, double cy, double bg_angle, double end_angle, double pass)
{
    glBegin(GL_LINE_STRIP);
    for (double i = bg_angle; (pass > 0) ? i < end_angle : i > end_angle; i+=pass)
    {
        glVertex2f(cx + cos(i) * r, cy + sin(i) * r);
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
void drawSector(SectorRoom * obj, double r1, double r2, int wall_pos, int inv)
{
    int factor, f, pos = 0;
    double dangle = 0, deg, text_pos[2], angle;
    double m, b, x1, x2, y1, y2;
    char info[10];

    obj->true_area = obj->area + obj->wall_area;

    // draw 2 walls
    angle = obj->bg_angle;
    for(int i = -1; i < 2; i+=2)
    {
        // set wall color
        glColor3f(1.0f, 0.0f, 0.4f);
        // draw intern line
        angle += dangle*inv;
        deg = angle * M_PI / 180;

        // edge points of the room
        obj->edge_points[pos][0] = cos(deg) * r1; obj->edge_points[pos++][1] = sin(deg) * r1;
        obj->edge_points[pos][0] = cos(deg) * r2; obj->edge_points[pos++][1] = sin(deg) * r2;

        glBegin(GL_LINE_LOOP);
        glVertex2f(cos(deg) * r1, sin(deg) * r1);
        glVertex2f(cos(deg) * r2, sin(deg) * r2);
        glEnd();

        // find delta angle
        if(i == -1)
        {
            dangle = (360 * obj->true_area)/(M_PI * (r2 - r1) * (r2 + r1));
            obj->delta = dangle;
        }
    }

    // draw wall line
    x1 = obj->edge_points[wall_pos+1][0]; y1 = obj->edge_points[wall_pos+1][1];
    x2 = obj->edge_points[wall_pos+2][0]; y2 = obj->edge_points[wall_pos+2][1];
    factor = (x1 >= 0) ? -1 : 1;
    m = (y2 - y1)/(x2 - x1);
    b = y1 - m*x1;
    b = b + wall_pos*factor*(WALL*(sqrt(1 + m*m)));

    // find the (x,y) intersections of line and circles
    x1 = (-2*m*b + factor*(-1)*sqrt(4*m*m*b*b - 4*(1 + m*m)*(b*b - r1*r1)))/(2*(1 + m*m));
    y1 = m * x1 + b;
    x2 = (-2*m*b + factor*(-1)*sqrt(4*m*m*b*b - 4*(1 + m*m)*(b*b - r2*r2)))/(2*(1 + m*m));
    y2 = m * x2 + b;

    obj->end_angle = acos(x2/r2);

    // save the wall coordenates
    obj->wall_points[0][0] = x1; obj->wall_points[0][1] = y1;
    obj->wall_points[1][0] = x2; obj->wall_points[1][1] = y2;

    // draw
    glBegin(GL_LINE_LOOP);
    glVertex2f(x1, y1);
    glVertex2f(x2, y2);
    glEnd();

    // draw info text
    _midle_angle_point(text_pos, obj->bg_angle, dangle, r1, r2);
    glColor3f(1, 1, 1);
    sprintf(info, "%.2fm2", obj->area);
    drawWord(text_pos[0], text_pos[1], info); // draw area
    drawWord(text_pos[0], text_pos[1] + 0.3, obj->name); // draw name
}

// draw windows
// type = (-1) low window | (1) high window
void putSectorWindows(SectorRoom * obj, int type, int qnt, double length, double r2)
{
    double arc, k, deltab, points[4][2], angles[2];
    if(obj->delta == -1) return (void) printf("Please, draw first");

    arc = (obj->delta * M_PI/180)*r2;
    k = (arc - length)/2;
    deltab = k/r2;

    if(k < 0) return (void) printf("Don't has space in this room");

    // begin line
    angles[0] = obj->bg_angle*M_PI/180;
    points[0][0] = cos(angles[0] + deltab)*r2; points[0][1] = sin(angles[0] + deltab)*r2;
    points[1][0] = cos(angles[0] + deltab)*(r2 - WALL); points[1][1] = sin(angles[0] + deltab)*(r2 - WALL);
    // end line
    angles[1] = (obj->bg_angle+obj->delta)*M_PI/180;
    points[2][0] = cos(angles[1] - deltab)*r2; points[2][1] = sin(angles[1] - deltab)*r2;
    points[3][0] = cos(angles[1] - deltab)*(r2 - WALL); points[3][1] = sin(angles[1] - deltab)*(r2 - WALL);
    
    if(type == 1)
    {
        glLineStipple(1, 0xF0F0);
        glEnable(GL_LINE_STIPPLE);
        glColor3f(0.6f, 0.6f, 0.4f); // red
    }
    else
    {
        glColor3f(0.4f, 1.0f, 0.2f); // green color
    }

    for(int i = 0; i < 3; i+=2)
    {
        glBegin(GL_LINE_LOOP);
        glVertex2f(points[i][0], points[i][1]);
        glVertex2f(points[i+1][0], points[i+1][1]);
        glEnd();
    }

    // draw the conected arcs
    if(type == -1) 
    {
        glColor3f(1.0f, 1.0f, 0.0f); // yellow color
        _draw_arc(r2, 0, 0, angles[0] + deltab, angles[1] - deltab, 0.01);
        _draw_arc(r2 - WALL, 0, 0, angles[0] + deltab, angles[1] - deltab, 0.001);
        glColor3f(0.4f, 1.0f, 0.2f); // green color
    }
    _draw_arc(r2 - WALL/3, 0, 0, angles[0] + deltab, angles[1] - deltab, 0.001);
    _draw_arc(r2 - WALL/3*2, 0, 0, angles[0] + deltab, angles[1] - deltab, 0.001);
    if(type == 1) glDisable(GL_LINE_STIPPLE);
    return;
}

// DOOR TYPES
void _draw_linear_boxes(double points[][2], double limit_line[][2], double length, double *m, int dir, int order[])
{
    double disp[3] = {DOOR_BOX, DOOR_BOX + length, 2*DOOR_BOX + length};
    int k = 0;

    *m = _coef(limit_line);
    for(int i = 1; i < 4; i++)
    {
            displace_point(points[0], *m, disp[i-1], points[i*2], dir);
            displace_point(points[1], *m, disp[i-1], points[i*2 + 1], dir);
    }

    // draw the boxes
    glColor3f(0.4f, 1.0f, 0.2f); // green color
    for(int i = 0; i < 2; i++)
    {
        glBegin(GL_LINE_LOOP);
        for(int j = 0; j < 4; j++)
        {
            glVertex2f(points[order[j] + k][0], points[order[j] + k][1]);
        }
        glEnd();
        k += 4;
    }

    // draw the conection lines
    glColor3f(0.8, 0.2, 0); // red color
    for(int i = 2; i < 4; i++)
    {
        glBegin(GL_LINE_LOOP);
        glVertex2f(points[i][0], points[i][1]);
        glVertex2f(points[i+2][0], points[i+2][1]);
        glEnd();
    }
}

void _simple_door(double points[][2], double limit_line[][2], double length, int dir, int inverse)
{
    double m, m1, bg_angle, end_angle, dangle, leaf_points[4][2], leaf_disp[2] = {0, length};
    double * bg_point, * end_point;
    int order[4] = {0, 2, 3, 1}, k;
    k = (limit_line[0][1] > 0) ? 1 : -1;

    _draw_linear_boxes(points, limit_line, length, &m, dir, order);

    // check the configs
    if(inverse == 1) {bg_point = points[2]; end_point = points[4];}
    else {bg_point = points[4]; end_point = points[2]; k *= (-1); dir *= (-1);}
    

    // draw the door leaf
    glColor3f(0.4f, 1.0f, 0.2f); // green color
    m1 = _coef2(points[0], points[1]);
    for(int j = 0; j < 2; j++)
    {
        displace_point(bg_point, m1, leaf_disp[j], leaf_points[j], k);
        displace_point(leaf_points[j], m, DOOR_BOX, leaf_points[j+2], dir);
    }

    glBegin(GL_LINE_LOOP);
    for(int j = 0; j < 4; j++)
    {
        glVertex2f(leaf_points[order[j]][0], leaf_points[order[j]][1]);
    }
    glEnd();

    // draw the movement door arc
    bg_angle = atan2(leaf_points[1][1] - bg_point[1], leaf_points[1][0] - bg_point[0]);
    end_angle = atan2(end_point[1] - bg_point[1], end_point[0] - bg_point[0]);
    dangle = end_angle - bg_angle;

    if(dangle > M_PI) dangle = 2*M_PI - dangle;
    else if(dangle < -M_PI) dangle = 2*M_PI + dangle;

    printf("begin = %.2lf, end = %.2lf\n\n", bg_angle, end_angle);
    glColor3f(0.8, 0.2, 0);
    _draw_arc(length, bg_point[0], bg_point[1], bg_angle, bg_angle+dangle, 0.01);
}

void _sliding_door(double points[][2], double limit_line[][2], double length, int dir, int wall)
{
    double m, m1, leaf_points[4][2], leaf_disp[2] = {0.075, 0.105};
    int order[4] = {0, 2, 3, 1}, k = 0;

    double lengths[] = {1.2, 1.0, 0.8, 0.6, 0.4, 0.3, 0.2};
    if(length < 0) // dynamic length
    {
        for(int i = 0; i < 5; i++)
        {
            if((dist_points(limit_line[0], limit_line[1]) - lengths[i])/2 > 0.1)
            {
                length = lengths[i];
                break;
            } 
        }
    }

    _draw_linear_boxes(points, limit_line, length, &m, (-1)*dir, order);

    // draw the leaves by point[3] and point[5]
    glColor3f(0.4f, 1.0f, 0.2f); // green color
    k = (limit_line[0][1] > 0) ? 1 : -1;
    m1 = _coef2(points[0], points[1]);
    for(int i = 3; i < 5; i++)
    {
        for(int j = 0; j < 2; j++)
        {
            displace_point(points[i], m1, leaf_disp[j], leaf_points[j], k*wall);
            displace_point(leaf_points[j], m, length/2, leaf_points[j+2], dir*(-1));
        }

        glBegin(GL_LINE_LOOP);
        for(int j = 0; j < 4; j++)
        {
            glVertex2f(leaf_points[order[j]][0], leaf_points[order[j]][1]);
        }
        glEnd();

        dir *= (-1);
        k *= (-1);
    }
}

void _sliding_circular_door(SectorRoom * obj, double length, int leafs, double r1)
{
    double arc, k, deltab, points[4][2], angles[2];
    double lengths[] = {1.2, 1.0, 0.8, 0.6, 0.4, 0.3, 0.2};
    if(obj->delta == -1) return (void) printf("Please, draw first");

    arc = (obj->delta * M_PI/180)*r1;

    if(length < 0) // dynamic length
    {
        for(int i = 0; i < 5; i++)
        {
            if((arc - lengths[i])/2 > 0.1)
            {
                length = lengths[i];
                break;
            } 
        }
    }

    k = (arc - length)/2;
    deltab = (k+0.15)/r1;

    if(k < 0) return (void) printf("Don't has space in this room");

    // begin line
    angles[0] = obj->bg_angle*M_PI/180;
    points[0][0] = cos(angles[0] + deltab)*r1; points[0][1] = sin(angles[0] + deltab)*r1;
    points[1][0] = cos(angles[0] + deltab)*(r1 - WALL); points[1][1] = sin(angles[0] + deltab)*(r1 - WALL);
    // end line
    angles[1] = (obj->bg_angle+obj->delta)*M_PI/180;
    points[2][0] = cos(angles[1] - deltab)*r1; points[2][1] = sin(angles[1] - deltab)*r1;
    points[3][0] = cos(angles[1] - deltab)*(r1 - WALL); points[3][1] = sin(angles[1] - deltab)*(r1 - WALL);

    glColor3f(0.4f, 1.0f, 0.2f); // green color
    for(int i = 0; i < 3; i+=2)
    {
        glBegin(GL_LINE_LOOP);
        glVertex2f(points[i][0], points[i][1]);
        glVertex2f(points[i+1][0], points[i+1][1]);
        glEnd();
    }

    // draw the conected arcs
    glColor3f(0.8, 0.2, 0); // red color
    _draw_arc(r1, 0, 0, angles[0] + deltab, angles[1] - deltab, 0.01);
    _draw_arc(r1 - WALL, 0, 0, angles[0] + deltab, angles[1] - deltab, 0.001);
    
    // draw the leaves
    const double END_LEAF = deltab + length/(2*r1);
    glColor3f(0.4f, 1.0f, 0.2f); // green color
    // first leaf
    _draw_arc(r1 - 0.045, 0, 0, angles[0] + deltab, angles[1] - END_LEAF, 0.001);
    _draw_arc(r1 - 0.075, 0, 0, angles[0] + deltab, angles[1] - END_LEAF, 0.001);
    // second leaf
    _draw_arc(r1 - 0.075, 0, 0, angles[1] - deltab, angles[0] + END_LEAF, -0.001);
    _draw_arc(r1 - 0.105, 0, 0, angles[1] - deltab, angles[0] + END_LEAF, -0.001);
    // conect leaves
    glBegin(GL_LINE_LOOP);
    glVertex2f((r1 - 0.045)*cos(angles[1] - END_LEAF), (r1 - 0.045)*sin(angles[1] - END_LEAF));
    glVertex2f((r1 - 0.105)*cos(angles[0] + END_LEAF), (r1 - 0.105)*sin(angles[0] + END_LEAF));
    glEnd();
    
}

// mapping : wall -> {enable, type} => {{}, {}, {}} 
void putSectorDoors(SectorRoom * obj, int mapping[][2], double lengths[], double r1, double r2)
{
    // testing with wall -1 -> edges (0, 1);
    double m, points[8][2], dist;
    int factor;

    factor = (obj->wall_points[0][0] > 0) ? 1 : -1;
    m = _coef(obj->wall_points);
    dist = (dist_points(obj->wall_points[0], obj->wall_points[1]) - (lengths[0] + 2*DOOR_BOX))/2;

    displace_point(obj->wall_points[0], m, dist, points[1], factor);

    // wall 0 enable
    if(mapping[0][0])
    {
        displace_point(obj->edge_points[0], m, dist, points[0], factor);
        if(mapping[0][1]) // type 1
        {
            _simple_door(points, obj->wall_points, lengths[0], factor, 1);
        }
        else // type 0
        {
            _sliding_door(points, obj->wall_points, lengths[0], factor, 1);
        }
    }
    if (mapping[1][0])// wall 1 enable
    {
        _sliding_circular_door(obj, lengths[1], 1, r1);
    }
    if(mapping[2][0]) // wall 2 enable
    {
        displace_point(obj->edge_points[2], m, dist, points[0], factor);
        if(mapping[2][1]) // type 1
        {
            _simple_door(points, obj->wall_points, lengths[2], factor, -1);
        }
        else // type 0
        {
            _sliding_door(points, obj->wall_points, lengths[2], factor, -1);
        }   
    }

    // if is the Hall
    if(!strcmp(obj->name, "Hall"))
    {
        _sliding_circular_door(obj, lengths[1], 1, r2);
    }


}
