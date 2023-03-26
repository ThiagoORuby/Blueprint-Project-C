#ifndef ROOMS_H
#define ROOMS_H
#include "SectorRoom.h"

typedef struct room{

    struct sector * super; // inheritance of super class
    // atributes
    char name[20];
    int area_interval[2];
    struct room * prev; // pointer to previous sector
    struct room * next; // pointer to next sector
} Room;

Room * newRoom(char * name, int interval[], double wf);

void pushRoom(Room ** list, Room type, double area, double wf);

void printRooms(Room * list);

void setRoomTypes();


#endif // ROOMS_H