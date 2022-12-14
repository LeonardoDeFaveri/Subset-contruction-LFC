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

void push_sorted(struct LIST *list, void *value, cmp_function cmp) {
    if (list == NULL) { return; }

    L_NODE *node = malloc(sizeof(L_NODE));
    node->value = value;
    node->next = NULL;

    if (list->size == 0) {
        list->first = list->last = node;
        list->size = 1;
        return;
    }

    struct L_NODE *previous = list->first;
    struct L_NODE *other = list->first->next;
    short prev_res = cmp(value, previous->value);
    if (prev_res <= 0 || list->size == 1) {
        if (prev_res >= 0) {
            list->first->next = node;
            list->last = node;
        } else {
            node->next = list->first;
            list->first = node;
        }
        list->size++;
        return;
    }
    
    while (other != NULL) {
        short res = cmp(value, other->value);
        
        if (prev_res > 0 && res <= 0) { // previous->value < value <= other->value
            node->next = other;
            previous->next = node;
            break;
        } else { // value > other->value
            prev_res = res;
            previous = other;
            other = other->next;
        }
    }

    if (other == NULL) {
        previous->next = node;
        list->last = node;
    }
    list->size++;
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