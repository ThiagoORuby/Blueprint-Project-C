#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Rooms.h"
#include "utils.h"

// initialize list of rooms
Room room_types[8];

// names
char * room_names[] = {"Hall", "Closet", "Quarto", "WC", "Cozinha", "Lavanderia", "Sala Estudos", "Escritorio"};
// list of put_furniture functions
// max_min
int area_ranges[][2] = {{5, 6}, {5,6}, {12, 18}, {6,8}, {12, 15}, {5, 7}, {8, 10}, {8, 10}};

// Creating a new Room
Room * newRoom(char * name, int interval[2], double wf)
{
    Room * room = malloc(sizeof(Room));
    // creating super class
    SectorRoom * super = newSectorRoom(name, -1, wf, -1);

    room->super = super;
    for(int i = 0; i < 2; i++)
        room->area_interval[i] = interval[i];
    room->prev = NULL;
    room->next = NULL;
    
    return room;
}

void pushRoom(Room ** list, Room type, double area, double wf)
{
    Room * temp;
    Room * room = newRoom(type.name, type.area_interval, wf);
    room->super->area = area;
    if(*list == NULL)
    {
        room->prev = room;
        room->next = room;
        *list = room;
        return;
    }
    else
    {
        temp = (*list)->prev;
        room->prev = temp;
        room->next = *list;
        temp->next = room;
        (*list)->prev = room;  
    }
}

void printRooms(Room * list)
{
    Room * temp = list;
    printf("{\n");
    do{
        printf("Name: %s, Int: %d - %d, area = %.2lf, angle = %.2lf \n", 
        temp->super->name, temp->area_interval[0], temp->area_interval[1], temp->super->area, temp->super->bg_angle);
        temp = temp->next;
    } while(temp != list);
    printf("}\n");
}


// Creating the list of room types
void setRoomTypes()
{
    for(int i = 0; i < 8; i++)
    {
        Room room;
        strcpy(room.name, room_names[i]);
        for(int j = 0; j < 2; j++) room.area_interval[j] = area_ranges[i][j];
        room_types[i] = room;
    }
}