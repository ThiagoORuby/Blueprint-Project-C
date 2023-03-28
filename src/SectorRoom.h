#ifndef SECTOR_ROOM_H
#define SECTOR_ROOM_H

// Classe SectorRoom
typedef struct sector{
    // atributes
    char name[20];
    double area;
    double bg_angle;
    double end_angle;
    double wall_area;
    double true_area;
    double delta;
    double edge_points[4][2]; // limits of the room
    double wall_points[2][2]; // points to next room

    // methods
    void (* draw)(struct sector *, double, double, int, int);
    void (* put_windows)(struct sector *,int,  int, double, double);
    void (* put_doors)(struct sector *, int [][2], double [], double, double);
    void (* put_furnitures)(struct sector *);
} SectorRoom;

// constructor
SectorRoom * newSectorRoom(char * name, double area, double wall_area, double angle);
// destructor
void destroySectorRoom(SectorRoom *);

// Desenha SectorRoom
void drawSector(SectorRoom * obj, double r1, double r2, int wall_pos, int inv);

// Desenha janelas
// type = (-1) low window | (1) high window
void putSectorWindows(SectorRoom * obj, int type, int qnt, double length, double r1);

// Desenha portas
void putSectorDoors(SectorRoom * obj, int mapping[][2], double lengths[], double r1, double r2);

#endif // ROOM_COMP_H