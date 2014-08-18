// defines 5x7 ascii characters 0x20-0x7F (32-127)
#include "Arduino.h"
#include <avr/pgmspace.h>

PROGMEM const char font5x7[][5]  = {
        {0x00,0x00,0x00,0x00,0x00}, /*space*/ // is 32 in ASCII
        {0x00,0xF6,0xF6,0x00,0x00}, /*!*/
        {0x00,0xE0,0x00,0xE0,0x00}, /*"*/
        {0x28,0xFE,0x28,0xFE,0x28}, /*#*/
        {0x00,0x64,0xD6,0x54,0x08}, /*$*/
        {0xC2,0xCC,0x10,0x26,0xC6}, /*%*/
        {0x4C,0xB2,0x92,0x6C,0x0A}, /*&*/
        {0x00,0x00,0xE0,0x00,0x00}, /*'*/
        {0x00,0x38,0x44,0x82,0x00}, /*(*/
        {0x00,0x82,0x44,0x38,0x00}, /*)*/
        {0x88,0x50,0xF8,0x50,0x88}, /***/
        {0x08,0x08,0x3E,0x08,0x08}, /*+*/
        {0x00,0x00,0x05,0x06,0x00}, /*,*/
        {0x08,0x08,0x08,0x08,0x08}, /*-*/
        {0x00,0x00,0x06,0x06,0x00}, /*.*/
        {0x02,0x0C,0x10,0x60,0x80}, /*/*/
        {0x7c,0x8a,0x92,0xa2,0x7c}, // 0
        {0x00,0x42,0xfe,0x02,0x00}, // 1
        {0x42,0x86,0x8a,0x92,0x62}, // 2
        {0x84,0x82,0xa2,0xd2,0x8c}, // 3
        {0x18,0x28,0x48,0xfe,0x08}, // 4
        {0xe4,0xa2,0xa2,0xa2,0x9c}, // 5
        {0x3c,0x52,0x92,0x92,0x0c}, // 6
        {0x80,0x8e,0x90,0xa0,0xc0}, // 7
        {0x6c,0x92,0x92,0x92,0x6c}, // 8
        {0x60,0x92,0x92,0x94,0x78}, // 9
        {0x00,0x6c,0x6c,0x00,0x00}, // :
        {0x00,0x6a,0x6c,0x00,0x00}, // ;
        {0x00,0x10,0x28,0x44,0x82}, // <
        {0x28,0x28,0x28,0x28,0x28}, // =
        {0x82,0x44,0x28,0x10,0x00}, // >
        {0x40,0x80,0x8a,0x90,0x60}, // ?
        {0x4c,0x92,0x9e,0x82,0x7c}, // @
        {0x7e,0x88,0x88,0x88,0x7e}, // A
        {0xfe,0x92,0x92,0x92,0x6c}, // B
        {0x7c,0x82,0x82,0x82,0x44}, // C
        {0xfe,0x82,0x82,0x44,0x38}, // D
        {0xfe,0x92,0x92,0x92,0x82}, // E
        {0xfe,0x90,0x90,0x80,0x80}, // F
        {0x7c,0x82,0x82,0x8a,0x4c}, // G
        {0xfe,0x10,0x10,0x10,0xfe}, // H
        {0x00,0x82,0xfe,0x82,0x00}, // I
        {0x04,0x02,0x82,0xfc,0x80}, // J
        {0xfe,0x10,0x28,0x44,0x82}, // K
        {0xfe,0x02,0x02,0x02,0x02}, // L
        {0xfe,0x40,0x20,0x40,0xfe}, // M
        {0xfe,0x20,0x10,0x08,0xfe}, // N
        {0x7c,0x82,0x82,0x82,0x7c}, // O
        {0xfe,0x90,0x90,0x90,0x60}, // P
        {0x7c,0x82,0x8a,0x84,0x7a}, // Q
        {0xfe,0x90,0x98,0x94,0x62}, // R
        {0x62,0x92,0x92,0x92,0x8c}, // S
        {0x80,0x80,0xfe,0x80,0x80}, // T
        {0xfc,0x02,0x02,0x02,0xfc}, // U
        {0xf8,0x04,0x02,0x04,0xf8}, // V
        {0xfe,0x04,0x18,0x04,0xfe}, // W
        {0xc6,0x28,0x10,0x28,0xc6}, // X
        {0xc0,0x20,0x1e,0x20,0xc0}, // Y
        {0x86,0x8a,0x92,0xa2,0xc2}, // Z
        {0x00,0x00,0xfe,0x82,0x82}, // [
        {0x40,0x20,0x10,0x08,0x04}, // "\"
        {0x82,0x82,0xfe,0x00,0x00}, // ]
        {0x20,0x40,0x80,0x40,0x20}, // ^
        {0x02,0x02,0x02,0x02,0x02}, // _
        {0x00,0x80,0x40,0x20,0x00}, // `
        {0x04,0x2a,0x2a,0x2a,0x1e}, // a
        {0xfe,0x12,0x22,0x22,0x1c}, // b
        {0x1c,0x22,0x22,0x22,0x04}, // c
        {0x1c,0x22,0x22,0x12,0xfe}, // d
        {0x1c,0x2a,0x2a,0x2a,0x18}, // e
        {0x10,0x7e,0x90,0x80,0x40}, // f
        {0x10,0x28,0x2a,0x2a,0x3c}, // g
        {0xfe,0x10,0x20,0x20,0x1e}, // h
        {0x00,0x22,0xbe,0x02,0x00}, // i
        {0x04,0x02,0x22,0xbc,0x00}, // j
        {0x00,0xfe,0x08,0x14,0x22}, // k
        {0x00,0x82,0xfe,0x02,0x00}, // l
        {0x3e,0x20,0x18,0x20,0x1e}, // m
        {0x3e,0x10,0x20,0x20,0x1e}, // n
        {0x1c,0x22,0x22,0x22,0x1c}, // o
        {0x3e,0x28,0x28,0x28,0x10}, // p
        {0x10,0x28,0x28,0x18,0x3e}, // q
        {0x3e,0x10,0x20,0x20,0x10}, // r
        {0x12,0x2a,0x2a,0x2a,0x04}, // s
        {0x20,0xfc,0x22,0x02,0x04}, // t
        {0x3c,0x02,0x02,0x04,0x3e}, // u
        {0x38,0x04,0x02,0x04,0x38}, // v
        {0x3c,0x02,0x0c,0x02,0x3c}, // w
        {0x22,0x14,0x08,0x14,0x22}, // x
        {0x30,0x0a,0x0a,0x0a,0x3c}, // y
        {0x22,0x26,0x2a,0x32,0x22}, // z
        {0x00,0x10,0x6c,0x82,0x00}, // {
        {0x00,0x00,0xfe,0x00,0x00}, // |
        {0x00,0x82,0x6c,0x10,0x00}, // }
        {0x40,0x80,0xc0,0x40,0x80}, // ~
};
