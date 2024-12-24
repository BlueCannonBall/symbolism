#pragma output nostreams

#include "console.h"
#include <ctype.h>
#include <games.h>
#include <graphics.h>
#include <stdio.h>
#include <string.h>

// Screen: 96x64
// unsigned char: 5x7

unsigned char line = 0;

void initialize_console(void) {
    clg();
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

        if (str[i] == '\n') {
            col = 0;
            if (++line >= 8) {
                clg();
                line = 0;
            }
        } else if (str[i] != ' ') {
            unsigned char sprite[9];
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

char read_key(void) {
    static const char* special_sequence = NULL;

    if (special_sequence) {
        char ret = *(special_sequence++);
        if (!*special_sequence) {
            special_sequence = NULL;
        }
        return ret;
    }

    int ret = tolower(getk());
    switch (ret) {
    case 180: return 'x';
    case 140: return '-';
    case 183: special_sequence = "in("; return 's';
    case 185: special_sequence = "os("; return 'c';
    case 187: special_sequence = "an("; return 't';
    case 184: special_sequence = "rcsin("; return 'a';
    case 186: special_sequence = "rccos("; return 'a';
    case 188: special_sequence = "rctan("; return 'a';
    }
    return ret;
}

void read_line(char* str, unsigned char size) {
    unsigned char i = 0;
    for (unsigned char col = 0; i < size - 1; ++i, ++col) {
        if (col >= 16) {
            col = 0;
            if (++line >= 8) {
                clg();
                line = 0;
            }
        }

        // Render cursor
        xorclga(col * 6, line * 8, 5, 7);

        char key = read_key();
        xorclga(col * 6, line * 8, 5, 7); // Clear cursor
        if (key == 10) {
            break;
        } else if (key != ' ') {
            unsigned char sprite[9];
            sprite[0] = 5;
            sprite[1] = 7;
            memcpy(sprite + 2, get_char_sprite(str[i]), 7);
            putsprite(SPR_OR, col * 6, line * 8, sprite);
        }
    }
    str[i] = 0;

    if (++line >= 8) {
        clg();
        line = 0;
    }
}

const char* number_to_string(float value) {
    static char ret[30];
    char* ptr = ret;

    if (value < 0) {
        *ptr++ = '-';
        value = -value;
    }

    int integer = value;
    float fraction = value - integer;

    char int_str[20];
    unsigned char i = 0;
    do {
        int_str[i++] = '0' + (integer % 10);
        integer /= 10;
    } while (integer > 0);
    while (i > 0) {
        *ptr++ = int_str[--i];
    }

    if (fraction > 0.001f) {
        *ptr++ = '.';
        do {
            fraction *= 10;
            int digit = (int) fraction;
            *ptr++ = '0' + digit;
            fraction -= digit;
        } while (fraction > 0.001f);
    }

    *ptr = '\0';
    return ret;
}
