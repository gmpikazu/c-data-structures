#ifndef BST_H
#define BST_H

#include <stddef.h>

// returns negative if a < b, positive if a > b, zero if a == b
// takes a_len bytes from a, b_len bytes from b, and compares them logically
typedef int (*BST_CompareFn) (const void* a, size_t a_len, const void* b, size_t b_len);

typedef struct BST_Node {
    void* key;
    void* val;
    size_t k_sz;
    size_t v_sz;
    struct BST_Node* left;
    struct BST_Node* right;
} bst_node_t;

typedef struct {
    bst_node_t* root;
    size_t count;
    BST_CompareFn compare_fn;
} bst_t;

// Initialization / Cleanup (Stack)
int bst_init(bst_t* t, BST_CompareFn compare_fn);
void bst_free(bst_t* t);
// Allocation / Deallocation (Heap)
bst_t* bst_new(BST_CompareFn compare_fn);
void bst_delete(bst_t* t);

int bst_insert(bst_t* t, void* key, size_t k_sz, void* val, size_t v_sz);
int bst_remove(bst_t* t, void* key, size_t k_sz);
void* bst_get(bst_t* t, void* key, size_t k_sz);

#endif