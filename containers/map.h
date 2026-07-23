#ifndef MAP_H
#define MAP_H

#include <stddef.h> // size_t
#include <stdint.h> // uint64_t

// NOTE len is in bytes
typedef uint64_t (*HashFn) (const void* data, size_t len); // uint64_t hash
typedef int (*CompareFn) (const void* a, const void* b, size_t len); // returns 0 if *a and *b matches

typedef struct MapNode {
    void* key;
    void* val;
    size_t k_sz;
    size_t v_sz;
    struct MapNode* next;
} map_node_t;

typedef struct {
    map_node_t** buckets;
    size_t num_buckets;
    size_t node_count;
    float load_factor;
    HashFn hash_fn;
    CompareFn compare_fn;
} map_t;

// Initialization / Cleanup (Stack)
int map_init(map_t* m, size_t num_buckets, float load_factor, HashFn hash_fn, CompareFn compare_fn);
void map_free(map_t* m);
// Allocation / Deallocation (Heap)
map_t* map_new(size_t num_buckets, float load_factor, HashFn hash_fn, CompareFn compare_fn);
void map_delete(map_t* m);

int map_set(map_t* m, const void* key, size_t k_sz, const void* val, size_t v_sz);
void* map_get(map_t* m, const void* key, size_t k_sz); // user can modify internal pointed-to data, make sure to write at most v_sz bytes otherwise heap data will be corrupted
int map_remove(map_t* m, const void* key, size_t k_sz);

#endif