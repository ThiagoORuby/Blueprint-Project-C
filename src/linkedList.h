#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct list{
    int value;
    struct list * next;
} List;

List * create_node(int value);

void insert_begin(List ** list, int value);

void insert_end(List ** list, int value);

void print_list(List * list);

#endif // LINKED_LIST_H