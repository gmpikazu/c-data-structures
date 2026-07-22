#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h> // size_t
#include <stdbool.h> // true/false

typedef struct {
    void* arr;
    size_t size;
    size_t capacity;
    size_t element_size;
} vector_t;

// Initialization / Cleanup (Stack)
int vector_init(vector_t* v, size_t element_size, size_t initial_capacity);
void vector_free(vector_t* v);
// Allocation / Deallocation (Heap)
vector_t* vector_new(size_t element_size, size_t initial_capacity);
void vector_delete(vector_t* v);

int vector_push_back(vector_t* v, const void* data);
void* vector_get(const vector_t* v, size_t idx);
bool vector_is_empty(const vector_t* v);
size_t vector_size(const vector_t* v);

#endif // VECTOR_H