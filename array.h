#ifndef STDINT_H
#define STDINT_H
#include <stdint.h>
#endif

#ifndef STDLIB_H
#define STDLIB_H
#include <stdlib.h>
#endif

#ifndef STDBOOL_H
#define STDBOOL_H
#include <stdbool.h>
#endif

// Estrutura de array dinâmica
typedef struct {
    void **buffer;
    uint8_t size;
    uint32_t length;
    uint32_t capacity;
} ARRAY;

// Instancia uma nova array dinâmica
ARRAY *new_array(uint8_t size) {
    ARRAY *arr = malloc(sizeof(ARRAY));
    arr->buffer = NULL;
    arr->size = size;
    arr->length = 0;
    arr->capacity = 0;
    return arr;
}

// Insere um item na array dinâmica
bool push(ARRAY *arr, void *item) {
    if(!arr) return false;
    if(arr->length == arr->capacity) {
        arr->capacity *= 2;
        void *buffer = malloc(arr->capacity * arr->size);
        for(uint32_t i = 0; i < arr->length; i++)
            ((uint8_t*)buffer)[i] = ((uint8_t*)arr->buffer)[i];
        free(arr->buffer);
        *arr->buffer = buffer;
    }
    for(uint32_t i = 0; i < arr->length; i++)
        ((uint8_t*)arr->buffer)[arr->length * arr->size + i] = ((uint8_t*)item)[i];
    arr->length++;
    return true;
}

// Retorna o último item da array
void *pop(ARRAY *arr) {
    if(!arr) return NULL;
    return ((void**)arr->buffer)[(arr->length - 1) *  arr->size];
}