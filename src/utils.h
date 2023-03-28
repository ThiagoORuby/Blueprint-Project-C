// UTILS HEADER
#ifndef UTILS_H
#define UTILS_H

#define WALL 0.15f
#define DOOR_BOX 0.03f
#define WWIDTH 800
#define WHEIGHT 600

extern float scale;
extern float zoom_k;

// zoom functions
void enable_zoom();
void zoom_in(float factor);
void zoom_out(float factor);

// random funcs
double random_int(int interval[2]);
double random_double(double beg, double end);

// Draw Text
void drawChar(float x0, float y0, char c);
void drawWord(float x0, float y0, char * string);
void drawText(char* text, int x, int y);

// Compass rose
void drawCompassRose(double x0, double y0, double angle);

// Draw House components
void drawGround(double w, double h);
void drawCircle(double raio, double centroX, double centroY, int bg_angle, int end_angle);
void drawHouseArea(double r2);
void drawMainRoom(double r1);

#endif // UTILS_H;
