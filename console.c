#include "console.h"
#include <games.h>

// Screen: 96x64
// Char: 5x7

// char sprite[][35] = {
//     ['0'] = {
//         5,
//         7,
//         0b01110 << 3,
//         0b10001 << 3,
//         0b10011 << 3,
//         0b10101 << 3,
//         0b11001 << 3,
//         0b10001 << 3,
//         0b01110 << 3},
//     // ['1'] = {5, 7, 0b00100 << 3, 0b01100 << 3, 0b00100 << 3, 0b00100 << 3, 0b00100 << 3, 0b00100 << 3, 0b01110 << 3},
// };

unsigned char line = 0;

unsigned char* get_char_sprite(char c) {
    switch (c) {
    case '0': {
        static unsigned char sprite_0[] = {
            5,
            7,
            0b01110 << 3,
            0b10001 << 3,
            0b10011 << 3,
            0b10101 << 3,
            0b11001 << 3,
            0b10001 << 3,
            0b01110 << 3,
        };
        return sprite_0;
    }

    case '1': {
        static unsigned char sprite_1[] = {
            5,
            7,
            0b00100 << 3,
            0b01100 << 3,
            0b00100 << 3,
            0b00100 << 3,
            0b00100 << 3,
            0b00100 << 3,
            0b01110 << 3,
        };
        return sprite_1;
    }

    case '2': {
        static unsigned char sprite_2[] = {
            5,
            7,
            0b01110 << 3,
            0b10001 << 3,
            0b00001 << 3,
            0b00010 << 3,
            0b00100 << 3,
            0b01000 << 3,
            0b11111 << 3,
        };
        return sprite_2;
    }
    }
}

void print(const char* str) {
    for (unsigned char i = 0; str[i]; ++i) {
        putsprite(SPR_OR, i * 6, line * 7, get_char_sprite(str[i]));
    }
    ++line;
}
