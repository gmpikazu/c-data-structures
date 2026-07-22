#include "list.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

list_node_t* node_new(const void* data, size_t element_size) {
    list_node_t* node = malloc(sizeof(list_node_t));
    if (node == NULL) { return NULL; }
    node->data = malloc(element_size);
    if (node->data == NULL) {
        free(node);
        return NULL;
    }
    memcpy(node->data, data, element_size);
    return node;
}

int list_init(list_t* l, size_t element_size) {
    if (l == NULL) { return 1; }
    
    l->head = NULL;
    l->tail = NULL;
    l->size = 0;
    l->element_size = element_size;
    return 0;
}

void list_free(list_t* l) {
    if (l == NULL) { return; }
    
    list_node_t* curr = l->head;
    while (curr != NULL) {
        list_node_t* next = curr->next;
        free(curr->data);
        free(curr);
        curr = next;
    }
    l->head = NULL;
    l->tail = NULL;
    l->size = 0;
}

list_t* list_new(size_t element_size) {
    list_t* l = malloc(sizeof(list_t));
    if (l == NULL) { return NULL; }
    if (list_init(l, element_size) != 0) {
        free(l);
        return NULL;
    }
    return l;
}

void list_delete(list_t* l) {
    if (l == NULL) { return; }

    list_free(l);
    free(l);
}

int list_push_back(list_t* l, const void* data) {
    if (l == NULL || data == NULL) { return 1; }
    
    list_node_t* node = node_new(data, l->element_size);
    if (node == NULL) { return 1; }
    node->next = NULL;
    
    if (l->head == NULL) {
        l->head = node;
        l->tail = node;
    }
    else {
        l->tail->next = node;
        l->tail = node;
    }
    l->size++;
    return 0;
}

int list_push_front(list_t* l, const void* data) {
    if (l == NULL || data == NULL) { return 1; }
    
    list_node_t* node = node_new(data, l->element_size);
    if (node == NULL) { return 1; }
    if (l->head == NULL) {
        node->next = NULL;
        l->tail = node;
    }
    else {
        node->next = l->head;
    }
    l->head = node;
    l->size++;
    return 0;
}

int list_insert_at(list_t *l, size_t idx, const void *data) {
    if (l == NULL || data == NULL) { return 1; }
    if (idx > l->size) { return 1; }
    
    if (idx == 0) {
        return list_push_front(l, data);
    }
    else if (idx == l->size) {
        return list_push_back(l, data);
    }
    else {
        assert(l->size > 0);
        assert(l->head != NULL);
        list_node_t* curr = l->head;
        list_node_t* prev = NULL;
        size_t i = 0;
        while (i < idx) {
            prev = curr;
            curr = curr->next;
            i++;
        }
        list_node_t* node = node_new(data, l->element_size);
        if (node == NULL) { return 1; }
        node->next = curr;
        prev->next = node;
        l->size++;
        return 0;
    }
}

int list_pop_back(list_t* l) {
    if (l == NULL) { return 1; }
    if (l->size == 0) { return 1; }

    assert(l->head != NULL);
    assert(l->tail != NULL);

    // single element
    if (l->head == l->tail) {
        free(l->head->data);
        free(l->head);
        l->head = NULL;
        l->tail = NULL;
        l->size = 0;
        return 0;
    }

    // more elements
    list_node_t* curr = l->head;
    while (curr->next != l->tail) {
        curr = curr->next;
    }
    list_node_t* prev_tail = l->tail;
    curr->next = NULL;
    l->tail = curr;
    l->size--;
    
    free(prev_tail->data);
    free(prev_tail);
    return 0;
}

int list_pop_front(list_t* l) {
    if (l == NULL) { return 1; }
    if (l->size == 0) { return 1; }
    
    assert(l->head != NULL);
    list_node_t* prev_head = l->head;
    l->head = prev_head->next; // can be NULL
    l->size--;
    
    // when l->size becomes 0, l->head becomes NULL
    if (l->head == NULL) {
        l->tail = NULL;
    }
    
    free(prev_head->data);
    free(prev_head);
    return 0;
}

int list_remove_at(list_t* l, size_t idx) {
    if (l == NULL) { return 1; }
    if (l->size == 0) { return 1; }
    if (idx >= l->size) { return 1; }
    
    if (idx == 0) {
        return list_pop_front(l);
    }
    else if (idx == l->size-1) {
        return list_pop_back(l);
    }
    else {
        assert(l->size > 0);
        assert(l->head != NULL);
        list_node_t* curr = l->head;
        list_node_t* prev = NULL;
        size_t i = 0;
        while (i < idx) {
            prev = curr;
            curr = curr->next;
            i++;
        }
        prev->next = curr->next;
        l->size--;
        free(curr->data);
        free(curr);
        return 0;
    }
}

void* list_get(const list_t* l, size_t idx) {
    if (l == NULL) { return NULL; }
    if (l->size == 0) { return NULL; }
    if (idx >= l->size) { return NULL; }
    
    assert(l->head != NULL);
    list_node_t* curr = l->head;
    size_t i = 0;
    while (i < idx) {
        curr = curr->next;
        i++;
    }
    return curr->data;
}

void list_clear(list_t* l) {
    list_free(l); // keeps pointer alive
    list_init(l, l->element_size);
}