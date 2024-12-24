#include "parser.h"
#include "console.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
// #include "console.h"

unsigned char expr_cursor = 0;

struct Expression* create_expr(void) {
    static struct Expression exprs[64]; // 64 expressions MAXIMUM at ALL TIMES
    struct Expression* ret = &exprs[expr_cursor++];
    if (expr_cursor >= 64) {
        print("==MEM EXCEPTION==");
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
    case EXPRESSION_VARIABLE:
    case EXPRESSION_NUMBER:
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
    // STEP 1: PARSE THE FIRST OPERAND OF THE EXPRESSION
    // THE FIRST OPERAND CAN BE ONE OF THREE:
    // 1. A NUMBER
    // 2. AN EXPRESSION (DENOTED BY AN OPEN PARENTHESIS)
    // 3. A VARIABLE
    struct Expression* lhs;
    if (isdigit(**str) || **str == '-') {
        // 1. NUMBER
        lhs = create_expr();
        lhs->type = EXPRESSION_NUMBER;
        lhs->number = read_number(str);
    } else if (**str == '(') {
        // 2. AN EXPRESSION
        ++*str;
        parse(&lhs, str, EXPRESSION_NONE);
        // after that, there should be a closed parenthesis
        if (**str != ')') {
            print("==PARSING EXCEPTION==");
        }

        // need to get whatever is after the closed parenthesis
        ++*str;
    } else if (isalpha(**str)) {
        // 3. A VARIABLE
        lhs = create_expr();
        lhs->type = EXPRESSION_VARIABLE;
        lhs->variable = **str;

        // need to get whatever is after the variable
        ++*str;
    }

    // STEP 2: PARSE THE SECOND OPERAND. THIS IS A LOOP BECAUSE THE PROCESS MAY REPEAT IF
    // A "LOWER" OPERATOR IS ENCOUNTERED, FORCING THE LHS TO BE REEVALUATED
    while (true) {
        // AFTER THE FIRST OPERAND, THERE MAY BE:
        // 1. A SYMBOL (AN OPERATOR)
        // 2. A CLOSED PARENTHESIS (ENDING AN EXPRESSION)
        // 3. A VARIABLE OR AN OPENED PARANTHESIS (IMPLICIT MULTIPLICATION)
        bool is_implicit = false;
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
            case '(':
            default:
                // if the character is anything else, we have an implicit multiplication with a variable/parenthetic expression
                operator = EXPRESSION_MULTIPLY;
                is_implicit = true;
                break;
        }

        // if the operator we've received is lower than our starting operator,
        // we must stop and make the expression we've constructed so far the new lhs
        if (operator_value(operator) <= operator_value(last_operator)) {
            *result = lhs;
            return true;
        }

        // get the rhs expression, if the operator is implicit there is no need to move forward
        if (!is_implicit) {
            ++*str;
        }
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

// Approximates the exponential function expf(x) using a Taylor series expansion
float expf_approx(float x) {
    float result = 1.0f;
    float term = 1.0f;
    int n;

    for (n = 1; n < 20; ++n) {
        term *= x / n;
        result += term;
    }
    return result;
}

// Approximates the natural logarithm logf(x) using a series expansion around 1
float logf_approx(float x) {
    if (x <= 0.0f) return -1.0f; // Invalid input

    // For small values of x close to 1, use series approximation: log(x) = 2 * ((x-1)/(x+1) + ((x-1)/(x+1))^3/3 + ((x-1)/(x+1))^5/5 + ...)
    float y = (x - 1) / (x + 1);
    float y2 = y * y;
    float result = 0.0f;
    float term = y;

    for (int n = 1; n <= 15; n += 2) {
        result += term / n;
        term *= y2;
    }
    return 2 * result;
}

bool is_whole(float num) {
    int truncated = (int)num;  // Truncate the float to an integer
    return num == (float)truncated;  // Check if the original number matches the truncated value
}

// Approximates powf(a, b) by using the relation: a^b = expf(b * logf(a))
float powf_approx(float a, float b) {
    if (b < 0) {
        return 1.0f / powf_approx(a, -b);
    }
    if (a == 0.0f && b > 0) {
        return 0.0f; // 0 raised to a positive power is 0
    }
    if (is_whole(b)) {
        float result = 1.0f;
        for (int i = 0; i < (int)b; ++i) {
            result *= a;
        }
        return result;
    } else {
        return expf_approx(b * logf_approx(a));
    }
}

float evaluate_expression(const struct Expression* expr) {
    switch (expr->type) {
        case EXPRESSION_NUMBER:
            return expr->number;
        case EXPRESSION_NONE:
        case EXPRESSION_VARIABLE:
            return 0.0f;
        case EXPRESSION_ADD:
            return evaluate_expression(expr->lhs) + evaluate_expression(expr->rhs);
        case EXPRESSION_SUBTRACT:
            return evaluate_expression(expr->lhs) - evaluate_expression(expr->rhs);
        case EXPRESSION_MULTIPLY:
            return evaluate_expression(expr->lhs) * evaluate_expression(expr->rhs);
        case EXPRESSION_DIVIDE:
            return evaluate_expression(expr->lhs) / evaluate_expression(expr->rhs);
        case EXPRESSION_EXPONENTIATE:
            return powf_approx(evaluate_expression(expr->lhs), evaluate_expression(expr->rhs));
    }
}
