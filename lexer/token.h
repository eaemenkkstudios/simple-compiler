#ifndef STDINT_H
#define STDINT_H
#include <stdint.h>
#endif

#ifndef ARRAY_H
#define ARRAY_H
#include "../utils/array.h"
#endif

#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif

#ifndef TOKEN_H
#define TOKEN_H
#include "token.h"
#endif

// Ponto do texto
typedef struct {
    uint32_t line;
    uint32_t column;
} POINT;

// Classe de token
typedef enum {
    TOKEN_CODE_UNKN = 0,
    TOKEN_CODE_ETX  = 3,
    TOKEN_CODE_LF   = 10,
    TOKEN_CODE_SET,
    TOKEN_CODE_ADD  = 21,
    TOKEN_CODE_SUB,
    TOKEN_CODE_MUL,
    TOKEN_CODE_DIV,
    TOKEN_CODE_MOD,
    TOKEN_CODE_EQU  = 31,
    TOKEN_CODE_NEQU,
    TOKEN_CODE_GT,
    TOKEN_CODE_LT,
    TOKEN_CODE_GTE,
    TOKEN_CODE_LTE,
    TOKEN_CODE_VAR  = 41,
    TOKEN_CODE_NUM  = 51,
    TOKEN_CODE_REM  = 61,
    TOKEN_CODE_IN,
    TOKEN_CODE_LET,
    TOKEN_CODE_OUT,
    TOKEN_CODE_GOTO,
    TOKEN_CODE_IF,
    TOKEN_CODE_END
} TOKEN_CODE;

// Token
typedef struct {
    TOKEN_CODE  code;
    int16_t     value;
    POINT       position;
} TOKEN;

// Posição do cursor
POINT cursor = {1, 1};
// Array de tokens
ARRAY *tokens = NULL;

// Instancia uma token e adiciona à array
bool push_token(TOKEN_CODE code, int16_t value) {
    if(code == TOKEN_CODE_UNKN) return false;
    TOKEN *t = malloc(sizeof(TOKEN));
    t->code = code;
    t->position = cursor;
    t->value = value;
    return array_push(tokens, t);
}