#ifndef SECTOR_ROOM_H
#define SECTOR_ROOM_H

typedef struct sector{
    // atributes
    char name[20];
    double area;
    double bg_angle;
    double delta;
    double edge_points[4][2]; // limits of the room
    double wall_points[2][2]; // points to next room
    struct sector * prev; // pointer to previous sector
    struct sector * next; // pointer to next sector

    // methods
    void (* draw)(struct sector *, double, double, int);
    void (* put_windows)(struct sector *,int,  int, double, double);
    void (* put_doors)(struct sector *, int, double, double);

} SectorRoom;

// constructor
SectorRoom * newSectorRoom(char *, double, double);
// destructor
void destroySectorRoom(SectorRoom *);

void drawSector(SectorRoom *, double, double, int);
void putSectorWindows(SectorRoom *, int, int, double, double);
void putSectorDoors(SectorRoom *, int, double, double);

#endif // ROOM_COMP_H