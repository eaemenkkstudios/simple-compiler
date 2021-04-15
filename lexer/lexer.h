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

#ifndef LEXICAL_ERROR_H
#define LEXICAL_ERROR_H
#include "error.h"
#endif

#ifndef STRING_H
#define STRING_H
#include <string.h>
#endif

// Texto do código
char *text;

// Tipo de número
typedef enum {
    NUM_TYPE_NAN,
    NUM_TYPE_INVALID,
    NUM_TYPE_NUM
} NUM_TYPE;

// Verifica se é um caractere de espaçamento
bool is_blank(char str) {
    return str == ' ';
}

// Verifica se é um caractere de nova linha
bool is_lf(char str) {
    return str == '\n';
}

// Verifica se é um caractere de fim de texto
bool is_etx(char str) {
    return str == '\x03' || str == '\0';
}

// Verifica se é um caractere terminal
bool is_ending_char(char str) {
    return is_etx(str) || is_lf(str) || is_blank(str);
}

// Verifica se é um caractere de letra
bool is_letter_char(char str) {
    return str >= 'a' && str <= 'z';
}

// Avança ao próximo caractere
void next() {
    text++;
    cursor.column++;
}

// Avança até a próxima token
void advance_token() {
    while(!is_ending_char(*text)) next();
}

// Avança espaços em branco
void advance_blank() {
    while(is_blank(*text)) next();
}

// Verifica se é uma sequência numérica válida
NUM_TYPE is_number(char *str) {
    bool isNumber = false;
    bool hasMinus = *str == '-';
    if(hasMinus) str++;
    while(!is_ending_char(*str)) {
        if(*str < '0' || *str > '9') {
            if(isNumber) return NUM_TYPE_INVALID;
            return NUM_TYPE_NAN;
        }
        else isNumber = true;
        str++;
    }
    return NUM_TYPE_NUM;
}

// Verifica se é uma sequência de letras válida
bool is_word(char *str) {
    bool isWord = false;
    while(!is_ending_char(*str)) {
        if(*str < 'a' || *str > 'z') return false;
        else isWord = true;
        str++;
    }
    return isWord;
}

// Verifica se é uma variável válida
bool is_var(char *str) {
    return is_letter_char(*str) && is_ending_char(str[1]);
}

// Executa a análise léxica no código
TOKEN *parse(char *t) {
    // Aloca arrays de tokens e erros
    tokens = new_array(sizeof(TOKEN));
    lexicalErrors = new_array(sizeof(LEXICAL_ERROR));
    text = t;
    
    // Lê token a token
    while(*text) {
        // Pula espaços em branco
        advance_blank();

        // Lê token individualmente
        uint32_t i = 0;
        while(!is_ending_char(text[i])) i++;
        char *token = malloc(sizeof(char) * (i + 1));
        for(uint32_t j = 0; j < i; j++) token[j] = text[j];
        token[i] = '\0';

        // Checa se token é um número
        NUM_TYPE num = is_number(token);
        if(num == NUM_TYPE_NUM) {
                                                push_token(TOKEN_CODE_NUM);
                                                fill_token(atoi(token));
        } else if (num == NUM_TYPE_INVALID)     throw_lexical_error(LEXICAL_ERROR_CODE_INVALID_NUMBER);
        // Checa se token é caractere especial
        else if(token[0] == '\n')               push_token(TOKEN_CODE_LF);
        else if(token[0] == '\0'
            || token[0] == '\x03')              push_token(TOKEN_CODE_ETX);
        // Checa se token é variável
        if(is_var(token)) {
                                                push_token(TOKEN_CODE_VAR);
                                                fill_token(token[0]);
        }
        // Checa se token é palavra reservada
        else if(is_word(token)) {
            if(!strcmp(token, "rem"))           push_token(TOKEN_CODE_REM);
            else if (!strcmp(token, "input"))   push_token(TOKEN_CODE_IN);
            else if (!strcmp(token, "let"))     push_token(TOKEN_CODE_LET);
            else if (!strcmp(token, "print"))   push_token(TOKEN_CODE_OUT);
            else if (!strcmp(token, "goto"))    push_token(TOKEN_CODE_GOTO);
            else if (!strcmp(token, "if"))      push_token(TOKEN_CODE_IF);
            else if (!strcmp(token, "end"))     push_token(TOKEN_CODE_END);
            else                                throw_lexical_error(LEXICAL_ERROR_CODE_INVALID_WORD);
        }
        // Checa se token é operador
        else if(!strcmp(token, "="))            push_token(TOKEN_CODE_SET);
        else if(!strcmp(token, "+"))            push_token(TOKEN_CODE_ADD);
        else if(!strcmp(token, "-"))            push_token(TOKEN_CODE_SUB);
        else if(!strcmp(token, "*"))            push_token(TOKEN_CODE_SET);
        else if(!strcmp(token, "/"))            push_token(TOKEN_CODE_DIV);
        else if(!strcmp(token, "%"))            push_token(TOKEN_CODE_MOD);
        else if(!strcmp(token, "=="))           push_token(TOKEN_CODE_EQU);
        else if(!strcmp(token, "!="))           push_token(TOKEN_CODE_NEQU);
        else if(!strcmp(token, ">"))            push_token(TOKEN_CODE_GT);
        else if(!strcmp(token, "<"))            push_token(TOKEN_CODE_LT);
        else if(!strcmp(token, ">="))           push_token(TOKEN_CODE_GTE);
        else if(!strcmp(token, "<="))           push_token(TOKEN_CODE_LTE);
        else                                    throw_lexical_error(LEXICAL_ERROR_CODE_UNKNOWN);
        // Desaloca token
        free(token);

        // Avança para a próxima token
        advance_token();
    }
}