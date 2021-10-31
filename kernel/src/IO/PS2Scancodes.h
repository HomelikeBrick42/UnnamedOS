#pragma once

#include "Common.h"

typedef enum PS2Scancode {
	PS2Scancode_Pressed_Escape              = 0x01,
	PS2Scancode_Pressed_1                   = 0x02,
	PS2Scancode_Pressed_2                   = 0x03,
	PS2Scancode_Pressed_3                   = 0x04,
	PS2Scancode_Pressed_4                   = 0x05,
	PS2Scancode_Pressed_5                   = 0x06,
	PS2Scancode_Pressed_6                   = 0x07,
	PS2Scancode_Pressed_7                   = 0x08,
	PS2Scancode_Pressed_8                   = 0x09,
	PS2Scancode_Pressed_9                   = 0x0A,
	PS2Scancode_Pressed_0                   = 0x0B,
	PS2Scancode_Pressed_Minus               = 0x0C,
	PS2Scancode_Pressed_Equals              = 0x0D,
	PS2Scancode_Pressed_Backspace           = 0x0E,
	PS2Scancode_Pressed_Tab                 = 0x0F,
	PS2Scancode_Pressed_Q                   = 0x10,
	PS2Scancode_Pressed_W                   = 0x11,
	PS2Scancode_Pressed_E                   = 0x12,
	PS2Scancode_Pressed_R                   = 0x13,
	PS2Scancode_Pressed_T                   = 0x14,
	PS2Scancode_Pressed_Y                   = 0x15,
	PS2Scancode_Pressed_U                   = 0x16,
	PS2Scancode_Pressed_I                   = 0x17,
	PS2Scancode_Pressed_O                   = 0x18,
	PS2Scancode_Pressed_P                   = 0x19,
	PS2Scancode_Pressed_OpenSquareBracket   = 0x1A,
	PS2Scancode_Pressed_CloseSquareBracket  = 0x1B,
	PS2Scancode_Pressed_Enter               = 0x1C,
	PS2Scancode_Pressed_LeftControl         = 0x1D,
	PS2Scancode_Pressed_A                   = 0x1E,
	PS2Scancode_Pressed_S                   = 0x1F,
	PS2Scancode_Pressed_D                   = 0x20,
	PS2Scancode_Pressed_F                   = 0x21,
	PS2Scancode_Pressed_G                   = 0x22,
	PS2Scancode_Pressed_H                   = 0x23,
	PS2Scancode_Pressed_J                   = 0x24,
	PS2Scancode_Pressed_K                   = 0x25,
	PS2Scancode_Pressed_L                   = 0x26,
	PS2Scancode_Pressed_Semicolon           = 0x27,
	PS2Scancode_Pressed_SingleQuote         = 0x28,
	PS2Scancode_Pressed_BackTick            = 0x29,
	PS2Scancode_Pressed_LeftShift           = 0x2A,
	PS2Scancode_Pressed_Backslash           = 0x2B,
	PS2Scancode_Pressed_Z                   = 0x2C,
	PS2Scancode_Pressed_X                   = 0x2D,
	PS2Scancode_Pressed_C                   = 0x2E,
	PS2Scancode_Pressed_V                   = 0x2F,
	PS2Scancode_Pressed_B                   = 0x30,
	PS2Scancode_Pressed_N                   = 0x31,
	PS2Scancode_Pressed_M                   = 0x32,
	PS2Scancode_Pressed_Comma               = 0x33,
	PS2Scancode_Pressed_Period              = 0x34,
	PS2Scancode_Pressed_Slash               = 0x35,
	PS2Scancode_Pressed_RightShift          = 0x36,
	PS2Scancode_Pressed_Keypad_Asterisk     = 0x37,
	PS2Scancode_Pressed_LeftAlt             = 0x38,
	PS2Scancode_Pressed_Space               = 0x39,
	PS2Scancode_Pressed_CapsLock            = 0x3A,
	PS2Scancode_Pressed_F1                  = 0x3B,
	PS2Scancode_Pressed_F2                  = 0x3C,
	PS2Scancode_Pressed_F3                  = 0x3D,
	PS2Scancode_Pressed_F4                  = 0x3E,
	PS2Scancode_Pressed_F5                  = 0x3F,
	PS2Scancode_Pressed_F6                  = 0x40,
	PS2Scancode_Pressed_F7                  = 0x41,
	PS2Scancode_Pressed_F8                  = 0x42,
	PS2Scancode_Pressed_F9                  = 0x43,
	PS2Scancode_Pressed_F10                 = 0x44,
	PS2Scancode_Pressed_NumLock             = 0x45,
	PS2Scancode_Pressed_ScrollLock          = 0x46,
	PS2Scancode_Pressed_Keypad_7            = 0x47,
	PS2Scancode_Pressed_Keypad_8            = 0x48,
	PS2Scancode_Pressed_Keypad_9            = 0x49,
	PS2Scancode_Pressed_Keypad_Minus        = 0x4A,
	PS2Scancode_Pressed_Keypad_4            = 0x4B,
	PS2Scancode_Pressed_Keypad_5            = 0x4C,
	PS2Scancode_Pressed_Keypad_6            = 0x4D,
	PS2Scancode_Pressed_Keypad_Plus         = 0x4E,
	PS2Scancode_Pressed_Keypad_1            = 0x4F,
	PS2Scancode_Pressed_Keypad_2            = 0x50,
	PS2Scancode_Pressed_Keypad_3            = 0x51,
	PS2Scancode_Pressed_Keypad_0            = 0x52,
	PS2Scancode_Pressed_Keypad_Period       = 0x53,
	PS2Scancode_Pressed_F11                 = 0x57,
	PS2Scancode_Pressed_F12                 = 0x58,
	PS2Scancode_Released_Escape             = 0x81,
	PS2Scancode_Released_1                  = 0x82,
	PS2Scancode_Released_2                  = 0x83,
	PS2Scancode_Released_3                  = 0x84,
	PS2Scancode_Released_4                  = 0x85,
	PS2Scancode_Released_5                  = 0x86,
	PS2Scancode_Released_6                  = 0x87,
	PS2Scancode_Released_7                  = 0x88,
	PS2Scancode_Released_8                  = 0x89,
	PS2Scancode_Released_9                  = 0x8A,
	PS2Scancode_Released_0                  = 0x8B,
	PS2Scancode_Released_Minus              = 0x8C,
	PS2Scancode_Released_Equals             = 0x8D,
	PS2Scancode_Released_Backspace          = 0x8E,
	PS2Scancode_Released_Tab                = 0x8F,
	PS2Scancode_Released_Q                  = 0x90,
	PS2Scancode_Released_W                  = 0x91,
	PS2Scancode_Released_E                  = 0x92,
	PS2Scancode_Released_R                  = 0x93,
	PS2Scancode_Released_T                  = 0x94,
	PS2Scancode_Released_Y                  = 0x95,
	PS2Scancode_Released_U                  = 0x96,
	PS2Scancode_Released_I                  = 0x97,
	PS2Scancode_Released_O                  = 0x98,
	PS2Scancode_Released_P                  = 0x99,
	PS2Scancode_Released_OpenSquareBracket  = 0x9A,
	PS2Scancode_Released_CloseSquareBracket = 0x9B,
	PS2Scancode_Released_Enter              = 0x9C,
	PS2Scancode_Released_LeftControl        = 0x9D,
	PS2Scancode_Released_A                  = 0x9E,
	PS2Scancode_Released_S                  = 0x9F,
	PS2Scancode_Released_D                  = 0xA0,
	PS2Scancode_Released_F                  = 0xA1,
	PS2Scancode_Released_G                  = 0xA2,
	PS2Scancode_Released_H                  = 0xA3,
	PS2Scancode_Released_J                  = 0xA4,
	PS2Scancode_Released_K                  = 0xA5,
	PS2Scancode_Released_L                  = 0xA6,
	PS2Scancode_Released_Semicolon          = 0xA7,
	PS2Scancode_Released_SingleQuote        = 0xA8,
	PS2Scancode_Released_BackTick           = 0xA9,
	PS2Scancode_Released_LeftShift          = 0xAA,
	PS2Scancode_Released_Backslash          = 0xAB,
	PS2Scancode_Released_Z                  = 0xAC,
	PS2Scancode_Released_X                  = 0xAD,
	PS2Scancode_Released_C                  = 0xAE,
	PS2Scancode_Released_V                  = 0xAF,
	PS2Scancode_Released_B                  = 0xB0,
	PS2Scancode_Released_N                  = 0xB1,
	PS2Scancode_Released_M                  = 0xB2,
	PS2Scancode_Released_Comma              = 0xB3,
	PS2Scancode_Released_Period             = 0xB4,
	PS2Scancode_Released_Slash              = 0xB5,
	PS2Scancode_Released_RightShift         = 0xB6,
	PS2Scancode_Released_Keypad_Asterisk    = 0xB7,
	PS2Scancode_Released_LeftAlt            = 0xB8,
	PS2Scancode_Released_Space              = 0xB9,
	PS2Scancode_Released_CapsLock           = 0xBA,
	PS2Scancode_Released_F1                 = 0xBB,
	PS2Scancode_Released_F2                 = 0xBC,
	PS2Scancode_Released_F3                 = 0xBD,
	PS2Scancode_Released_F4                 = 0xBE,
	PS2Scancode_Released_F5                 = 0xBF,
	PS2Scancode_Released_F6                 = 0xC0,
	PS2Scancode_Released_F7                 = 0xC1,
	PS2Scancode_Released_F8                 = 0xC2,
	PS2Scancode_Released_F9                 = 0xC3,
	PS2Scancode_Released_F10                = 0xC4,
	PS2Scancode_Released_NumLock            = 0xC5,
	PS2Scancode_Released_ScrollLock         = 0xC6,
	PS2Scancode_Released_Keypad_7           = 0xC7,
	PS2Scancode_Released_Keypad_8           = 0xC8,
	PS2Scancode_Released_Keypad_9           = 0xC9,
	PS2Scancode_Released_Keypad_Minus       = 0xCA,
	PS2Scancode_Released_Keypad_4           = 0xCB,
	PS2Scancode_Released_Keypad_5           = 0xCC,
	PS2Scancode_Released_Keypad_6           = 0xCD,
	PS2Scancode_Released_Keypad_Plus        = 0xCE,
	PS2Scancode_Released_Keypad_1           = 0xCF,
	PS2Scancode_Released_Keypad_2           = 0xD0,
	PS2Scancode_Released_Keypad_3           = 0xD1,
	PS2Scancode_Released_Keypad_0           = 0xD2,
	PS2Scancode_Released_Keypad_Period      = 0xD3,
	PS2Scancode_Released_F11                = 0xD7,
	PS2Scancode_Released_F12                = 0xD8,
} PS2Scancode;

extern const char* PS2Scancode_Names[];
extern const char PS2Scancode_LowercaseASCII[];
extern const char PS2Scancode_UppercaseASCII[];