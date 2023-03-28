#include <stdio.h>
#include <stdlib.h>
#include "linkedList.h"


List * create_node(int value)
{
    List * node = malloc(sizeof(List));
    node->value = value;
    node->next = NULL;
    return node;
}

void insert_begin(List ** list, int value)
{
    List * temp;
    List * new = create_node(value);

    if(*list == NULL)
    {
        *list = new;
    }
    else
    {
        temp = *list;
        new->next = temp;
        *list = new;
    }
}

void insert_end(List ** list, int value)
{
    List * temp;
    List * new = create_node(value);

    if(*list == NULL)
    {
        *list = new;
    }
    else
    {
        temp = *list;
        while(temp->next != NULL) temp = temp->next;
        temp->next = new;
    }
}

void print_list(List * list)
{
    List * temp = list;

    while(temp != NULL)
    {
        printf("%d ", temp->value);
        temp = temp->next;
    }

    printf("\n");
}