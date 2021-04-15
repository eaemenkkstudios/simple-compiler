#ifndef STDLIB_H
#define STDLIB_H
#include <stdlib.h>
#endif

#ifndef STDINT_H
#define STDINT_H
#include <stdint.h>
#endif

#ifndef STDBOOL_H
#define STDBOOL_H
#include <stdbool.h>
#endif

#ifndef MATH_H
#define MATH_H
#include <math.h>
#endif

#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif

typedef struct {
    uint32_t line;
    uint32_t column;
} POINT;

typedef struct {
    uint8_t code;
    void    *value;
    POINT   position;
} TOKEN;

typedef enum {
    LEXICAL_ERROR_NO_ERROR,
    LEXICAL_ERROR_NUMBER_EXPECTED,
    LEXICAL_ERROR_POSITIVE_NUMBER_EXPECTED,
} LEXICAL_ERROR;

char *text;
POINT current = {1, 1};

void throw_error(LEXICAL_ERROR error) {
    printf("ERROR: %d\n", error);
}

// Verifica se é um caractere de espaçamento
bool is_blank(const char str) {
    return str == ' ';
}


// Verifica se é um caractere de nova linha
bool is_lf(const char str) {
    return str == '\n';
}

// Verifica se é um caractere de fim de texto
bool is_etx(const char str) {
    return str == '\x03' || str == '\0';
}

// Verifica se é um caractere terminal
bool is_ending_char(const char str) {
    return is_etx(str) || is_lf(str) || is_blank(str);
}

void advance() {
    while(is_blank(*text)) {
        text++;
        current.column++;
    }
    while(!is_ending_char(*text)) {
        text++;
        current.column++;
    }
}

// Verifica se é uma sequência numérica válida
bool is_number(char *str, bool positiveOnly) {
    bool isNumber = false;
    bool hasMinus = *str == '-';
    if(hasMinus) str++;
    while(!is_ending_char(*str)) {
        if(*str < '0' || *str > '9') return false;
        else isNumber = true;
        str++;
    }
    if(isNumber && hasMinus && positiveOnly) {
        throw_error(LEXICAL_ERROR_POSITIVE_NUMBER_EXPECTED);
        return false;
    }
    return isNumber;
}

// Obtém um número
uint32_t get_number(char *str) {
    int32_t number = 0;
    uint8_t length = 0;

    bool hasMinus = *str == '-';
    if(hasMinus) str++;

    while(!is_ending_char(str[length])) length++;
    for(uint8_t i = 0; i < length; i++)
        number += (uint32_t)pow(10, length - i - 1) * (str[i] - '0');

    return number * (hasMinus ? -1 : 1);
}

TOKEN *parse(char *t) {
    text = t;
    bool newLine = true;
    while(*text) {
        while(is_blank(*text)) advance();
        if(newLine) {
            if(is_number(text, true)) {
                
            } else {
               throw_error(LEXICAL_ERROR_NUMBER_EXPECTED);
            }
            newLine = false;
            advance();
        }
        text++;
    }
}