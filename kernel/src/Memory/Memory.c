#include "Memory.h"

void SetMemory(void* block, uint8_t value, uint64_t size) {
	for (uint64_t i = 0; i < size; i++) {
		((uint8_t*)block)[i] = value;
	}
}

void CopyMemory(void* dest, const void* src, uint64_t size) {
	if (dest < src) {
		for (uint64_t i = 0; i < size; i++) {
			((uint8_t*)dest)[i] = ((const uint8_t*)src)[i];
		}
	} else {
		for (uint64_t i = 0; i < size; i++) {
			((uint8_t*)dest)[size - i - 1] = ((const uint8_t*)src)[size - i - 1];
		}
	}
}
