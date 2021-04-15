#ifndef TOKEN_H
#define TOKEN_H
#include "token.h"
#endif

#ifndef ARRAY_H
#define ARRAY_H
#include "../array.h"
#endif

// Códigos de erros
typedef enum {
    LEXICAL_ERROR_CODE_NAN,
    LEXICAL_ERROR_CODE_NAPN,
} LEXICAL_ERROR_CODE;

// Erro léxico
typedef struct {
    LEXICAL_ERROR_CODE code;
    TOKEN *token;
} LEXICAL_ERROR;

// Lista de erros
ARRAY *lexicalErrors = NULL;

// Instancia um erro e adiciona à array
void throw_lexical_error(LEXICAL_ERROR_CODE code) {
    LEXICAL_ERROR *e = malloc(sizeof(LEXICAL_ERROR));
    e->code = code;
    e->token = (TOKEN*)pop(tokens);
    push(lexicalErrors, e);
}