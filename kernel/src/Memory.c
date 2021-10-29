#include "Memory.h"

uint64_t GetMemorySize(EfiMemoryDescriptor* memoryMapDescriptors, uint64_t memoryMapEntries, uint64_t memoryMapDescriptorSize) {
	static uint64_t MemorySizeBytes = 0;
	if (MemorySizeBytes > 0) {
		return MemorySizeBytes;
	}

	for (uint64_t i = 0; i < memoryMapEntries; i++) {
		EfiMemoryDescriptor* descriptor = (EfiMemoryDescriptor*)((uint64_t)memoryMapDescriptors + i * memoryMapDescriptorSize);
		MemorySizeBytes += descriptor->NumPages * 4096;
	}

	return MemorySizeBytes;
}
