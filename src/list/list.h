#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include "../digraph/digraph.h"

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

/// This is the signature of a function that compares `v1` and `v2`.
/// The return value must be
/// - `0` if `v1=v2`
/// - `>0` if `v1>v2`
/// - `<0` if `v1<v2`
typedef short (*cmp_function)(const void *v1, const void *v2);

struct LIST* build_empty_list();
void destroy_list(struct LIST *list);
void push_back(struct LIST *list, void *value);
/// Adds `value` into `list` in a sorted way. If every element is inserted this way,
/// the final result should be a list sorted as indicated by `cmp`.
void push_sorted(struct LIST *list, void *value, cmp_function cmp);
/// Adds every element of `other` to `list`. `other` is consumed
/// so after the invocation it's an empty list.
void push_list(struct LIST *list, struct LIST *other);
void* pop_first(struct LIST *list);
char* to_string(struct LIST *list);
unsigned long length(struct LIST *list);

#endif