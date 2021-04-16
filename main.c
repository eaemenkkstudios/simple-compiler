#ifndef LEXER_H
#define LEXER_H
#include "lexer/lexer.h"
#endif

#ifndef SYNTAX_H
#define SYNTAX_H
#include "syntax/syntax.h"
#endif

#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif

// Descreve erros léxicos
char *lexical_error_to_string(LEXICAL_ERROR_CODE code) {
    switch(code) {
        case LEXICAL_ERROR_CODE_INVALID_NUMBER:
            return "Numero invalido";
        case LEXICAL_ERROR_CODE_INVALID_WORD:
            return "Palavra reservada invalida.";
        case LEXICAL_ERROR_CODE_UNKNOWN:
            return "Token desconhecida.";
        default:
            return "Erro de analise lexica desconhecido.";
    }
}
// Descreve erros sintáticos
char *syntax_error_to_string(SYNTAX_ERROR_CODE code) {
    switch(code) {
        case SYNTAX_ERROR_CODE_UNEXPECTED_TOKEN:
            return "Token inesperado.";
        case SYNTAX_ERROR_CODE_INDEX_EXPECTED:
            return "Indice esperado.";
        case SYNTAX_ERROR_CODE_POSITIVE_NUMBER_EXPECTED:
            return "Numero positivo esperado.";
        case SYNTAX_ERROR_CODE_NUMBER_EXPECTED:
            return "Numero esperado.";
        case SYNTAX_ERROR_CODE_LOGICAL_OPERATOR_EXPECTED:
            return "Operador logico esperado.";
        case SYNTAX_ERROR_CODE_MATH_OPERATOR_EXPECTED:
            return "Operador matematico esperado.";
        case SYNTAX_ERROR_CODE_OPERAND_EXPECTED:
            return "Operando esperado.";
        case SYNTAX_ERROR_CODE_RESERVED_WORD_EXPECTED:
            return "Palavra reservada esperada.";
        case SYNTAX_ERROR_CODE_ATTRIBUTION_EXPECTED:
            return "Atribuicao esperada.";
        case SYNTAX_ERROR_CODE_VARIABLE_EXPECTED:
            return "Variavel esperada.";
        case SYNTAX_ERROR_CODE_GOTO_EXPECTED:
            return "Goto esperado.";
        case SYNTAX_ERROR_CODE_MULTIPLE_ENDINGS:
            return "Multiplas tokens END detectadas.";
        case SYNTAX_ERROR_CODE_END_OF_SENTENCE_EXPECTED:
            return "Final de sentenca esperado.";
        default:
            return "Erro de analise sintatica desconhecido.";
    }
}

int main(int argc, char **argv) {
    // Aloca arrays de tokens e erros
    tokens = new_array();
    lexicalErrors = new_array();
    // Buffer de leitura
    char buffer[1024];
    
    // Carrega arquivo
    FILE *f = fopen(argv[1], "r");

    // Realiza análise léxica
    while(fgets(buffer, 1024, f)) parse(buffer);

    // Mostrar tokens de análise léxica
    // for(uint32_t i = 0; i < tokens->length; i++) {
    //     TOKEN *t = get(tokens, i);
    //     printf("%.2u, %.4li, (%u, %u)\n", t->code, t->value, t->position.line, t->position.column);
    // }

    // Mostrar erros de análise léxica
    for(uint32_t i = 0; i < lexicalErrors->length; i++) {
        LEXICAL_ERROR *l = get(lexicalErrors, i);
        printf("Erro (Lexico): %s Linha: %u, coluna: %u.\n",
            lexical_error_to_string(l->code),
            l->position.column,
            l->position.line);
    }

    // Mostrar erros de análise sintática
    for(uint32_t i = 0; i < syntaxErrors->length; i++) {
        SYNTAX_ERROR *s = get(syntaxErrors, i);
        printf("Erro (Lexico): %s Linha: %u, coluna: %u.\n",
            syntax_error_to_string(s->code),
            ((TOKEN*)get(tokens, s->index))->position.column,
            ((TOKEN*)get(tokens, s->index))->position.line);
    }
}