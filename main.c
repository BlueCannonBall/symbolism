#pragma string name SYMBOL

// #include "parser.h"
#include "console.h"

int main(void) {
    initialize_console();
    
    char buf[8];
    read_line(buf, 8);
    print(buf);
    print(buf);
    print(buf);

    print(number_to_string(buf[0]));

    return 0;
}
