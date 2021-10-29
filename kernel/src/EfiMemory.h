#pragma once

#include <stdint.h>
#include <stddef.h>

typedef struct EfiMemoryDescriptor {
	uint32_t Type;
	uint32_t Padding;
	void* PhysicalAddress;
	void* VirtualAddress;
	uint64_t NumPages;
	uint64_t Attribs;
} EfiMemoryDescriptor;

extern const char* EfiMemoryTypeNames[];
