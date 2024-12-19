#pragma once

enum OperationType {
    OPERATION_NONE = 0,
    OPERATION_ADD,
    OPERATION_SUBTRACT,
    OPERATION_MULTIPLY,
    OPERATION_DIVIDE,
    OPERATION_EXPONENTIATE,
};

struct Expression {
    enum OperationType type;
    struct Operation* lhs;
    struct Operation* rhs;
};

struct Expression* create_expr();
void reset_expr_heap();
struct Expression* parse(const char* str);
