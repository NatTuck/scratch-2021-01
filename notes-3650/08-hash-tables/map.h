#ifndef MAP_H
#define MAP_H

typedef struct pair {
    char* key;
    char* val;
    struct pair* next;
} pair;

typedef struct map {
    pair** data;
    long   size;
    long   capacity;
} map;

typedef struct map_it {
    // these are unowned references
    map* mm;
    long ii;
    pair* item;
} map_it;

map* make_map();
void free_map(map* mm);
// put: map makes owned copies of key, val
void map_put(map* mm, const char* key, const char* val);
// get: returns an unowned reference to val
char* map_get(map* mm, const char* key);
void map_del(map* mm, const char* key);

map_it iterate(map* mm);
int    has_next(map_it it);
map_it next(map_it it);

#endif
