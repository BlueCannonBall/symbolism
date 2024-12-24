#pragma string name SYMBOL

// #include "parser.h"
#include "console.h"
#include "parser.h"

int main(void) {
    initialize_console();

    char buf[20];
    read_line(buf, 20);
    print("");
    print("");
    print("");

    struct Expression* expr;
    const char* ptr = buf;
    parse(&expr, &ptr, EXPRESSION_NONE);
    print(number_to_string(evaluate_expression(expr)));

    return 0;
}
