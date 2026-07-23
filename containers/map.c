#include "map.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define FNV_OFFSET_BASIS_64 14695981039346656037ULL
#define FNV_PRIME_64 1099511628211ULL

// FNV-1a Algorithm in public domain
static uint64_t default_hash_fn(const void* data, size_t len) {
    const uint8_t *ptr = (const uint8_t *)data;
    uint64_t hash = FNV_OFFSET_BASIS_64;
    for (size_t i = 0; i < len; i++) {
        hash ^= ptr[i];
        hash *= FNV_PRIME_64;
    }
    return hash;
}

static int default_compare_fn(const void* a, const void* b, size_t len) {
    return memcmp(a, b, len);
}

int map_init(map_t *m, size_t num_buckets, float load_factor, HashFn hash_fn, CompareFn compare_fn) {
    if (m == NULL) { return 1; }

    m->num_buckets = (num_buckets == 0) ? 7 : num_buckets;
    // NOTE must clear allocate to initialize all pointers to NULL
    m->buckets = calloc(m->num_buckets, sizeof(map_node_t*));
    if (m->buckets == NULL) { return 1; }
    m->node_count = 0;
    m->load_factor = load_factor;
    m->hash_fn = (hash_fn == NULL) ? default_hash_fn : hash_fn;
    m->compare_fn = (compare_fn == NULL) ? default_compare_fn : compare_fn;
    return 0;
}

void map_free(map_t* m) {
    if (m == NULL) { return; }

    for (size_t i = 0; i < m->num_buckets; i++) {
        map_node_t* curr = m->buckets[i];
        while (curr != NULL) {
            map_node_t* next_node = curr->next;
            free(curr->key);
            free(curr->val);
            free(curr);
            curr = next_node;
        }
    }
    free(m->buckets);
    m->buckets = NULL;
    m->num_buckets = 0;
    m->node_count = 0;
}

map_t* map_new(size_t num_buckets, float load_factor, HashFn hash_fn, CompareFn compare_fn) {
    map_t* m = malloc(sizeof(map_t));
    if (m == NULL) { return NULL; }
    if (map_init(m, num_buckets, load_factor, hash_fn, compare_fn) != 0) {
        free(m);
        return NULL;
    }
    return m;
}

void map_delete(map_t* m) {
    if (m == NULL) { return; }

    map_free(m);
    free(m);
}

static int map_resize(map_t* m) {
    if (m == NULL) { return 1; }

    assert(m->num_buckets != 0);
    size_t new_num_buckets = m->num_buckets * 2;
    map_node_t** new_buckets = calloc(new_num_buckets, sizeof(map_node_t*));
    if (new_buckets == NULL) { return 1; }
    // traverse through each chain in all the buckets
    for (size_t i = 0; i < m->num_buckets; i++) {
        map_node_t* curr = m->buckets[i];
        while (curr != NULL) {
            map_node_t* next_node = curr->next; // save before overwrite
            uint64_t hash = m->hash_fn(curr->key, curr->k_sz);
            size_t new_idx = hash % new_num_buckets;
            // prepend to new_buckets[new_idx]
            curr->next = new_buckets[new_idx]; // curr->next overwritten
            new_buckets[new_idx] = curr;
            curr = next_node; // go to previously-saved curr->next
        }
    }
    free(m->buckets); // free old array
    m->buckets = new_buckets;
    m->num_buckets = new_num_buckets;
    return 0;
}

int map_set(map_t* m, const void* key, size_t k_sz, const void* val, size_t v_sz) {
    if (m == NULL || key == NULL || val == NULL) { return 1; }

    uint64_t hash = m->hash_fn(key, k_sz);
    size_t idx = hash % m->num_buckets;
    map_node_t* curr = m->buckets[idx];
    while (curr != NULL) {
        if (curr->k_sz == k_sz && m->compare_fn(curr->key, key, k_sz) == 0) {
            void* new_val = malloc(v_sz);
            if (new_val == NULL) { return 1; }
            // only modifies state once malloc succeeds
            memcpy(new_val, val, v_sz);
            free(curr->val);
            curr->val = new_val;
            return 0;
        }
        curr = curr->next;
    }
    // reaching here means we didn't pass any duplicates
    map_node_t* new_map_node = malloc(sizeof(map_node_t));
    if (new_map_node == NULL) { return 1; }
    void* new_key = malloc(k_sz);
    if (new_key == NULL) { 
        free(new_map_node);
        return 1; 
    }
    void* new_val = malloc(v_sz);
    if (new_val == NULL) {
        free(new_map_node);
        free(new_key);
        return 1;
    }
    // filling key, val, and linking to new_map_node
    memcpy(new_key, key, k_sz);
    memcpy(new_val, val, v_sz);
    // create temporary map_node_t on stack and copies bytes into malloc-ed new_map_node
    *new_map_node = (map_node_t){.key = new_key, .val = new_val, .k_sz = k_sz, .v_sz = v_sz};
    // prepend to m->buckets[idx], doesn't care about curr
    new_map_node->next = m->buckets[idx];
    m->buckets[idx] = new_map_node;
    m->node_count++;
    if ((float)m->node_count / m->num_buckets > m->load_factor) {
      // if resize fails, buckets is overloaded but not corrupted, return 0 still
      map_resize(m); 
    }
    return 0;
}

void* map_get(map_t* m, const void* key, size_t k_sz) {
    if (m == NULL || key == NULL) { return NULL; }

    uint64_t hash = m->hash_fn(key, k_sz);
    size_t idx = hash % m->num_buckets;
    map_node_t* curr = m->buckets[idx];
    while (curr != NULL) {
        if (curr->k_sz == k_sz && m->compare_fn(curr->key, key, k_sz) == 0) {
            return curr->val;
        }
        curr = curr->next;
    }
    return NULL;
}

int map_remove(map_t* m, const void* key, size_t k_sz) {
    if (m == NULL || key == NULL) { return 1; }
    uint64_t hash = m->hash_fn(key, k_sz);
    size_t idx = hash % m->num_buckets;
    map_node_t* curr = m->buckets[idx];
    map_node_t* prev = NULL;
    while (curr != NULL) {
        if (curr->k_sz == k_sz && m->compare_fn(curr->key, key, k_sz) == 0) {
            if (prev == NULL) {
                m->buckets[idx] = curr->next;
            }
            else {
                prev->next = curr->next;
            }
            free(curr->key);
            free(curr->val);
            free(curr);
            m->node_count--;
            return 0;
        }
        prev = curr;
        curr = curr->next;
    }
    return 1;
}