#ifndef LIST_H
#define LIST_H

#include "../digraph.h"
#include <stdlib.h>

struct L_NODE {
    void *value;
    struct L_NODE *next;
};
typedef struct L_NODE L_NODE;

struct LIST {
    L_NODE *first;
    L_NODE *last;
    unsigned long size;
};

struct LIST* build_empty_list();
void destroy_list(struct LIST *list);
void push_back(struct LIST *list, void *value);
void push_list(struct LIST *list, struct LIST *other);
void* pop_first(struct LIST *list);
char* to_string(struct LIST *list);
unsigned long length(struct LIST *list);

#endif