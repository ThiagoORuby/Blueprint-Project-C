#ifndef ROOMS_H
#define ROOMS_H
#include "SectorRoom.h"

// Classe Room
typedef struct room{

    struct sector * super; // inheritance of super class

    // atributes
    char name[20];
    int area_interval[2];
    struct room * prev; // pointer to previous room
    struct room * next; // pointer to next room
} Room;


// --- LISTA CIRCULAR COM ROOM ---

// Cria nova Room
Room * newRoom(char * name, int interval[], double wf);

// Adiciona nova Room na lista circular 
void pushRoom(Room ** list, Room type, double area, double wf);

// Inverte a lista circular
void reverseList(Room** list);

// Printa a lista circular
void printRooms(Room * list);

// -------------------------------

// Define uma lista com os tipos diferentes de Rooms
void setRoomTypes();


#endif // ROOMS_H