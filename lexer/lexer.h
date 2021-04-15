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

#ifndef ARRAY_H
#define ARRAY_H
#include "../array.h"
#endif

#ifndef TOKEN_H
#define TOKEN_H
#include "token.h"
#endif

#ifndef ERROR_H
#define ERROR_H
#include "error.h"
#endif

// Texto do código
char *text;

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

// Avança até a próxima token
void advance() {
    while(is_blank(*text)) {
        text++;
        cursor.column++;
    }
    while(!is_ending_char(*text)) {
        text++;
        cursor.column++;
    }
}

// Verifica se é uma sequência numérica válida
bool is_number(char *str) {
    bool isNumber = false;
    bool hasMinus = *str == '-';
    if(hasMinus) str++;
    while(!is_ending_char(*str)) {
        if(*str < '0' || *str > '9') return false;
        else isNumber = true;
        str++;
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

// Executa a análise léxica no código
TOKEN *parse(char *t) {
    tokens = new_array(sizeof(TOKEN));
    lexicalErrors = new_array(sizeof(LEXICAL_ERROR));

    text = t;
    bool newLine = true;
    
    while(*text) {
        while(is_blank(*text)) advance();
        if(newLine) {
            uint32_t num = get_number(text);
            add_token(TOKEN_CODE_NUM, num);
            if(is_number(text, true)) {
                if(num < 0) throw_lexical_error(LEXICAL_ERROR_CODE_NAN);
            } else throw_lexical_error(LEXICAL_ERROR_CODE_NAPN);
            newLine = false;
            advance();
        }
        text++;
    }
}