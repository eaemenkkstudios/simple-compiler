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
    uint32_t length;
    uint32_t capacity;
} ARRAY;

// Instancia uma nova array dinâmica
ARRAY *new_array() {
    ARRAY *arr = malloc(sizeof(ARRAY));
    arr->buffer = malloc(sizeof(void*));
    arr->length = 0;
    arr->capacity = 1;
    return arr;
}

// Insere um item na array dinâmica
bool push(ARRAY *arr, void *item) {
    if(!arr) return false;
    if(arr->length == arr->capacity) {
        arr->capacity *= 2;
        void **buffer = malloc(arr->capacity * sizeof(void*));
        memcpy(buffer, arr->buffer, arr->length * sizeof(void*));
        free(arr->buffer);
        arr->buffer = buffer;
    }
    arr->buffer[arr->length++] = item;
    return true;
}

// Retorna o último item da array
void *pop(ARRAY *arr) {
    if(!arr || !arr->length) return NULL;
    return (arr->buffer)[arr->length - 1];
}

// Retorna o item index da array
void *get(ARRAY *arr, uint32_t index) {
    if(!arr || index >= arr->length) return NULL;
    return (arr->buffer)[index];
}