#include "Memory.h"

void SetMemory(void* block, uint8_t value, uint64_t size) {
	for (uint64_t i = 0; i < size; i++) {
		((uint8_t*)block)[i] = value;
	}
}
