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

// Estrutura de um nó
typedef struct {
    char key;
    void *content;
} NODE;

// Estrutura de um mapa
typedef struct {
    NODE *buffer;
    uint32_t length;
    uint32_t capacity;
} MAP;

// Instancia um novo mapa
MAP *map_new() {
    MAP *map = malloc(sizeof(MAP));
    map->buffer = malloc(sizeof(NODE));
    map->length = 0;
    map->capacity = 1;
    return map;
}

// Insere um item no mapa
bool map_push(MAP *map, char key, void *item) {
    if(!map) return false;
    for(uint32_t i = 0; i < map->length; i++) {
        if(map->buffer[i].key == key) {
            map->buffer[i].content = item;
            return true;
        }
    }
    if(map->length == map->capacity) {
        map->capacity *= 2;
        NODE *buffer = malloc(map->capacity * sizeof(NODE));
        memcpy(buffer, map->buffer, map->length * sizeof(NODE));
        free(map->buffer);
        map->buffer = buffer;
    }
    map->buffer[map->length].key = key;
    map->buffer[map->length].content = item;
    map->length++;
    return true;
}

// Retorna o item do mapa com a key especificada
void *map_get(MAP *map, char key) {
    if(!map) return NULL;
    for(uint32_t i = 0; i < map->length; i++) {
        if(map->buffer[i].key == key)
            return map->buffer[i].content;
    }
    return NULL;
}