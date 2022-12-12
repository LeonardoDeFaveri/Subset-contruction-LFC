#ifndef MERGE_FIND_SET_H
#define MERGE_FIND_SET_H

#include "../map/map.h"
#include "../list/list.h"

struct MFS_ITEM {
    void *value;
    struct MFS_ITEM *representative;
};

struct MFS_ITEM *new_mfs_item(void *value, struct MFS_ITEM *representative);

struct MFS {
    hashmap *sets;
};

struct MFS *make_mfs();
struct MFS_ITEM *make_set(struct MFS *mfs, void *value);
struct MFS_ITEM *find_set(struct MFS_ITEM *item);
struct MFS_ITEM *merge(struct MFS *mfs, struct MFS_ITEM *item1, struct MFS_ITEM *item2);

#endif