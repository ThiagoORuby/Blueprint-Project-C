#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double dist_points(double p1[], double p2[])
{
    return sqrt(pow(p2[0] - p1[0], 2) + pow(p2[1] - p1[1], 2));
}

double dist_points2(double x1, double y1, double x2, double y2)
{
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

void middle_point(double p1[], double p2[], double middle[]){
    for(int i = 0; i < 2; i++)
        middle[i] = (p1[i] + p2[i])/2;
}

double dist_line(double p[], double m, double b){
    return (-p[1] + m*p[0] + b)/sqrt(1 + m*m);
}

void intersection(double line1[], double line2[], double intersect[]){
    return;
}

double _coef(double points[][2]){
    return (points[0][1] - points[1][1])/(points[0][0] - points[1][0]);
}

double _coef2(double p1[], double p2[]){
    return (p2[1] - p1[1])/(p2[0] - p1[0]);
}

void displace_point(double p[], double m, double length, double displace[], int dir){
    double module = sqrt(1 + m*m);
    displace[0] = p[0] + dir*length*(1/module);
    displace[1] = p[1] + dir*length*(m/module);
}