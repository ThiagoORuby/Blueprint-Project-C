#ifndef FURNITURE_H
#define FURNITURE_H

// CLOSET
/*
guarda roupas
*/

// BEDROOM
/*
cama
penteadeira
*/


// draw an ellipse
void drawEllipse(double x0, double y0, double a, double b, double bg_angle, double end_angle, int factor);

// WC

// draw the toilet
void draw_toilet(double x0, double y0);

// bath sink
void draw_bath_sink(double x0, double y0);

//box
void draw_box(double x0, double y0);

//shower
void draw_shower(double x0, double y0);


// SERVICE AREA
/*
lavanderia
maquina de lavar
*/

// KITCHEN

// draw sink
void draw_sink(double x0, double y0);

// draw stove
void draw_stove(double x0, double y0);

//draw refrigerator
void draw_refrigerator(double x0, double y0);

//draw cabinet
void draw_cabinet(double x0, double y0);

//draw kitchen island
void draw_island(double x0, double y0);



// LIVING ROOM
/*
tapete
sofás 2 tipos
hack
televisao
*/

// DINING ROOM
/*
mesa
cadeiras
*/

// STUDY ROOM
/*
escrivaninha
computador
...
*/

void draw_plant(double x0, double y0);

void draw_vase(double x0, double y0);

#endif //FURNITURE_H
