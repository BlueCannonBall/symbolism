// parser.c

// Implements functions for parsing mathematical expressions,
// diffrentiating them, and simplifying them.  

#include "parser.h"
#include "console.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static unsigned char expr_cursor = 0;

// This ad hoc allocator exists because this has to work on the
// black-and-white TI-84+ calculator. Z88DK, the SDK used to compile
// this, doesn't provide malloc & free for that calculator.
struct Expression* create_expr(void) {
    static struct Expression exprs[128]; // 128 expressions MAXIMUM at ALL TIMES
    struct Expression* ret = &exprs[expr_cursor++];
    if (expr_cursor >= 128) {
        print("==MEM EXCEPTION==");
        exit(EXIT_FAILURE);
    }
    memset(ret, 0, sizeof(struct Expression));
    return ret;
}

void reset_expr_heap(void) {
    expr_cursor = 0;
}

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

struct Expression* create_number(float value) {
    struct Expression* expr = create_expr();
    expr->type = EXPRESSION_NUMBER;
    expr->number = value;
    return expr;
}

struct Expression* create_variable(char variable) {
    struct Expression* expr = create_expr();
    expr->type = EXPRESSION_VARIABLE;
    expr->variable = variable;
    return expr;
}

struct Expression* create_binary(enum ExpressionType type, struct Expression* lhs, struct Expression* rhs) {
    if (!lhs || !rhs) return NULL;

    struct Expression* expr = create_expr();
    expr->type = type;
    expr->lhs = lhs;
    expr->rhs = rhs;
    return expr;
}

struct Expression* create_copy(const struct Expression* expr) {
    if (!expr) return NULL;
    struct Expression* expr2 = create_expr();
    expr2->type = expr->type;
    expr2->number = expr->number;
    expr2->variable = expr->variable;
    expr2->lhs = create_copy(expr->lhs);
    expr2->rhs = create_copy(expr->rhs);
    return expr2;
}

// Returns a bool stating whether the parsing process was interrupted
bool parse(struct Expression** result, const char** str, enum ExpressionType last_operator) {
    // STEP 1: PARSE THE FIRST OPERAND OF THE EXPRESSION
    // THE FIRST OPERAND CAN BE ONE OF FOUR:
    // 1. A NUMBER
    // 2. A UNARY-NEGATED OPERAND
    // 3. AN EXPRESSION (DENOTED BY AN OPEN PARENTHESIS)
    // 4. A VARIABLE
    struct Expression* lhs;
    if (isdigit(**str)) {
        // 1. NUMBER
        lhs = create_number(read_number(str));
    } else if (**str == '-') {
        // 2. UNARY MINUS
        ++*str;
        struct Expression* operand;
        parse(&operand, str, EXPRESSION_MULTIPLY);
        if (!operand) {
            *result = NULL;
            return false;
        }
        if (operand->type == EXPRESSION_NUMBER) {
            operand->number = -operand->number;
            lhs = operand;
        } else {
            lhs = create_binary(EXPRESSION_MULTIPLY, create_number(-1.0f), operand);
        }
    } else if (**str == '(') {
        // 3. AN EXPRESSION
        ++*str;
        parse(&lhs, str, EXPRESSION_NONE);
        if (!lhs) {
            *result = NULL;
            return false;
        }
        // after that, there should be a closed parenthesis
        if (**str != ')') {
            print("==PARSING EXCEPTION==");
            *result = lhs;
            return false;
        }

        // need to get whatever is after the closed parenthesis
        ++*str;
    } else if (isalpha(**str)) {
        // 4. A VARIABLE
        lhs = create_variable(**str);

        // need to get whatever is after the variable
        ++*str;
    } else {
        print("==PARSING EXCEPTION==");
        *result = NULL;
        return false;
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
        if (!rhs) {
            *result = NULL;
            return false;
        }
        if (was_interrupted) {
            lhs = create_binary(operator, lhs, rhs);
            continue;
        } else {
            *result = create_binary(operator, lhs, rhs);
            return false;
        }
    }
}

// Approximates the exponential function expf(x) using a Taylor series expansion
float expf_approx(float x) {
    float result = 1.0f;
    float term = 1.0f;
    for (int n = 1; n < 20; ++n) {
        term *= x / n;
        result += term;
    }
    return result;
}

