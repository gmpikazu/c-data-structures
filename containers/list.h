#ifndef LIST_H
#define LIST_H

#include <stddef.h> // size_t

typedef struct ListNode {
    void* data;
    struct ListNode* next;
} list_node_t;

list_node_t* node_new(const void* data, size_t element_size);

typedef struct {
    list_node_t* head;
    list_node_t* tail;
    size_t size;
    size_t element_size;
} list_t;

// Initialization / Cleanup (Stack)
int list_init(list_t* l, size_t element_size);
void list_free(list_t* l);
// Allocation / Deallocation (Heap)
list_t* list_new(size_t element_size);
void list_delete(list_t* l);

int list_push_back(list_t* l, const void* data);
int list_push_front(list_t* l, const void* data);
int list_insert_at(list_t* l, size_t idx, const void* data);
int list_pop_back(list_t* l);
int list_pop_front(list_t* l);
int list_remove_at(list_t* l, size_t idx);
void* list_get(const list_t* l, size_t idx);
void list_clear(list_t* l);

#endif // LIST_H