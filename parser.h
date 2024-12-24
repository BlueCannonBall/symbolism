#ifndef _PARSER_H
#define _PARSER_H

#include <stdbool.h>

enum ExpressionType {
    EXPRESSION_NUMBER = 0,
    EXPRESSION_NONE, // a special type for the parser
    EXPRESSION_VARIABLE,
    EXPRESSION_ADD,
    EXPRESSION_SUBTRACT,
    EXPRESSION_MULTIPLY,
    EXPRESSION_DIVIDE,
    EXPRESSION_EXPONENTIATE,
};

struct Expression {
    enum ExpressionType type;
    float number;
    char variable;
    struct Expression* lhs;
    struct Expression* rhs;
};

struct Expression* create_expr(void);
void reset_expr_heap(void);
float read_number(const char** str);
bool parse(struct Expression** result, const char** str, enum ExpressionType last_operator);
float evaluate_expression(const struct Expression* expr);

#endif