// Approximates the natural logarithm logf(x) using a series expansion around 1
float logf_approx(float x) {
    if (x <= 0.0f) return -1.0f;

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
    int truncated = (int) num;       // Truncate the float to an integer
    return num == (float) truncated; // Check if the original number matches the truncated value
}

// Approximates powf(a, b) by using the relation: a^b = expf(b * logf(a))
float powf_approx(float a, float b) {
    if (b < 0) {
        return 1.0f / powf_approx(a, -b);
    }
    if (a == 0.0f && b > 0) {
        return 0.0f;
    }
    if (is_whole(b)) {
        float result = 1.0f;
        for (int i = 0; i < (int) b; ++i) {
            result *= a;
        }
        return result;
    } else {
        return expf_approx(b * logf_approx(a));
    }
}

// Evaluate an expression. All variables will be replaced with zeros.
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

struct Expression* differentiate(const struct Expression* expr, char independent_variable) {
    switch (expr->type) {
    case EXPRESSION_NUMBER:
        return create_number(0.0);
    case EXPRESSION_VARIABLE: {
        if (independent_variable == expr->variable) {
            return create_number(1.0);
        } else {
            return create_number(0.0);
        }
    }
    case EXPRESSION_ADD:
        return create_binary(EXPRESSION_ADD, differentiate(expr->lhs, independent_variable), differentiate(expr->rhs, independent_variable));
    case EXPRESSION_SUBTRACT:
        return create_binary(EXPRESSION_SUBTRACT, differentiate(expr->lhs, independent_variable), differentiate(expr->rhs, independent_variable));
    case EXPRESSION_MULTIPLY:
        // d/dx f(x) + g(x) = f'(x)*g(x) + f(x)*g'(x)
        return create_binary(
            EXPRESSION_ADD,
            create_binary(
                EXPRESSION_MULTIPLY,
                differentiate(expr->lhs, independent_variable),
                create_copy(expr->rhs)),
            create_binary(
                EXPRESSION_MULTIPLY,
                create_copy(expr->lhs),
                differentiate(expr->rhs, independent_variable)));
    case EXPRESSION_DIVIDE:
        // d/dx [u(x)/v(x)] = (u'(x)v(x) - u(x)v'(x))/(v(x)^2)
        return create_binary(
            EXPRESSION_DIVIDE,
            create_binary(
                EXPRESSION_SUBTRACT,
                create_binary(
                    EXPRESSION_MULTIPLY,
                    differentiate(expr->lhs, independent_variable),
                    create_copy(expr->rhs)),
                create_binary(
                    EXPRESSION_MULTIPLY,
                    create_copy(expr->lhs),
                    differentiate(expr->rhs, independent_variable))),
            create_binary(
                EXPRESSION_EXPONENTIATE,
                create_copy(expr->rhs),
                create_number(2.0)));
    case EXPRESSION_EXPONENTIATE: {
        // we only consider the case where the power is a number
        if (expr->rhs->type != EXPRESSION_NUMBER) {
            print("Exponents without numeric powers are undifferentiable");
            return NULL;
        }
        // d/dx f(x)^n = n(f(x))^(n-1) * f'(x)
        return create_binary(
            EXPRESSION_MULTIPLY,
            create_binary(
                EXPRESSION_MULTIPLY,
                create_number(expr->rhs->number), // n
                create_binary(
                    EXPRESSION_EXPONENTIATE,
                    create_copy(expr->lhs),              // f(x)
                    create_number(expr->rhs->number - 1) // n - 1
                    )),
            differentiate(expr->lhs, independent_variable) // f'(x)
        );
    }
    case EXPRESSION_NONE:
        print("Invalid input");
        return NULL;
    }
}

void append_to_buffer(char** buffer, const char* str) {
    while (*str) {
        **buffer = *str;
        ++(*buffer);
        ++str;
    }
    **buffer = 0;
}

