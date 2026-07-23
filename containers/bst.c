#include "bst.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>

static int default_compare_fn(const void* a, size_t a_len, const void* b, size_t b_len) {
    size_t min_len = (a_len < b_len) ? a_len : b_len;
    int res = memcmp(a, b, min_len);
    // arbitrary: if common prefix matches, then length determines which is smaller
    if (res == 0 && a_len != b_len) {
        return (a_len < b_len) ? -1 : 1;
    }
    return res;
}

int bst_init(bst_t* t, BST_CompareFn compare_fn) {
    if (t == NULL) { return 1; }

    t->root = NULL;
    t->count = 0;
    t->compare_fn = (compare_fn == NULL) ? default_compare_fn : compare_fn;
    return 0;
}

static void recursive_delete(bst_node_t* root) {
    if (root == NULL) { return; }
    
    if (root->left == NULL && root->right == NULL) {
        free(root->key);
        free(root->val);
        free(root);
    }
    else {
        recursive_delete(root->left);
        recursive_delete(root->right);
        free(root->key);
        free(root->val);
        free(root);
    }
}

void bst_free(bst_t* t) {
    if (t == NULL) { return; }

    recursive_delete(t->root);
    t->root = NULL;
    t->count = 0;
}

bst_t* bst_new(BST_CompareFn compare_fn) {
    bst_t* t = malloc(sizeof(bst_t));
    if (t == NULL) { return NULL; }
    if (bst_init(t, compare_fn) != 0) {
        free(t);
        return NULL;
    }
    return t;
}

void bst_delete(bst_t* t) {
    if (t == NULL) { return; }
    
    bst_free(t);
    free(t);
}

// NOTE iterative but uses clean double pointer approach
int bst_insert(bst_t* t, void* key, size_t k_sz, void* val, size_t v_sz) {
    if (t == NULL || key == NULL || val == NULL) { return 1; }
    
    // (*curr_ptr) becomes the curr node itself
    bst_node_t** curr_ptr = &t->root;
    while (*curr_ptr != NULL) {
        int res = t->compare_fn(key, k_sz, (*curr_ptr)->key, (*curr_ptr)->k_sz);
        if (res < 0) {
            // address of left member, not value
            curr_ptr = &(*curr_ptr)->left;
        }
        else if (res > 0) {
            // address of right member, not value
            curr_ptr = &(*curr_ptr)->right;
        }
        else { // res == 0
            void* new_val = malloc(v_sz);
            if (new_val == NULL) { return 1; }
            memcpy(new_val, val, v_sz);
            free((*curr_ptr)->val);
            (*curr_ptr)->val = new_val;
            (*curr_ptr)->v_sz = v_sz;
            return 0;
        }
    }
    // reaching here means there were no duplicates
    bst_node_t* new_node = malloc(sizeof(bst_node_t));
    if (new_node == NULL) { return 1; }
    void* new_key = malloc(k_sz);
    if (new_key == NULL) {
        free(new_node);
        return 1;
    }
    void* new_val = malloc(v_sz);
    if (new_val == NULL) {
        free(new_node);
        free(new_key);
        return 1;
    }
    memcpy(new_key, key, k_sz);
    memcpy(new_val, val, v_sz);
    *new_node = (bst_node_t){.key = new_key, .val = new_val, .k_sz = k_sz, .v_sz = v_sz, .left = NULL, .right = NULL};
    // NOTE curr_ptr now points to a memory region (in struct) that points to NULL
    // we write the address of new_node into that memory region (this handles root == NULL case too)
    *curr_ptr = new_node;
    t->count++;
    return 0;
}

// NOTE iterative but uses clean double pointer approach
int bst_remove(bst_t* t, void* key, size_t k_sz) {
    if (t == NULL || key == NULL) { return 1; }
    
    bst_node_t** curr_ptr = &t->root;
    while (*curr_ptr != NULL) {
        int res = t->compare_fn(key, k_sz, (*curr_ptr)->key, (*curr_ptr)->k_sz);
        if (res < 0) {
            // address of left member, not value
            curr_ptr = &(*curr_ptr)->left;
        }
        else if (res > 0) {
            // address of right member, not value
            curr_ptr = &(*curr_ptr)->right;
        }
        else { // res == 0
            break; // target found
        }
    }
    if (*curr_ptr == NULL) return 1; // not found
    
    // perform deletion
    bst_node_t* target = NULL;
    bst_node_t* child = NULL;
    if ((*curr_ptr)->left != NULL && (*curr_ptr)->right != NULL) {
        // find leftmost successor starting from right child
        bst_node_t** succ_ptr = &(*curr_ptr)->right;
        while ((*succ_ptr)->left != NULL) {
            succ_ptr = &(*succ_ptr)->left;
        }
        // shallow swap values of curr and succ
        void *temp_key = (*curr_ptr)->key;
        void *temp_val = (*curr_ptr)->val;
        size_t temp_k_sz = (*curr_ptr)->k_sz;
        size_t temp_v_sz = (*curr_ptr)->v_sz;
        (*curr_ptr)->key = (*succ_ptr)->key;
        (*curr_ptr)->val = (*succ_ptr)->val;
        (*curr_ptr)->k_sz = (*succ_ptr)->k_sz;
        (*curr_ptr)->v_sz = (*succ_ptr)->v_sz;
        // succ holds pointers to the data we want to free
        (*succ_ptr)->key = temp_key;
        (*succ_ptr)->val = temp_val;
        (*succ_ptr)->k_sz = temp_k_sz;
        (*succ_ptr)->v_sz = temp_v_sz;

        target = *succ_ptr;
        child = target->right; // no left child
        *succ_ptr = child;
    }
    else { // includes single child and no child
        target = *curr_ptr;
        bst_node_t *child = (target->left != NULL) ? target->left : target->right;
        *curr_ptr = child;
    }
    free(target->key);
    free(target->val);
    free(target);
    t->count--;
    return 0;
}

void* bst_get(bst_t *t, void *key, size_t k_sz) {
    if (t == NULL || key == NULL) { return NULL; }

    bst_node_t* curr = t->root;
    while (curr != NULL) {
        int res = t->compare_fn(key, k_sz, curr->key, curr->k_sz);
        if (res < 0) {
            curr = curr->left;
        }
        else if (res > 0) {
            curr = curr->right;
        }
        else { // res == 0
            return curr->val;
        }
    }
    return NULL;
}