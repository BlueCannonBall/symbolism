#pragma once

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

struct Expression* create_expr();
void reset_expr_heap();
float read_number(const char** str);
struct Expression* parse(const char** str);
