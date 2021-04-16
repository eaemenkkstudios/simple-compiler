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
    SYNTAX_ERROR_CODE_UNEXPECTED_TOKEN,
    SYNTAX_ERROR_CODE_INDEX_EXPECTED,
    SYNTAX_ERROR_CODE_POSITIVE_NUMBER_EXPECTED,
    SYNTAX_ERROR_CODE_NUMBER_EXPECTED,
    SYNTAX_ERROR_CODE_LOGICAL_OPERATOR_EXPECTED,
    SYNTAX_ERROR_CODE_MATH_OPERATOR_EXPECTED,
    SYNTAX_ERROR_CODE_OPERAND_EXPECTED,
    SYNTAX_ERROR_CODE_RESERVED_WORD_EXPECTED,
    SYNTAX_ERROR_CODE_ATTRIBUTION_EXPECTED,
    SYNTAX_ERROR_CODE_VARIABLE_EXPECTED,
    SYNTAX_ERROR_CODE_GOTO_EXPECTED,
    SYNTAX_ERROR_CODE_MULTIPLE_ENDINGS,
    SYNTAX_ERROR_CODE_END_OF_SENTENCE_EXPECTED,
} SYNTAX_ERROR_CODE;

// Erro sintático
typedef struct {
    SYNTAX_ERROR_CODE code;
    uint32_t index;
} SYNTAX_ERROR;

// Lista de erros
ARRAY *syntaxErrors = NULL;

// Instancia um erro e adiciona à array
void throw_syntax_error(SYNTAX_ERROR_CODE code, uint32_t index) {
    SYNTAX_ERROR *e = malloc(sizeof(SYNTAX_ERROR));
    e->code = code;
    e->index = index;
    push(syntaxErrors, e);
}