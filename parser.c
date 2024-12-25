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

// Create a new Expression of type number
struct Expression* create_number(float value) {
    struct Expression* expr = create_expr();
    expr->type = EXPRESSION_NUMBER;
    expr->number = value;
    return expr;
}

// Create a new Expression of type variable
struct Expression* create_variable(char variable) {
    struct Expression* expr = create_expr();
    expr->type = EXPRESSION_VARIABLE;
    expr->variable = variable;
    return expr;
}

// Create a new Expression of a binary operation
struct Expression* create_binary(enum ExpressionType type, struct Expression* lhs, struct Expression* rhs) {
    struct Expression* expr = create_expr();
    expr->type = type;
    expr->lhs = lhs;
    expr->rhs = rhs;
    return expr;
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
        lhs = create_number(read_number(str));
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
        lhs = create_variable(**str);

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

struct PolynomialTerm {
    float coefficient;
    char variable;
    double power;
};

// Despite the name "parse", no parsing really occurs here. This function operates on an already parsed `struct Expression*`
bool try_parse_polynomial_term(struct PolynomialTerm* result, const struct Expression* expr) {
    // the expr must be a multiplication or a single variable
    if (expr->type == EXPRESSION_VARIABLE) {
        result->coefficient = 1.0;
        result->variable = expr->variable;
        result->power = 1.0;
        return true;
    } else if (expr->type == EXPRESSION_NUMBER) {
        result->coefficient = expr->number;
        result->variable = 0; // it doesn't actually matter in this case
        result->power = 0.0;
        return true;

    } else if (expr->type != EXPRESSION_MULTIPLY) {
        return false;
    }

    // one side must be a number
    const struct Expression* number = NULL;
    if (expr->lhs->type == EXPRESSION_NUMBER) {
        number = expr->lhs;
    } else if (expr->rhs->type == EXPRESSION_NUMBER) {
        number = expr->rhs;
    }

    // another side must be a variable...
    const struct Expression* variable = NULL;
    if (expr->lhs->type == EXPRESSION_VARIABLE) {
        variable = expr->lhs;
    } else if (expr->rhs->type == EXPRESSION_VARIABLE) {
        variable = expr->rhs;
    }

    // ... or an exponent with a constant power
    const struct Expression* power = NULL;
    if (expr->lhs->type == EXPRESSION_EXPONENTIATE && expr->lhs->lhs->type == EXPRESSION_VARIABLE && expr->lhs->rhs->type == EXPRESSION_NUMBER) {
        variable = expr->lhs->lhs;
        power = expr->lhs->rhs;
    } else if (expr->rhs->type == EXPRESSION_EXPONENTIATE && expr->rhs->lhs->type == EXPRESSION_VARIABLE && expr->rhs->rhs->type == EXPRESSION_NUMBER) {
        variable = expr->rhs->lhs;
        power = expr->rhs->rhs;
    }

    if (number && variable) {
        result->variable = variable->variable;
        result->coefficient = number->number;
        if (power) {
            result->power = power->number;
        } else {
            result->power = 0.0f;
        }

        return true;
    }

    return false;
}

// (2*x)
// becomes Term { coefficient: 2, variables: [x, 1] }
// (2*x)*(2*(x*y))
// (2*x) becomes Term { coefficient: 2, variables: [x, 1] }
// (x*y) becomes Term { coefficient: 1, variables: [x, 1, y, 1]
// 2*(x*y) becomes
struct Term {

};

struct Expression* simplify_expression(const struct Expression* expr) {
    struct Expression* out = create_expr();
    switch (expr->type) {
        // nothing we can do here!
        case EXPRESSION_VARIABLE:
        case EXPRESSION_NONE:
        case EXPRESSION_NUMBER:
            *out = *expr;
            break;
        case EXPRESSION_MULTIPLY: {
            // we can simplify a multiplication if there are constants/exponents to be collapsed
            // e.g. (x) * (x) = x^2
            // e.g. (2*x) * (x) = 2*x^2
            // e.g. (2*x) * (3*x) = 6*x^2
            // e.g. (2*x) * (3*y) = 6*(x*y)

            // Simplify lhs and rhs
            struct Expression* new_lhs = simplify_expression(expr->lhs);
            struct Expression* new_rhs = simplify_expression(expr->rhs);

            struct PolynomialTerm lhs_polynomial;
            struct PolynomialTerm rhs_polynomial;

            out->type = EXPRESSION_NUMBER;

            // CASE 1: BOTH SIDES ARE JUST CONSTANTS
            if (new_lhs->type == EXPRESSION_NUMBER && new_rhs->type == EXPRESSION_NUMBER) {
                out->number = new_lhs->number * new_rhs->number;
                break;
            }

            out->type = EXPRESSION_MULTIPLY;

            if (try_parse_polynomial_term(&lhs_polynomial, new_lhs) && try_parse_polynomial_term(&rhs_polynomial, new_rhs)) {
                if ((lhs_polynomial.variable == rhs_polynomial.variable) || (lhs_polynomial.power == 0.0 || rhs_polynomial.power == 0.0)) {
                    // one of the terms might have a power of zero, in which case there is no variable
                    // .. TODO
                    // combine the terms
                    float new_power = lhs_polynomial.power + rhs_polynomial.power;
                    float new_coefficient = lhs_polynomial.coefficient * rhs_polynomial.coefficient;

                    new_lhs = create_expr();
                    new_lhs->type = EXPRESSION_NUMBER;
                    new_lhs->number = new_coefficient;

                    new_rhs = create_expr();
                    if (new_power == 1.0) {
                        new_rhs->type = EXPRESSION_VARIABLE;
                        new_rhs->variable = lhs_polynomial.variable; // doesn't matter whether it's lhs or rhs!
                    } else {
                        new_rhs->type = EXPRESSION_EXPONENTIATE;
                        new_rhs->lhs = create_expr();
                        new_rhs->lhs->type = EXPRESSION_VARIABLE;
                        new_rhs->lhs->variable = lhs_polynomial.variable; // doesn't matter whether it's lhs or rhs!
                        new_rhs->rhs = create_expr();
                        new_rhs->rhs->type = EXPRESSION_NUMBER;
                        new_rhs->rhs->number = new_power;
                    }

                    out->lhs = new_lhs;
                    out->rhs = new_rhs;
                    break;
                } else {
                    // This is the most complicated case: (2*x) * (3*y) = 6*(x*y)
                    print("Not implemented!");
                    break;
                }
            } else {
                // Don't do any combining
                out->lhs = new_lhs;
                out->rhs = new_rhs;
                break;
            }
        }
    }

    return out;
}
