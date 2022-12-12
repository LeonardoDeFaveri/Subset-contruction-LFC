#include "merge_find_set.h"
#include <stdint.h>
#include <stdint.h>
#include <stdlib.h>

struct MFS_ITEM *new_mfs_item(void *value, struct MFS_ITEM *representative) {
    struct MFS_ITEM *item = malloc(sizeof(struct MFS_ITEM));
    item->value = value;
    item->representative = representative;
    return item;
}

struct MFS *make_mfs() {
    struct MFS *mfs = malloc(sizeof(struct MFS));
    mfs->sets = hashmap_create();
    return mfs;
}

struct MFS_ITEM *make_set(struct MFS *mfs, void *value) {
    struct LIST *list = build_empty_list();
    struct MFS_ITEM *item = new_mfs_item(value, NULL);
    // The representative of the first item, is the item itself
    item->representative = item;
    push_back(list, (void *) item);
    hashmap_set(mfs->sets, value, sizeof(value), (uintptr_t) list);
    return item;
}

struct MFS_ITEM *find_set(struct MFS_ITEM *item) {
    return item->representative;
}

/// Appends every element of `list2` to `list1` setting their representative to
/// `representative`.
void append_list(struct LIST *list1, struct LIST *list2, struct MFS_ITEM *representative) {
    void *item = pop_first(list2);
    while (item != NULL) {
        struct MFS_ITEM *mfs_item = (struct MFS_ITEM *) item;
        mfs_item->representative = representative;
        push_back(list1, item);
        item = pop_first(list2);
    }
}

struct MFS_ITEM *merge(struct MFS *mfs, struct MFS_ITEM *item1, struct MFS_ITEM *item2) {
    struct MFS_ITEM *representative1 = find_set(item1);
    struct MFS_ITEM *representative2 = find_set(item2);

    if (representative1 == representative2) { return representative1; }

    uintptr_t ptr1;
    if (!hashmap_get(mfs->sets, representative1->value, sizeof(representative1->value), &ptr1)) {
        return representative2;
    }
    uintptr_t ptr2;
    if (!hashmap_get(mfs->sets, representative2->value, sizeof(representative2->value), &ptr2)) {
        return representative1;
    }

    struct LIST *list1 = (struct LIST *) ptr1;
    struct LIST *list2 = (struct LIST *) ptr2;

    if (list1->size > list2->size) {
        append_list(list1, list2, representative1);
        hashmap_remove(mfs->sets, representative2->value, sizeof(representative2->value));
        destroy_list(list2);
        return representative1;
    } else {
        append_list(list2, list1, representative2);
        hashmap_remove(mfs->sets, representative1->value, sizeof(representative1->value));
        destroy_list(list1);
        return representative2;
    }
}