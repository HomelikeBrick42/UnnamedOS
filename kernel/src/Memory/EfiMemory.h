#pragma once

#include "Common.h"

typedef enum EfiMemoryType {
	EfiMemoryType_ReservedMemory,
	EfiMemoryType_LoaderCode,
	EfiMemoryType_LoaderData,
	EfiMemoryType_BootServicesCode,
	EfiMemoryType_BootServicesData,
	EfiMemoryType_RuntimeServicesCode,
	EfiMemoryType_RuntimeServicesData,
	EfiMemoryType_ConventionalMemory,
	EfiMemoryType_UnusableMemory,
	EfiMemoryType_ACPIReclaimMemory,
	EfiMemoryType_ACPIMemoryNVS,
	EfiMemoryType_MemoryMappedIO,
	EfiMemoryType_MemoryMappedIOPortSpace,
	EfiMemoryType_PalCode,
	EfiMemoryType_Count,
} EfiMemoryType;

typedef struct EfiMemoryDescriptor {
	uint32_t Type;
	uint32_t Padding;
	void* PhysicalAddress;
	void* VirtualAddress;
	uint64_t NumPages;
	uint64_t Attribs;
} EfiMemoryDescriptor;

extern const char* EfiMemoryTypeNames[EfiMemoryType_Count];
