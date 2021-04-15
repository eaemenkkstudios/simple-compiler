#ifndef LEXER_H
#define LEXER_H
#include "lexer/lexer.h"
#endif

#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif

int main() {
    char num[1024];
    scanf("%s", num);
    printf("Is number? %s\n", is_number(num) ? "true" : "false");
    printf("Number: %i\n", get_number(num));
}