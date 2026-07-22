#include "vector.h"
#include <stdlib.h> // malloc
#include <string.h> // memcpy
#include <stdint.h> // SIZE_MAX

int vector_init(vector_t* v, size_t element_size, size_t initial_capacity) {
    if (v == NULL) { return 1; }

    v->element_size = element_size;
    v->capacity = (initial_capacity == 0) ? 4 : initial_capacity;
    v->size = 0;
    if (element_size != 0 && initial_capacity > SIZE_MAX / element_size) { return 1; }
    v->arr = malloc(initial_capacity * element_size);
    if (v->arr == NULL) { return 1; }
    return 0;
}

void vector_free(vector_t* v) {
    if (v == NULL) { return; }

    free(v->arr);
    v->arr = NULL;
}

vector_t* vector_new(size_t element_size, size_t initial_capacity) {
    vector_t* v = malloc(sizeof(vector_t));
    if (v == NULL) { return NULL; }
    if (vector_init(v, element_size, initial_capacity) != 0){
        free(v);
        return NULL; 
    }
    return v;
}

void vector_delete(vector_t* v) {
    if (v == NULL) { return; }

    vector_free(v);
    free(v);
}

int vector_push_back(vector_t* v, const void* data) {
    if (v == NULL || data == NULL) { return 1; }

    if (v->size == v->capacity) {
        size_t new_cap = v->capacity == 0 ? 4 : v->capacity * 2;
        if (v->element_size != 0 && new_cap > SIZE_MAX / v->element_size) { return 1; }
        void* new_arr = realloc(v->arr, new_cap * v->element_size);
        if (new_arr == NULL) { return 1; }
        v->arr = new_arr;
        v->capacity = new_cap;
    }
    size_t offset = v->size * v->element_size;
    // cast void* to char* for byte offset
    memcpy((char*)v->arr + offset, data, v->element_size);
    v->size++;
    return 0;
}

void* vector_get(const vector_t* v, size_t idx) {
    if (v == NULL) { return NULL; }
    if (idx >= v->size) { return NULL; }

    return (char*)v->arr + idx * v->element_size;
}

bool vector_is_empty(const vector_t* v) {
    if (v == NULL) { return true; }

    return v->size == 0;
}

size_t vector_size(const vector_t* v) {
    if (v == NULL) { return 0; }
    
    return v->size;
}