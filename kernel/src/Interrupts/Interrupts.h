#pragma once

#include "Common.h"

#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1
#define PIC_EOI      0x20

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x01
#define ICW4_8086 0x01

typedef struct InteruptFrame InteruptFrame;

// TODO: Find some way to make gcc stop ignoring __attribute__((interupt))
__attribute__((interrupt)) void PageFault_Handler(InteruptFrame* interuptFrame);
__attribute__((interrupt)) void DoubleFault_Handler(InteruptFrame* interuptFrame);
__attribute__((interrupt)) void GeneralProtectionFault_Handler(InteruptFrame* interuptFrame);
__attribute__((interrupt)) void KeyboardInt_Handler(InteruptFrame* interuptFrame);

void PIC_Remap(void);
void PIC_EndMaster(void);
void PIC_EndSlave(void);
