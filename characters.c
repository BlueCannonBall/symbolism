#include "console.h"

const char* get_char_sprite(char c) {
    switch (c) {
    case '0': {
        static const char sprite_0[] = {
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
        static const char sprite_1[] = {
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
        static const char sprite_2[] = {
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
        static const char sprite_3[] = {
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
        static const char sprite_4[] = {
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
        static const char sprite_5[] = {
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
        static const char sprite_6[] = {
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
        static const char sprite_7[] = {
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
        static const char sprite_8[] = {
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
        static const char sprite_9[] = {
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
        static const char sprite_x[] = {
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
        static const char sprite_y[] = {
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
        static const char sprite_z[] = {
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
        static const char sprite_dot[] = {
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

    case ',': {
        static const char sprite_comma[] = {
            0x00,
            0x00,
            0x00,
            0x00,
            0x60,
            0x20,
            0x40,
        };
        return sprite_comma;
    }

    case '!': {
        static const char sprite_exclamation[] = {
            0x70,
            0x70,
            0x70,
            0x20,
            0x20,
            0x00,
            0x20,
        };
        return sprite_exclamation;
    }

    case '?': {
        static const char sprite_question[] = {
            0x70,
            0x88,
            0x08,
            0x10,
            0x20,
            0x00,
            0x20,
        };
        return sprite_question;
    }

    case '+': {
        static const char sprite_plus[] = {
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
        static const char sprite_minus[] = {
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
        static const char sprite_multiply[] = {
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
        static const char sprite_divide[] = {
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

    case 'a': {
        static const char sprite_a[] = {
            0x00,
            0x00,
            0x70,
            0x08,
            0x78,
            0x88,
            0x78,
        };
        return sprite_a;
    }

    case 'b': {
        static const char sprite_b[] = {
            0x80,
            0x80,
            0x80,
            0xF0,
            0x88,
            0x88,
            0xF0,
        };
        return sprite_b;
    }

    case 'c': {
        static const char sprite_c[] = {
            0x00,
            0x00,
            0x78,
            0x80,
            0x80,
            0x80,
            0x78,
        };
        return sprite_c;
    }

    case 'd': {
        static const char sprite_d[] = {
            0x08,
            0x08,
            0x08,
            0x78,
            0x88,
            0x88,
            0x78,
        };
        return sprite_d;
    }

    case 'e': {
        static const char sprite_e[] = {
            0x00,
            0x00,
            0x70,
            0x88,
            0xF8,
            0x80,
            0x70,
        };
        return sprite_e;
    }

    case 'f': {
        static const char sprite_f[] = {
            0x30,
            0x40,
            0x40,
            0xF0,
            0x40,
            0x40,
            0x40,
        };
        return sprite_f;
    }

    case 'g': {
        static const char sprite_g[] = {
            0x00,
            0x78,
            0x88,
            0x88,
            0x78,
            0x08,
            0x70,
        };
        return sprite_g;
    }

    case 'h': {
        static const char sprite_h[] = {
            0x80,
            0x80,
            0x80,
            0xF0,
            0x88,
            0x88,
            0x88,
        };
        return sprite_h;
    }

    case 'i': {
        static const char sprite_i[] = {
            0x00,
            0x20,
            0x00,
            0x20,
            0x20,
            0x20,
            0x20,
        };
        return sprite_i;
    }

    case 'j': {
        static const char sprite_j[] = {
            0x00,
            0x10,
            0x00,
            0x10,
            0x10,
            0x50,
            0x30,
        };
        return sprite_j;
    }

    case 'k': {
        static const char sprite_k[] = {
            0x80,
            0x80,
            0x90,
            0xA0,
            0xC0,
            0xA0,
            0x90,
        };
        return sprite_k;
    }

    case 'l': {
        static const char sprite_l[] = {
            0x60,
            0x20,
            0x20,
            0x20,
            0x20,
            0x20,
            0x30,
        };
        return sprite_l;
    }

    case 'm': {
        static const char sprite_m[] = {
            0x00,
            0x00,
            0xF0,
            0xA8,
            0xA8,
            0xA8,
            0x88,
        };
        return sprite_m;
    }

    case 'n': {
        static const char sprite_n[] = {
            0x00,
            0x00,
            0xB0,
            0xC8,
            0x88,
            0x88,
            0x88,
        };
        return sprite_n;
    }

    case 'o': {
        static const char sprite_o[] = {
            0x00,
            0x00,
            0x70,
            0x88,
            0x88,
            0x88,
            0x70,
        };
        return sprite_o;
    }

    case 'p': {
        static const char sprite_p[] = {
            0x00,
            0x70,
            0x88,
            0x88,
            0xF0,
            0x80,
            0x80,
        };
        return sprite_p;
    }

    case 'q': {
        static const char sprite_q[] = {
            0x00,
            0x70,
            0x88,
            0x88,
            0x78,
            0x08,
            0x08,
        };
        return sprite_q;
    }

    case 'r': {
        static const char sprite_r[] = {
            0x00,
            0x00,
            0xB8,
            0xC0,
            0x80,
            0x80,
            0x80,
        };
        return sprite_r;
    }

    case 's': {
        static const char sprite_s[] = {
            0x00,
            0x00,
            0x78,
            0x80,
            0x70,
            0x08,
            0xF0,
        };
        return sprite_s;
    }

    case 't': {
        static const char sprite_t[] = {
            0x20,
            0x20,
            0x70,
            0x20,
            0x20,
            0x20,
            0x30,
        };
        return sprite_t;
    }

    case 'u': {
        static const char sprite_u[] = {
            0x00,
            0x00,
            0x88,
            0x88,
            0x88,
            0x88,
            0x78,
        };
        return sprite_u;
    }

    case 'v': {
        static const char sprite_v[] = {
            0x00,
            0x00,
            0x88,
            0x88,
            0x88,
            0x50,
            0x20,
        };
        return sprite_v;
    }

    case 'w': {
        static const char sprite_w[] = {
            0x00,
            0x00,
            0x88,
            0x88,
            0xA8,
            0xA8,
            0x50,
        };
        return sprite_w;
    }

    case 'A': {
        static const char sprite_A[] = {
            0x20,
            0x50,
            0x88,
            0xF8,
            0x88,
            0x88,
            0x88,
        };
        return sprite_A;
    }

    case 'B': {
        static const char sprite_B[] = {
            0xF0,
            0x88,
            0x88,
            0xF0,
            0x88,
            0x88,
            0xF8,
        };
        return sprite_B;
    }

    case 'C': {
        static const char sprite_C[] = {
            0x70,
            0x88,
            0x80,
            0x80,
            0x80,
            0x88,
            0x70,
        };
        return sprite_C;
    }

    case 'D': {
        static const char sprite_D[] = {
            0xF0,
            0x88,
            0x88,
            0x88,
            0x88,
            0x88,
            0xF0,
        };
        return sprite_D;
    }

    case 'E': {
        static const char sprite_E[] = {
            0xF8,
            0x80,
            0x80,
            0xF0,
            0x80,
            0x80,
            0xF8,
        };
        return sprite_E;
    }

    case 'F': {
        static const char sprite_F[] = {
            0xF8,
            0x80,
            0x80,
            0xF0,
            0x80,
            0x80,
            0x80,
        };
        return sprite_F;
    }

    case 'G': {
        static const char sprite_G[] = {
            0x70,
            0x88,
            0x80,
            0xB8,
            0x88,
            0x88,
            0x68,
        };
        return sprite_G;
    }

    case 'H': {
        static const char sprite_H[] = {
            0x88,
            0x88,
            0x88,
            0xF8,
            0x88,
            0x88,
            0x88,
        };
        return sprite_H;
    }

    case 'I': {
        static const char sprite_I[] = {
            0x70,
            0x20,
            0x20,
            0x20,
            0x20,
            0x20,
            0x70,
        };
        return sprite_I;
    }

    case 'J': {
        static const char sprite_J[] = {
            0xF8,
            0x20,
            0x20,
            0x20,
            0x20,
            0xA0,
            0x60,
        };
        return sprite_J;
    }

    case 'K': {
        static const char sprite_K[] = {
            0x88,
            0x90,
            0xA0,
            0xC0,
            0xA0,
            0x90,
            0x88,
        };
        return sprite_K;
    }

    case 'L': {
        static const char sprite_L[] = {
            0x80,
            0x80,
            0x80,
            0x80,
            0x80,
            0x80,
            0xF8,
        };
        return sprite_L;
    }

    case 'M': {
        static const char sprite_M[] = {
            0x88,
            0xD8,
            0xA8,
            0xA8,
            0x88,
            0x88,
            0x88,
        };
        return sprite_M;
    }

    case 'N': {
        static const char sprite_N[] = {
            0x88,
            0x88,
            0xC8,
            0xA8,
            0x98,
            0x88,
            0x88,
        };
        return sprite_N;
    }

    case 'O': {
        static const char sprite_O[] = {
            0x70,
            0x88,
            0x88,
            0x88,
            0x88,
            0x88,
            0x70,
        };
        return sprite_O;
    }

    case 'P': {
        static const char sprite_P[] = {
            0xF0,
            0x88,
            0x88,
            0xF0,
            0x80,
            0x80,
            0x80,
        };
        return sprite_P;
    }

    case 'Q': {
        static const char sprite_Q[] = {
            0x70,
            0x88,
            0x88,
            0x88,
            0xA8,
            0x90,
            0x68,
        };
        return sprite_Q;
    }

    case 'R': {
        static const char sprite_R[] = {
            0xF0,
            0x88,
            0x88,
            0xF0,
            0xA0,
            0x90,
            0x88,
        };
        return sprite_R;
    }

    case 'S': {
        static const char sprite_S[] = {
            0x78,
            0x80,
            0x80,
            0x70,
            0x08,
            0x08,
            0xF0,
        };
        return sprite_S;
    }

    case 'T': {
        static const char sprite_T[] = {
            0xF8,
            0x20,
            0x20,
            0x20,
            0x20,
            0x20,
            0x20,
        };
        return sprite_T;
    }

    case 'U': {
        static const char sprite_U[] = {
            0x88,
            0x88,
            0x88,
            0x88,
            0x88,
            0x88,
            0x70,
        };
        return sprite_U;
    }

    case 'V': {
        static const char sprite_V[] = {
            0x88,
            0x88,
            0x88,
            0x88,
            0x50,
            0x50,
            0x20,
        };
        return sprite_V;
    }

    case 'W': {
        static const char sprite_W[] = {
            0x88,
            0x88,
            0xA8,
            0xA8,
            0xA8,
            0xA8,
            0x50,
        };
        return sprite_W;
    }

    case 'X': {
        static const char sprite_X[] = {
            0x88,
            0x88,
            0x50,
            0x20,
            0x50,
            0x88,
            0x88,
        };
        return sprite_X;
    }

    case 'Y': {
        static const char sprite_Y[] = {
            0x88,
            0x88,
            0x88,
            0xF8,
            0x08,
            0x08,
            0xF0,
        };
        return sprite_Y;
    }

    case 'Z': {
        static const char sprite_Z[] = {
            0xF8,
            0x08,
            0x10,
            0x20,
            0x40,
            0x80,
            0xF8,
        };
        return sprite_Z;
    }

    case '(': {
        static const char sprite_parenthesis1[] = {
            0x30,
            0x40,
            0x40,
            0x40,
            0x40,
            0x40,
            0x30,
        };
        return sprite_parenthesis1;
    }

    case ')': {
        static const char sprite_parenthesis2[] = {
            0x60,
            0x10,
            0x10,
            0x10,
            0x10,
            0x10,
            0x60,
        };
        return sprite_parenthesis2;
    }

    case '^': {
        static const char sprite_caret[] = {
            0x20,
            0x50,
            0x88,
            0x00,
            0x00,
            0x00,
            0x00,
        };
        return sprite_caret;
    }

    default: {
        static const char sprite_none[] = {
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
