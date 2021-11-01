#pragma once

#include "Common.h"

#define PS2_Button_Left   0b00000001
#define PS2_Button_Middle 0b00000010
#define PS2_Button_Right  0b00000100
#define PS2_X_Sign        0b00010000
#define PS2_Y_Sign        0b00100000
#define PS2_X_Overflow    0b01000000
#define PS2_Y_Overflow    0b10000000

void PS2Mouse_Init(void);
void PS2Mouse_Handle(uint8_t data);
void PS2Mouse_ProcessPacket(void);
void PS2Mouse_Write(uint8_t value);
uint8_t PS2Mouse_Read(void);
void PS2Mouse_Wait(void);
void PS2Mouse_WaitInput(void);

extern int32_t MouseX;
extern int32_t MouseY;
