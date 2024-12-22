#include "parser.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char expr_cursor = 0;

struct Expression* create_expr() {
    static struct Expression exprs[64] = {0}; // 64 expressions MAXIMUM at ALL TIMES
    struct Expression* ret = &exprs[expr_cursor++];
    if (expr_cursor >= 64) {
        fprintf(stderr, "=== HEAP FAILURE ===\n");
        exit(EXIT_FAILURE);
    }
    memset(ret, 0, sizeof(struct Expression));
    return ret;
}

void reset_expr_heap() {
    expr_cursor = 0;
}

// void skip_whitespace(const char* str) {
//     while (isspace(**str)) {
//         (*str)++;
//     }
// }

float read_number(const char** str) {
    float ret = 0.f;

    bool negative = false;
    if (**str == '-') {
        negative = true;
        ++*str;
    }

    for (; isdigit(**str); ++*str) {
        ret = ret * 10.f + (**str - '0');
    }

    if (**str == '.') {
        ++*str; // Skip decimal point
        for (unsigned char current_decimal_digit = 1; isdigit(**str); ++*str, ++current_decimal_digit) {
            float addend = **str - '0';
            for (unsigned char i = 0; i < current_decimal_digit; ++i) {
                addend /= 10.f;
            }
            ret += addend;
        }
    }

    return negative ? -ret : ret;
}

struct Expression* parse(const char** str) {
    float num;
    if (isdigit(**str) || **str == '-') {
        // first, get the number
        num = read_number(str);
        struct Expression* lhs = create_expr();
        lhs->type = EXPRESSION_NONE;
        lhs->number = num;
        // after the number there might be an operator, closed parenthesis, or EOI
        enum ExpressionType operator;
        switch (**str) {
            case '+':
                operator = EXPRESSION_ADD;
                break;
            case '-':
                operator = EXPRESSION_SUBTRACT;
                break;
            case 0:
            case ')': {
                // if there's a closed parenthesis, we can just return a root
                return lhs;
            }
        }

        // get the rhs expression
        ++*str;
        struct Expression* rhs = parse(str);
        struct Expression* final = create_expr();
        final->lhs = lhs;
        final->rhs = rhs;
        final->type = operator;
        return final;
    } else if (**str == '(') {
        // get the lhs
        ++*str;
        struct Expression* lhs = parse(str);

    }

    return NULL;
}
