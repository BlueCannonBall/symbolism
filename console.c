#include "console.h"
#include <ctype.h>
#include <games.h>
#include <graphics.h>

// Screen: 96x64
// Char: 5x7

unsigned char line = 0;

unsigned char* get_char_sprite(char c) {
    switch (c) {
    case '0': {
        static unsigned char sprite_0[] = {
            5,
            7,
            0x70,
            0x88,
            0x98,
            0xA8,
            0xC8,
            0x88,
            0x70,
        };
        return sprite_0;
    }

    case '1': {
        static unsigned char sprite_1[] = {
            5,
            7,
            0x20,
            0x60,
            0x20,
            0x20,
            0x20,
            0x20,
            0x70,
        };
        return sprite_1;
    }

    case '2': {
        static unsigned char sprite_2[] = {
            5,
            7,
            0x70,
            0x88,
            0x08,
            0x10,
            0x20,
            0x40,
            0xF8,
        };
        return sprite_2;
    }

    case '3': {
        static unsigned char sprite_3[] = {
            5,
            7,
            0x70,
            0x88,
            0x08,
            0x30,
            0x08,
            0x88,
            0x70,
        };
        return sprite_3;
    }

    case '4': {
        static unsigned char sprite_4[] = {
            5,
            7,
            0x90,
            0x90,
            0x90,
            0xF8,
            0x10,
            0x10,
            0x10,
        };
        return sprite_4;
    }

    case '5': {
        static unsigned char sprite_5[] = {
            5,
            7,
            0xF8,
            0x80,
            0xF0,
            0x08,
            0x08,
            0x88,
            0x70,
        };
        return sprite_5;
    }

    case '6': {
        static unsigned char sprite_6[] = {
            5,
            7,
            0x30,
            0x40,
            0x80,
            0xF0,
            0x88,
            0x88,
            0x70,
        };
        return sprite_6;
    }

    case '7': {
        static unsigned char sprite_7[] = {
            5,
            7,
            0xF8,
            0x08,
            0x10,
            0x20,
            0x20,
            0x40,
            0x40,
        };
        return sprite_7;
    }

    case '8': {
        static unsigned char sprite_8[] = {
            5,
            7,
            0x70,
            0x88,
            0x88,
            0x70,
            0x88,
            0x88,
            0x70,
        };
        return sprite_8;
    }

    case '9': {
        static unsigned char sprite_9[] = {
            5,
            7,
            0x70,
            0x88,
            0x88,
            0x78,
            0x08,
            0x10,
            0x60,
        };
        return sprite_9;
    }

    case 'x': {
        static unsigned char sprite_x[] = {
            5,
            7,
            0x00,
            0x00,
            0x88,
            0x50,
            0x20,
            0x50,
            0x88,
        };
        return sprite_x;
    }

    case 'y': {
        static unsigned char sprite_y[] = {
            5,
            7,
            0x00,
            0x00,
            0x88,
            0x88,
            0x78,
            0x08,
            0x70,
        };
        return sprite_y;
    }

    case 'z': {
        static unsigned char sprite_z[] = {
            5,
            7,
            0x00,
            0x00,
            0xF8,
            0x10,
            0x20,
            0x40,
            0xF8,
        };
        return sprite_z;
    }

    case '.': {
        static unsigned char sprite_dot[] = {
            5,
            7,
            0x00,
            0x00,
            0x00,
            0x00,
            0x00,
            0x60,
            0x60,
        };
        return sprite_dot;
    }

    case '+': {
        static unsigned char sprite_plus[] = {
            5,
            7,
            0x00,
            0x20,
            0x20,
            0xF8,
            0x20,
            0x20,
            0x00,
        };
        return sprite_plus;
    }

    case '-': {
        static unsigned char sprite_minus[] = {
            5,
            7,
            0x00,
            0x00,
            0x00,
            0xF8,
            0x00,
            0x00,
            0x00,
        };
        return sprite_minus;
    }

    case '*': {
        static unsigned char sprite_multiply[] = {
            5,
            7,
            0x00,
            0x00,
            0x50,
            0x20,
            0x50,
            0x00,
            0x00,
        };
        return sprite_multiply;
    }

    case '/': {
        static unsigned char sprite_divide[] = {
            5,
            7,
            0x00,
            0x08,
            0x10,
            0x20,
            0x40,
            0x80,
            0x00,
        };
        return sprite_divide;
    }

    default: {
        static unsigned char sprite_none[] = {
            5,
            7,
            0xD8,
            0xD8,
            0x00,
            0x00,
            0x70,
            0x88,
            0x88,
        };
        return sprite_none;
    }
    }
}

void print(const char* str) {
    for (unsigned char i = 0, col = 0; str[i]; ++i, ++col) {
        if (col >= 16) {
            col = 0;
            if (++line >= 8) {
                clg();
                line = 0;
            }
        }

        if (str[i] != ' ') {
            putsprite(SPR_OR, col * 6, line * 8, get_char_sprite(tolower(str[i])));
        }
    }

    if (++line >= 8) {
        clg();
        line = 0;
    }
}
