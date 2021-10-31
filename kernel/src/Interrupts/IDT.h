#pragma once

#include "Common.h"

#define IDT_TA_InteruptGate 0b10001110
#define IDT_TA_CallGate     0b10001100
#define IDT_TA_TrapGate     0b10001111

typedef struct IDTDescriptorEntry {
	uint16_t Offset0;
	uint16_t Selector;
	uint8_t IST;
	uint8_t Type_Attributes;
	uint16_t Offset1;
	uint32_t Offset2;
	uint32_t Ignore;
} IDTDescriptorEntry;

void IDTDescriptorEntry_SetOffset(IDTDescriptorEntry* idtDescriptorEntry, uint64_t offset);
uint64_t IDTDescriptorEntry_GetOffset(IDTDescriptorEntry* idtDescriptorEntry);

typedef struct IDTR {
	uint16_t Limit;
	uint64_t Offset;
} __attribute__((packed)) IDTR;
