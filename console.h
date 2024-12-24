#ifndef _CONSOLE_H
#define _CONSOLE_H

enum SpecialKey {
	SPECIAL_KEY_NONE,
    SPECIAL_KEY_SIN,
    SPECIAL_KEY_ARCSIN,
    SPECIAL_KEY_COS,
    SPECIAL_KEY_ARCCOS,
    SPECIAL_KEY_TAN,
    SPECIAL_KEY_ARCTAN,
};

void initialize_console(void);
const unsigned char* get_char_sprite(char c);
void print(const char* str);
char read_key(void);
void read_line(char* str, unsigned char size);
const char* number_to_string(float value);

#endif
