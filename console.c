#include "console.h"
#include <games.h>
#include <graphics.h>
#include <string.h>

// Screen: 96x64
// Char: 5x7

unsigned char line = 0;

void print(const char* str) {
    for (unsigned char i = 0, col = 0; str[i]; ++i, ++col) {
        if (col >= 16) {
            col = 0;
            if (++line >= 8) {
                clg();
                line = 0;
            }
        }

        if (str[i] == '\n') {
            col = 0;
            if (++line >= 8) {
                clg();
                line = 0;
            }
        } else if (str[i] != ' ') {
            char sprite[9];
            sprite[0] = 5;
            sprite[1] = 7;
            memcpy(sprite + 2, get_char_sprite(str[i]), 7);
            putsprite(SPR_OR, col * 6, line * 8, sprite);
        }
    }

    if (++line >= 8) {
        clg();
        line = 0;
    }
}
