#include "list.h"
#include <stdlib.h>
#include <stdio.h>

struct LIST* build_empty_list() {
    struct LIST* list = malloc(sizeof(struct LIST));
    list->first = NULL;
    list->last = NULL;
    list->size = 0;
    return list;
}

void destroy_list(struct LIST *list) {
    if (list == NULL) { return; }
    while (list->first != NULL) {
        L_NODE *node = list->first;
        list->first = list->first->next;
        free(node);
    }
    free(list);
    list = NULL;
}

void push_back(struct LIST *list, void *value) {
    if (list == NULL) { return; }

    char item = *(char*) value;
    L_NODE *node = malloc(sizeof(L_NODE));
    node->value = value;
    node->next = NULL;
    if (list->size == 0) {
        list->first = list->last = node;
        list->size = 1;
    } else {
        list->last->next = node;
        list->last = list->last->next;
        list->size++;
    }
}

void push_list(struct LIST *list, struct LIST *other) {
    if (list == NULL) {
        list = build_empty_list();
    }
    struct LIST *item = pop_first(other);

    while (item != NULL) {
        push_back(list, (void *)item);
        item = pop_first(other);
    }
}

void* pop_first(struct LIST *list) {
    if (list->size == 0) { return NULL; }

    void *value = list->first->value;
    list->first = list->first->next;
    list->size--;
    return value;
}

char* to_string(struct LIST *list) {
    if (list == NULL) return "";

    unsigned long len = length(list);
    char *str = malloc(sizeof(char) * (len + 1));
    for (int i = 0; i < len; i++) {
        void *item = pop_first(list);
        str[i] = *((char*) item);
        push_back(list, item);
    }
    str[len] = '\0';

    return str;
}

unsigned long length(struct LIST *list) {
    return list->size;
}