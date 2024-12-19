#pragma string name SYMBOL

#include "parser.h"
#include <stdio.h>

int main() {
    char line[20];
    puts("Enter equation:");
    fgets(line, sizeof(line), stdin);

    struct Expression* expr = parse(line);

    printf("%s\n", line);
    return 0;
}
