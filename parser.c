#include "parser.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "console.h"

unsigned char expr_cursor = 0;

struct Expression* create_expr(void) {
    static struct Expression exprs[64] = {0}; // 64 expressions MAXIMUM at ALL TIMES
    struct Expression* ret = &exprs[expr_cursor++];
    if (expr_cursor >= 64) {
        puts("=== HEAP FAILURE ===");
        exit(EXIT_FAILURE);
    }
    memset(ret, 0, sizeof(struct Expression));
    return ret;
}

void reset_expr_heap(void) {
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

unsigned char operator_value(enum ExpressionType operator) {
    // ranking: ADD/SUBTRACT, DIVIDE/MULTIPLY, EXPONENTIATE, PARENTHESIS
    switch (operator) {
        case EXPRESSION_NONE:
            return 0;
        case EXPRESSION_EXPONENTIATE:
            return 3;
        case EXPRESSION_MULTIPLY:
        case EXPRESSION_DIVIDE:
            return 2;
        case EXPRESSION_ADD:
        case EXPRESSION_SUBTRACT:
            return 1;
        default:
            return 0;
    }
}

// Returns a bool stating whether the parsing process was interrupted
bool parse(struct Expression** result, const char** str, enum ExpressionType last_operator) {
    float num;

    struct Expression* lhs;
    if (isdigit(**str) || **str == '-') {
        // first, get the number
        lhs = create_expr();
        num = read_number(str);
        lhs->type = EXPRESSION_NONE;
        lhs->number = num;
    } else if (**str == '(') {
        // get the lhs
        ++*str;
        parse(&lhs, str, EXPRESSION_NONE);
        // after that, there should be a closed parenthesis
        if (**str != ')') {
            puts("==PARSING EXCEPTION==");
        }

        // need to get whatever is after the closed parenthesis
        ++*str;
    }

    while (true) {
        // after the number/expression there might be an operator, closed parenthesis, or EOI
        enum ExpressionType operator;
        switch (**str) {
            case '+':
                operator = EXPRESSION_ADD;
                break;
            case '-':
                operator = EXPRESSION_SUBTRACT;
                break;
            case '*':
                operator = EXPRESSION_MULTIPLY;
                break;
            case '/':
                operator = EXPRESSION_DIVIDE;
                break;
            case '^':
                operator = EXPRESSION_EXPONENTIATE;
                break;
            case ')':
            case 0: {
                // if there's a closed parenthesis or EOI, we can just return a root
                *result = lhs;
                return false;
            }
        }

        // if the operator we've received is lower than our starting operator,
        // we must stop and make the expression we've constructed so far the new lhs
        if (operator_value(operator) < operator_value(last_operator)) {
            *result = lhs;
            return true;
        }

        // get the rhs expression
        ++*str;
        struct Expression* rhs;
        bool was_interrupted = parse(&rhs, str, operator);
        if (was_interrupted) {
            struct Expression* new_lhs = create_expr();
            new_lhs->lhs = lhs;
            new_lhs->rhs = rhs;
            new_lhs->type = operator;
            lhs = new_lhs;
            continue;
        } else {
            struct Expression* final = create_expr();
            final->lhs = lhs;
            final->rhs = rhs;
            final->type = operator;
            *result = final;
            return false;
        }
    }
}
