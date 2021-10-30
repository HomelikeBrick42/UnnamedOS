#pragma once

#include <stdint.h>
#include <stddef.h>

typedef struct GDTDescriptor {
	uint16_t Size;
	uint64_t Offset;
} __attribute__((packed)) GDTDescriptor;

typedef struct GDTEntry {
	uint16_t Limit0;
	uint16_t Base0;
	uint8_t Base1;
	uint8_t AccessByte;
	uint8_t Limit1_Flags;
	uint8_t Base2;
} __attribute__((packed)) GDTEntry;

typedef struct GDT {
	GDTEntry Null;
	GDTEntry KernelCode;
	GDTEntry KernelData;
	GDTEntry UserNull;
	GDTEntry UserCode;
	GDTEntry UserData;
} __attribute__((packed, aligned(0x1000))) GDT;

extern void LoadGDT(GDTDescriptor* gdtDescriptor);
extern GDT DefaultGDT;
