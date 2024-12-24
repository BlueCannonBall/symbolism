#pragma once

#include <stdbool.h>

enum ExpressionType {
    EXPRESSION_NONE = 0,
    EXPRESSION_ADD,
    EXPRESSION_SUBTRACT,
    EXPRESSION_MULTIPLY,
    EXPRESSION_DIVIDE,
    EXPRESSION_EXPONENTIATE,
};

struct Expression {
    enum ExpressionType type;
    float number;
    struct Expression* lhs;
    struct Expression* rhs;
};

struct Expression* create_expr(void);
void reset_expr_heap(void);
float read_number(const char** str);
bool parse(struct Expression** result, const char** str, enum ExpressionType last_operator);
