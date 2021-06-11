#ifndef STDINT_H
#define STDINT_H
#include <stdint.h>
#endif

#ifndef ARRAY_H
#define ARRAY_H
#include "../utils/array.h"
#endif

#ifndef MAP_H
#define MAP_H
#include "../utils/map.h"
#endif

#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif

#ifndef TOKEN_H
#define TOKEN_H
#include "../lexer/token.h"
#endif

#ifndef STRING_H
#define STRING_H
#include <string.h>
#endif

#ifndef STDLIB_H
#define STDLIB_H
#include <stdlib.h>
#endif

typedef struct {
    uint8_t var;
    char *value;
} VAR;

// 0 a
// 1 5
// 2 2
// 3 a + b
// 4 a + c
// 5 a * b

// input a       //  a = 0      load a, r0
// let b = 5     //  b = 1      load b, r1
// let c = 2     //  c = 2      load c, r2
// let b = a + b //  b = 3      add r1, r0
// let a = a + c //  a = 4      add r0, r2
// let d = a * b //  d = 5      mul r0, r1
                 //             copy r3, r0
// let e = a * b //  e = 5      mul r1, r0
// let a = d     //  a = 5

// Token atual
TOKEN objToken;
// Pointeiro de instruções
uint32_t objPointer = 0;
// Array de operações e mapa de variáveis
ARRAY *operations;
MAP *variables;

// Avança uma instrução
void objAdvance() {
    objToken = *((TOKEN*)array_get(tokens, ++objPointer));
}

// Realiza análise sintática
void parse_object() {
    // Inicializa array de operações e variáveis
    operations = array_new();
    variables = map_new();
    // Inicializa objToken
    objToken = *((TOKEN*)array_get(tokens, 0));

    // Variável de atribuição
    char assignmentVar = 0;
    // String auxiliar de variável
    char auxiliaryVar[2] = {0};
    // String auxiliar de número
    char auxiliaryNum[3] = {0};
    // String de expressão
    char *expression;

    while(true) {
        switch(objToken.code) {
            case TOKEN_CODE_LF:
                // Caso seja sucessor de atribuição
                if(assignmentVar) {
                    uint8_t *index = NULL;
                    // Verifica se expressão já ocorreu
                    for(uint8_t i = 0; i < operations->length; i++)
                        if(!strcmp(*((char**)array_get(operations, i)), expression)) {
                            index = (uint8_t *)malloc(sizeof(uint8_t));
                            *index = i;
                            break;
                        }
                    // Ou registra a expressão nova
                    if(!index) {
                        index = (uint8_t *)malloc(sizeof(uint8_t));
                        *index = operations->length;
                        array_push(operations, expression);
                    }
                    // E atribúi a expressão à variável
                    map_push(variables, assignmentVar, index);
                    assignmentVar = 0;
                    expression = NULL;
                }
                objAdvance();
                continue;
            case TOKEN_CODE_SET:
                // Obtém a variável e aloca uma nova expressão
                assignmentVar = ((TOKEN *)array_get(tokens, objPointer - 1))->value;
                expression = calloc(1, sizeof(char));
                continue;
            case TOKEN_CODE_ADD:
                // Insere operador na expressão
                strcat(expression, "+");
                continue;
            case TOKEN_CODE_SUB:
                // Insere operador na expressão
                strcat(expression, "-");
                continue;
            case TOKEN_CODE_MUL:
                // Insere operador na expressão
                strcat(expression, "*");
                continue;
            case TOKEN_CODE_DIV:
                // Insere operador na expressão
                strcat(expression, "/");
                continue;
            case TOKEN_CODE_MOD:
                // Insere operador na expressão
                strcat(expression, "%");
                continue;
            case TOKEN_CODE_VAR:
                // Verifica se é sucessor de input
                if(((TOKEN *)array_get(tokens, objPointer - 1))->code == TOKEN_CODE_IN) {
                    auxiliaryVar[0] = objToken.value;
                    strcat(expression, auxiliaryVar);

                    uint8_t *index = malloc(sizeof(uint8_t));
                    *index = operations->length;
                    map_push(variables, objToken.value, index);
                    
                    array_push(operations, expression);
                    expression = 0;
                } else
                // Ou se é parte de atribuição
                if(assignmentVar) {
                    auxiliaryVar[0] = objToken.value;
                    strcat(expression, auxiliaryVar);
                }
                continue;
            case TOKEN_CODE_NUM:
                if(assignmentVar) {
                    snprintf(auxiliaryNum, 3, "%d", objToken.value);
                    strcat(expression, auxiliaryNum);
                }
                continue;
        }
        if(objToken.code == TOKEN_CODE_END) break;
    }
}