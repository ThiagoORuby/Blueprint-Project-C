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

    // methods
    void (* draw)(struct sector *, double, double);
    void (* put_windows)(struct sector *, int, double, double);
    void (* put_doors)(struct sector *, int, double, double);

} SectorRoom;

// constructor
SectorRoom * newSectorRoom(char *, double, double);
// destructor
void destroySectorRoom(SectorRoom *);

void _midle_angle_point(double *, double, double, double, double);
void drawSector(SectorRoom *, double, double);
void putSectorWindows(SectorRoom *, int, double, double);
void putSectorDoors(SectorRoom *, int, double, double);

#endif // ROOM_COMP_H