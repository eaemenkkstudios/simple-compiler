#ifndef LEXER_H
#define LEXER_H
#include "lexer/lexer.h"
#endif

#ifndef SYNTAX_H
#define SYNTAX_H
#include "syntax/syntax.h"
#endif

#ifndef OBJECT_H
#define OBJECT_H
#include "object/object.h"
#endif

#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif

#ifndef STDLIB_H
#define STDLIB_H
#include <stdlib.h>
#endif

#ifndef STRING_H
#define STRING_H
#include <string.h>
#endif

// Descreve erros léxicos
char *lexical_error_to_string(LEXICAL_ERROR_CODE code) {
    switch(code) {
        case LEXICAL_ERROR_CODE_INVALID_NUMBER:
            return "Número inválido";
        case LEXICAL_ERROR_CODE_INVALID_WORD:
            return "Palavra reservada inválida.";
        case LEXICAL_ERROR_CODE_UNKNOWN:
            return "Token desconhecida.";
        default:
            return "Erro de análise léxica desconhecido.";
    }
}
// Descreve erros sintáticos
char *syntax_error_to_string(SYNTAX_ERROR_CODE code) {
    switch(code) {
        case SYNTAX_ERROR_CODE_UNEXPECTED_TOKEN:
            return "Token inesperado.";
        case SYNTAX_ERROR_CODE_INDEX_EXPECTED:
            return "Índice esperado.";
        case SYNTAX_ERROR_CODE_POSITIVE_NUMBER_EXPECTED:
            return "Número positivo esperado.";
        case SYNTAX_ERROR_CODE_NUMBER_EXPECTED:
            return "Número esperado.";
        case SYNTAX_ERROR_CODE_LOGICAL_OPERATOR_EXPECTED:
            return "Operador lógico esperado.";
        case SYNTAX_ERROR_CODE_MATH_OPERATOR_EXPECTED:
            return "Operador matemático esperado.";
        case SYNTAX_ERROR_CODE_OPERAND_EXPECTED:
            return "Operando esperado.";
        case SYNTAX_ERROR_CODE_RESERVED_WORD_EXPECTED:
            return "Palavra reservada esperada.";
        case SYNTAX_ERROR_CODE_ATTRIBUTION_EXPECTED:
            return "Atribuição esperada.";
        case SYNTAX_ERROR_CODE_VARIABLE_EXPECTED:
            return "Variável esperada.";
        case SYNTAX_ERROR_CODE_GOTO_EXPECTED:
            return "Goto esperado.";
        case SYNTAX_ERROR_CODE_MULTIPLE_ENDINGS:
            return "Múltiplas tokens END detectadas.";
        case SYNTAX_ERROR_CODE_END_OF_SENTENCE_EXPECTED:
            return "Final de sentença esperado.";
        case SYNTAX_ERROR_CODE_CRESCENT_INDEX_EXPECTED:
            return "Ordem crescente de índices esperada.";
        default:
            return "Erro de análise sintática desconhecido.";
    }
}

// Caminho do arquivo
char filePath[FILENAME_MAX];

// Define o caminho do arquivo
void set_file_path(char *file) {
    filePath[0] = '\0';
    strcpy(filePath, "./");
    strcat(filePath, file);
}

int main(int argc, char **argv) {
    // Aloca arrays de tokens e erros
    tokens = array_new();
    lexicalErrors = array_new();
    syntaxErrors = array_new();

    // Normaliza o caminho do arquivo
    set_file_path(argv[1]);

    // Buffer de leitura
    char buffer[1024];
    
    // Carrega arquivo
    FILE *f = fopen(filePath, "r");
    if(!f) {
        printf("Arquivo não encontrado: \x1b[1m%s\x1b[0m\n", filePath);
        return 1;
    }

    // Realiza análise léxica
    while(fgets(buffer, 1024, f)) parse(buffer);

    // Mostrar tokens de análise léxica
    for(uint32_t i = 0; i < tokens->length; i++) {
        TOKEN *t = array_get(tokens, i);
        printf("%.2u, %.4i, (%u, %u)\n", t->code, t->value, t->position.line, t->position.column);
    }

    // Realiza análise sintática
    parse_syntax();

    // Mostrar erros de análise léxica
    for(uint32_t i = 0; i < lexicalErrors->length; i++) {
        LEXICAL_ERROR *l = array_get(lexicalErrors, i);
        printf("\x1b[1m%s:%u:%u: \x1b[31merro (léxico):\x1b[0m %s\n",
            filePath,
            l->position.line,
            l->position.column,
            lexical_error_to_string(l->code));
    }

    // Mostrar erros de análise sintática
    for(uint32_t i = 0; i < syntaxErrors->length; i++) {
        SYNTAX_ERROR *s = array_get(syntaxErrors, i);
        printf("\x1b[1m%s:%u:%u: \x1b[31merro (sintático):\x1b[0m %s\n",
            filePath,
            ((TOKEN*)array_get(tokens, s->index))->position.line,
            ((TOKEN*)array_get(tokens, s->index))->position.column,
            syntax_error_to_string(s->code));
    }

    // if(lexicalErrors->length > 0 || syntaxErrors->length > 0)
    //     return 1;
    
    // parse_object();

    // for(uint32_t i = 0; i < operations->length; i++)
    //     printf("%s\n", ((char **)operations->buffer)[i]);
        
    return 0;
}