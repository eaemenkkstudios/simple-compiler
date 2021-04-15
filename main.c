#ifndef LEXER_H
#define LEXER_H
#include "lexer/lexer.h"
#endif

#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif

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

    for(uint32_t i = 0; i < tokens->length; i++) {
        TOKEN *t = tokens->buffer[i];
        printf("%.2u, %.4li, (%u, %u)\n", t->code, t->value, t->position.line, t->position.column);
    }

    for(uint32_t i = 0; i < lexicalErrors->length; i++) {
        LEXICAL_ERROR *l = lexicalErrors->buffer[i];
        printf("%u, (%u, %u)\n", l->code, l->position.column, l->position.line);
    }
}