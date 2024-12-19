#include "parser.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned char expr_cursor = 0;

struct Expression* create_expr() {
    static struct Expression exprs[64] = {0}; // 64 expressions MAXIMUM at ALL TIMES
    struct Expression* ret = &exprs[expr_cursor++];
    if (expr_cursor >= 64) {
        fprintf(stderr, "=== HEAP FAILURE ===\n");
        exit(EXIT_FAILURE);
    }
    memset(ret, 0, sizeof(struct Expression));
    return ret;
}

void reset_expr_heap() {
    expr_cursor = 0;
}

int read_number(const char* buf, unsigned int* cursor) {
    int ret;
    // idk if c can do this out of the box, but you can iterate over every digit and
    // ret += digit * offset where offset is the distance into the number
    // for (; buf[cursor])
}

// need some kind of state machine
// imagine you see a number as char 1. you have a few options after a number:
// * / + etc, another digit, or a VARIABLE. if you see a variable right after a number you've got yourself an expr
// make a separate function which parses a number u
// make a func that reads a number until it gets another charactter yes
struct Expression* parse(const char* str) {
    struct Expression* expr = create_expr();
    for (unsigned int i = 0; str[i]; ++i) {
        if (isdigit(str[i])) {
        }
    }
    return NULL;
}
