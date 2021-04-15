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

#ifndef STRING_H
#define STRING_H
#include <string.h>
#endif

// Texto do código
char *text;

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
void advance(bool hardAdvance) {
    if(hardAdvance) while(!is_ending_char(*text)) text++;
    while(is_blank(*text)) next();
}

// Verifica se é uma sequência numérica válida
bool is_number(char *str) {
    bool isNumber = false;
    bool hasMinus = *str == '-';
    if(hasMinus) str++;
    while(!is_ending_char(*str)) {
        if(*str < '0' || *str > '9') {
            if(isNumber) throw_lexical_error(LEXICAL_ERROR_CODE_NAN);
            return false;
        }
        else isNumber = true;
        str++;
    }
    return isNumber;
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

// Obtém código da token
TOKEN_CODE get_code(char *str) {
    uint32_t i = 0;
    while(!is_ending_char(str[i])) i++;
    char *token = malloc(sizeof(char) * (i + 1));
    for(uint32_t j = 0; j < i; j++) token[j] = str[j];
    token[i] = '\0';

    TOKEN_CODE                          code = TOKEN_CODE_UNKN;
    if(is_var(str))                     code = TOKEN_CODE_VAR;
    else if(is_word(str)) {
        if(!strcmp(str, "rem"))         code = TOKEN_CODE_REM;
        else if (!strcmp(str, "input")) code = TOKEN_CODE_IN;
        else if (!strcmp(str, "let"))   code = TOKEN_CODE_LET;
        else if (!strcmp(str, "print")) code = TOKEN_CODE_OUT;
        else if (!strcmp(str, "goto"))  code = TOKEN_CODE_GOTO;
        else if (!strcmp(str, "if"))    code = TOKEN_CODE_IF;
        else if (!strcmp(str, "end"))   code = TOKEN_CODE_END;
        else                            throw_lexical_error(LEXICAL_ERROR_CODE_NAW);
    } else if(is_number(str))           code = TOKEN_CODE_NUM;
    else if(!strcmp(str, "="))          code = TOKEN_CODE_SET;
    else if(!strcmp(str, "+"))          code = TOKEN_CODE_ADD;
    else if(!strcmp(str, "-"))          code = TOKEN_CODE_SUB;
    else if(!strcmp(str, "*"))          code = TOKEN_CODE_SET;
    else if(!strcmp(str, "/"))          code = TOKEN_CODE_DIV;
    else if(!strcmp(str, "%"))          code = TOKEN_CODE_MOD;
    else if(!strcmp(str, "=="))         code = TOKEN_CODE_EQU;
    else if(!strcmp(str, "!="))         code = TOKEN_CODE_NEQU;
    else if(!strcmp(str, ">"))          code = TOKEN_CODE_GT;
    else if(!strcmp(str, "<"))          code = TOKEN_CODE_LT;
    else if(!strcmp(str, ">="))         code = TOKEN_CODE_GTE;
    else if(!strcmp(str, "<="))         code = TOKEN_CODE_LTE;
    free(token);
    return code;
}

// Valida o índice de linha
void check_index(int64_t *lastLine) {
    TOKEN_CODE code;
    push_token(code = get_code(text));
    if(code == TOKEN_CODE_NUM) {
        int64_t val = atoi(text);
        fill_token(val);
        if(val < 0) throw_lexical_error(LEXICAL_ERROR_CODE_NAPN);
        else if(val == lastLine) throw_lexical_error(LEXICAL_ERROR_CODE_REPEATED_INDEX);
        else if(val < lastLine) throw_lexical_error(LEXICAL_ERROR_CODE_SMALLER_INDEX);
    } else throw_lexical_error(LEXICAL_ERROR_CODE_NO_INDEX);
}

// Executa a análise léxica no código
TOKEN *parse(char *t) {
    tokens = new_array(sizeof(TOKEN));
    lexicalErrors = new_array(sizeof(LEXICAL_ERROR));
    text = t;
    int64_t lastLine = -1;
    while(*text) {
        advance(false);
        check_index(&lastLine);
        advance(true);
    }
}