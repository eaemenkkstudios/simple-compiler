#ifndef TOKEN_H
#define TOKEN_H
#include "../lexer/token.h"
#endif

#ifndef ARRAY_H
#define ARRAY_H
#include "../utils/array.h"
#endif

#ifndef STDBOOL_H
#define STDBOOL_H
#include <stdbool.h>
#endif

#ifndef SYNTAX_ERROR_H
#define SYNTAX_ERROR_H
#include "error.h"
#endif

// Pointeiro de instruções
uint32_t syntaxPointer = 0;
// END encontrado
bool endFound = false;
// Último índice de linha
int64_t lastLine = 0;
// Primeiro índice
bool firstIndex = true;
// Token atual
TOKEN syntaxToken;

// Avança uma instrução
void syntaxAdvance() {
    syntaxToken = *((TOKEN*)array_get(tokens, ++syntaxPointer));
}

// Valida um número
bool validate_variable(bool crucial) {
    if(syntaxToken.code == TOKEN_CODE_VAR) {
        syntaxAdvance();
        return true;
    }
    if(crucial) throw_syntax_error(SYNTAX_ERROR_CODE_VARIABLE_EXPECTED, syntaxPointer);
    return false;
}

// Valida um operador aritmético
bool validate_math_operator(bool crucial) {
    switch(syntaxToken.code) {
        case TOKEN_CODE_ADD:
        case TOKEN_CODE_SUB:
        case TOKEN_CODE_MUL:
        case TOKEN_CODE_DIV:
        case TOKEN_CODE_MOD:
            syntaxAdvance();
            return true;
        default:
            if(crucial) throw_syntax_error(SYNTAX_ERROR_CODE_MATH_OPERATOR_EXPECTED, syntaxPointer);
            return false;
    }
}

// Valida um operador lógico
bool validate_logic_operator(bool crucial) {
    switch(syntaxToken.code) {
        case TOKEN_CODE_EQU:
        case TOKEN_CODE_NEQU:
        case TOKEN_CODE_GT:
        case TOKEN_CODE_LT:
        case TOKEN_CODE_GTE:
        case TOKEN_CODE_LTE:
            syntaxAdvance();
            return true;
        default:
            if(crucial) throw_syntax_error(SYNTAX_ERROR_CODE_LOGICAL_OPERATOR_EXPECTED, syntaxPointer);
            return false;
    }
}

// Valida uma atribuição
bool validate_attribuition(bool crucial) {
    if(syntaxToken.code == TOKEN_CODE_SET) {
        syntaxAdvance();
        return true;
    }
    if(crucial) throw_syntax_error(SYNTAX_ERROR_CODE_ATTRIBUTION_EXPECTED, syntaxPointer);
    return false;
}

// Valida um número
bool validate_number(bool crucial) {
    if(syntaxToken.code == TOKEN_CODE_NUM) {
        syntaxAdvance();
        return true;
    }
    if(crucial) throw_syntax_error(SYNTAX_ERROR_CODE_NUMBER_EXPECTED, syntaxPointer);
    return false;
}

// Valida um operando
bool validate_operand(bool crucial) {
    if(validate_number(false) || validate_variable(false))
        return true;
    if(crucial) throw_syntax_error(SYNTAX_ERROR_CODE_OPERAND_EXPECTED, syntaxPointer);
    return false;
}

// Valida um índice
bool validate_index(bool crucial, bool ordered) {
    TOKEN t = syntaxToken;
    if(validate_number(false)) {
        if(t.value < 0)
            throw_syntax_error(SYNTAX_ERROR_CODE_POSITIVE_NUMBER_EXPECTED, syntaxPointer - 1);
        if(ordered) {
            if(crucial && !firstIndex && t.value <= lastLine)
                throw_syntax_error(SYNTAX_ERROR_CODE_CRESCENT_INDEX_EXPECTED, syntaxPointer - 1);
            lastLine = t.value;
            firstIndex = false;
        }

        return true;
    }
    if(crucial) throw_syntax_error(SYNTAX_ERROR_CODE_INDEX_EXPECTED, syntaxPointer);
    return false;
}

// Valida um goto
bool validate_goto(bool crucial) {
    if(syntaxToken.code == TOKEN_CODE_GOTO) {
        syntaxAdvance();
        return validate_index(crucial, false);
    }
    if(crucial) throw_syntax_error(SYNTAX_ERROR_CODE_GOTO_EXPECTED, syntaxPointer);
    return false;
}

// Valida uma palavra reservada
bool validate_reserved_word(bool crucial) {
    switch(syntaxToken.code) {
        case TOKEN_CODE_END:
            if(!endFound) endFound = true;
            else if(crucial) throw_syntax_error(SYNTAX_ERROR_CODE_MULTIPLE_ENDINGS, syntaxPointer);
        case TOKEN_CODE_REM:
            syntaxAdvance();
            return true;
        case TOKEN_CODE_IN:
        case TOKEN_CODE_OUT:
            syntaxAdvance();
            return validate_variable(crucial);
        case TOKEN_CODE_GOTO:
            return validate_goto(crucial);
        case TOKEN_CODE_IF:
            syntaxAdvance();
            return validate_operand(crucial)
                && validate_logic_operator(crucial)
                && validate_operand(crucial)
                && validate_goto(crucial);
        case TOKEN_CODE_LET:
            syntaxAdvance();
            if(validate_variable(crucial)
                && validate_attribuition(crucial)
                && validate_operand(crucial)) {
                    if(validate_math_operator(false))
                        return validate_operand(crucial);
                    return true;
                }
            return false;
        default:
            throw_syntax_error(SYNTAX_ERROR_CODE_RESERVED_WORD_EXPECTED, syntaxPointer);
            return false;
    }
}

// Valida final de sentença
bool is_end_of_sentence() {
    return syntaxToken.code == TOKEN_CODE_ETX || syntaxToken.code == TOKEN_CODE_LF;
}

// Realiza análise sintática
void parse_syntax() {
    // Inicializa syntaxToken
    syntaxToken = *((TOKEN*)array_get(tokens, syntaxPointer));
    while(true) {
        // Verifica índice de linha
        validate_index(true, true);
        validate_reserved_word(true);
        if(!is_end_of_sentence()) {
            throw_syntax_error(SYNTAX_ERROR_CODE_END_OF_SENTENCE_EXPECTED, syntaxPointer);
            while(!is_end_of_sentence()) syntaxAdvance();
        }
        if(syntaxToken.code == TOKEN_CODE_LF) {
            syntaxAdvance();
            continue;
        }
        else if(syntaxToken.code == TOKEN_CODE_ETX) break;
    }
}