#ifndef VECTOR_FUNC_H
#define VECTOR_FUNC_H

double dist_points(double p1[], double p2[]);
double dist_points2(double x1, double y1, double x2, double y2);

void middle_point(double p1[], double p2[], double middle[]);
//void middle_point2(double x1, double y1, double x2, double y2, double middle[]);

double dist_line(double p[], double m, double b);

void intersection(double line1[], double line2[], double intersect[]);

double _coef(double points[][2]);
double _coef2(double p1[], double p2[]);

void displace_point(double p[], double m, double length, double displace[], int dir);

#endif // VECTOR_H