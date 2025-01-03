#pragma output nostreams

#include "console.h"
#include <ctype.h>
#include <games.h>
#include <graphics.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// Screen: 96x64
// unsigned char: 5x7

unsigned char line = 0;

void initialize_console(void) {
    line = 0;
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
            memcpy(sprite + 2, get_char_sprite(key), 7);
            putsprite(SPR_OR, col * 6, line * 8, sprite);
        }
        str[i] = key;
    }
    str[i] = 0;

    if (++line >= 8) {
        clg();
        line = 0;
    }
}

const char* number_to_string(float value) {
    static char buf[32];  // Static buffer for result
    char* ptr = buf;

    // Handle negative numbers
    if (value < 0) {
        *ptr++ = '-';
        value = -value;
    }

    // Extract integer part
    int int_part = (int)value;
    float fractional_part = value - int_part;

    // Convert integer part to string
    char int_buf[12];  // Temporary buffer for integer
    char* int_ptr = int_buf + sizeof(int_buf) - 1;
    *int_ptr = '\0';
    do {
        *--int_ptr = '0' + (int_part % 10);
        int_part /= 10;
    } while (int_part > 0);

    // Append integer part to result
    while (*int_ptr) {
        *ptr++ = *int_ptr++;
    }

    // Only handle the fractional part if it's non-zero
    if (fractional_part > 0.0001f) {
        *ptr++ = '.';

        // Convert fractional part to string, avoiding unnecessary zeros
        bool fractional_started = false;
        for (int i = 0; i < 6; ++i) { // Limit to 6 decimal places
            fractional_part *= 10;
            int digit = (int)fractional_part;
            if (digit != 0 || fractional_started) {  // Only print if non-zero or after we start printing
                *ptr++ = '0' + digit;
                fractional_part -= digit;
                fractional_started = true;
            } else {
                fractional_part -= digit;  // Skip trailing zeros
            }

            if (fractional_part < 0.0001f) break;  // Stop if the fractional part is effectively zero
        }
    }

    *ptr = '\0'; // Null-terminate the string
    return buf;
}
