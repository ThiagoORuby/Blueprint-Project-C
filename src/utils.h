// UTILS HEADER
#ifndef UTILS_H
#define UTILS_H

#define WALL 0.15f
#define DOOR_BOX 0.03f
#define R1 3.4
#define R2 6.9
#define WF ((R2 - 0.15) - (R1 + 0.15))*0.15

extern float scale;
extern float zoom_k;

// zoom functions
void enable_zoom();
void zoom_in(float factor);
void zoom_out(float factor);

// Draw Text
void drawChar(float x0, float y0, char c);
void drawWord(float x0, float y0, char * string);

// Compass rose
void drawCompassRose(double x0, double y0, double angle);

#endif // UTILS_h
