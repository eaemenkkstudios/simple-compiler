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
    tokens = new_array(sizeof(TOKEN));
    lexicalErrors = new_array(sizeof(LEXICAL_ERROR));
    // Buffer de leitura
    char buffer[1024];
    
    // Carrega arquivo
    FILE *f = fopen(argv[1], "r");

    // Realiza análise léxica
    while(fgets(buffer, 1024, f)) parse(buffer);

    for(uint32_t i = 0; i < tokens->length; i++) {
        TOKEN t = *((TOKEN*)(tokens->buffer + i));
        printf("%u, %li, (%u, %u)\n", t.code, t.value, t.position.line, t.position.column);
    }
}