// Function to format an expression
void format_expression(struct Expression* expr, char** buffer) {
    if (!expr) return;

    switch (expr->type) {
    case EXPRESSION_NUMBER: {
        const char* num_str = number_to_string(expr->number);
        append_to_buffer(buffer, num_str);
        break;
    }
    case EXPRESSION_VARIABLE: {
        char var_str[2];
        var_str[0] = expr->variable;
        var_str[1] = '\0';
        append_to_buffer(buffer, var_str);
        break;
    }
    case EXPRESSION_ADD:
    case EXPRESSION_SUBTRACT:
    case EXPRESSION_MULTIPLY:
    case EXPRESSION_DIVIDE:
    case EXPRESSION_EXPONENTIATE: {
        const char* operator = NULL;
        switch (expr->type) {
        case EXPRESSION_ADD: operator = "+"; break;
        case EXPRESSION_SUBTRACT: operator = "-"; break;
        case EXPRESSION_MULTIPLY: operator = "*"; break;
        case EXPRESSION_DIVIDE: operator = "/"; break;
        case EXPRESSION_EXPONENTIATE: operator = "^"; break;
        default: break;
        }
        append_to_buffer(buffer, "(");
        format_expression(expr->lhs, buffer);
        append_to_buffer(buffer, operator);
        format_expression(expr->rhs, buffer);
        append_to_buffer(buffer, ")");
        break;
    }
    case EXPRESSION_NONE: {
        append_to_buffer(buffer, "None");
        break;
    }
    }
}

struct Expression* simplify_expression(const struct Expression* expr) {
    if (!expr) return NULL;

    if (expr->type == EXPRESSION_NUMBER || expr->type == EXPRESSION_VARIABLE) {
        return create_copy(expr);
    }

    struct Expression* simplified_lhs = simplify_expression(expr->lhs);
    struct Expression* simplified_rhs = simplify_expression(expr->rhs);

    switch (expr->type) {
    case EXPRESSION_ADD:
        // 0 + x = x
        if (simplified_lhs->type == EXPRESSION_NUMBER && simplified_lhs->number == 0.0f) {
            struct Expression* result = create_copy(simplified_rhs);
            return result;
        }
        if (simplified_rhs->type == EXPRESSION_NUMBER && simplified_rhs->number == 0.0f) {
            struct Expression* result = create_copy(simplified_lhs);
            return result;
        }
        break;

    case EXPRESSION_MULTIPLY:
        // 0 * x = 0
        if ((simplified_lhs->type == EXPRESSION_NUMBER && simplified_lhs->number == 0.0f) ||
            (simplified_rhs->type == EXPRESSION_NUMBER && simplified_rhs->number == 0.0f)) {
            return create_number(0.0f);
        }
        // x * 1 = x
        if (simplified_lhs->type == EXPRESSION_NUMBER && simplified_lhs->number == 1.0f) {
            struct Expression* result = create_copy(simplified_rhs);
            return result;
        }
        if (simplified_rhs->type == EXPRESSION_NUMBER && simplified_rhs->number == 1.0f) {
            struct Expression* result = create_copy(simplified_lhs);
            return result;
        }
        break;

    case EXPRESSION_EXPONENTIATE:
        // x^1 = x
        if (simplified_rhs->type == EXPRESSION_NUMBER && simplified_rhs->number == 1.0f) {
            struct Expression* result = create_copy(simplified_lhs);
            return result;
        }
        // x^0 = 1
        if (simplified_rhs->type == EXPRESSION_NUMBER && simplified_rhs->number == 0.0f) {
            return create_number(1.0f);
        }
        break;

    default:
        break;
    }

    // Constant folding: If both sides are constants, compute the result
    if (simplified_lhs->type == EXPRESSION_NUMBER && simplified_rhs->type == EXPRESSION_NUMBER) {
        float result;
        switch (expr->type) {
        case EXPRESSION_ADD:
            result = simplified_lhs->number + simplified_rhs->number;
            break;
        case EXPRESSION_SUBTRACT:
            result = simplified_lhs->number - simplified_rhs->number;
            break;
        case EXPRESSION_MULTIPLY:
            result = simplified_lhs->number * simplified_rhs->number;
            break;
        case EXPRESSION_DIVIDE:
            result = simplified_lhs->number / simplified_rhs->number;
            break;
        case EXPRESSION_EXPONENTIATE:
            result = powf_approx(simplified_lhs->number, simplified_rhs->number);
            break;
        default:
            result = 0.0f;
            break;
        }
        return create_number(result);
    }

    struct Expression* result = create_binary(expr->type, simplified_lhs, simplified_rhs);
    return result;
}
