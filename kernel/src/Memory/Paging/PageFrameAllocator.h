#pragma once

#include "Memory/EfiMemory.h"

#include <stdint.h>
#include <stddef.h>

uint8_t PageFrameAllocator_Init(EfiMemoryDescriptor* memoryMapDescriptors, uint64_t memoryMapSize, uint64_t memoryMapDescriptorSize);
uint64_t PageFrameAllocator_GetMemorySize(void);
uint64_t PageFrameAllocator_GetFreeMemory(void);
uint64_t PageFrameAllocator_GetUsedMemory(void);
uint64_t PageFrameAllocator_GetReservedMemory(void);
void* PageFrameAllocator_RequestPage(void);
void PageFrameAllocator_FreePage(void* address);
void PageFrameAllocator_FreePages(void* address, uint64_t count);
void PageFrameAllocator_LockPage(void* address);
void PageFrameAllocator_LockPages(void* address, uint64_t count);
