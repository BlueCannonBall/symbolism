#pragma string name SYMBOL

// #include "parser.h"
#include "console.h"
#include "parser.h"

int main(void) {
    initialize_console();
    reset_expr_heap();

    char buf[20];
    read_line(buf, 20);
    print("");
    print("");
    print("");

    struct Expression* expr;
    const char* ptr = buf;
    parse(&expr, &ptr, EXPRESSION_NONE);
    //struct Expression* derivative = differentiate(expr, 'x');
    char format_buf[256];
    char* ptr2 = format_buf;
    //struct Expression* simplified = simplify_expression(derivative);
    format_expression(expr, &ptr2);
    print(format_buf);

    return 0;
}
