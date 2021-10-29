#pragma once

#include "EfiMemory.h"

#include <stdint.h>
#include <stddef.h>

uint64_t GetMemorySize(EfiMemoryDescriptor* memoryMapDescriptors, uint64_t memoryMapEntries, uint64_t memoryMapDescriptorSize);
