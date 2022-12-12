#include <stdio.h>
#include "list/list.h"

int main(int argc, char **argv) {
    struct LIST *list = build_empty_list();
    
    for (int i = 0; i < 10; i++) {
        char *c = malloc(sizeof(c));
        *c = i + 65;
        push_back(list, (void *) c);
    }

    L_NODE *item = list->first;
    for (int i = 0; i < list->size; i++) {
        char c = *(char *)item->value;
        printf("Char: %c\n", c);
        item = item->next;
    }

    char *str = to_string(list);

    printf("str: %s\n", str);

    return 0;
}