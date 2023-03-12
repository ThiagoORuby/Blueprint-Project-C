// UTILS HEADER
#ifndef UTILS_H
#define UTILS_H

#define WALL 0.15f

extern float scale;
extern float zoom_k;

// zoom functions
void enable_zoom();
void zoom_in(float);
void zoom_out(float);

// Draw Text
void drawChar(float, float, char);
void drawWord(float, float, char *);

#endif // UTILS_h
