#pragma once

#include "Common.h"

#include "Memory/EfiMemory.h"

void SetMemory(void* block, uint8_t value, uint64_t size);
void CopyMemory(void* dest, const void* src, uint64_t size);
