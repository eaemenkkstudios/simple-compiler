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

#ifndef STRING_H
#define STRING_H
#include <string.h>
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
        memcpy(buffer, arr->buffer, arr->length * arr->size);
        free(arr->buffer);
        *arr->buffer = buffer;
    }
    memcpy(arr->buffer + arr->length * arr->size, item, arr->size);
    arr->length++;
    return true;
}

// Retorna o último item da array
void *pop(ARRAY *arr) {
    if(!arr) return NULL;
    return ((void**)arr->buffer)[(arr->length - 1) *  arr->size];
}