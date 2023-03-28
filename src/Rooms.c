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


void reverseList(Room** head_ref) {
  //1. If head is not null create three nodes
  //   prevNode - pointing to head,
  //   tempNode - pointing to head,
  //   curNode - pointing to next of head
  if(*head_ref != NULL) {
    Room* prevNode = *head_ref;
    Room* tempNode = *head_ref;
    Room* curNode = (*head_ref)->next;
    
    //2. assign next and previous of prevNode 
    //   as itself to make the first node as last 
    //   node of the reversed list
    prevNode->next = prevNode;
    prevNode->prev = prevNode;
    
    while(curNode != *head_ref) {
      //3. While the curNode is not head adjust links 
      //   (unlink curNode and link it to the reversed list 
      //   from front and modify curNode and prevNode)  
      tempNode = curNode->next;

      curNode->next = prevNode;
      prevNode->prev = curNode;
      (*head_ref)->next = curNode;
      curNode->prev = *head_ref;

      prevNode = curNode;
      curNode = tempNode;
    }

    //4. Make prevNode (last node) as head
    *head_ref = prevNode->prev;
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