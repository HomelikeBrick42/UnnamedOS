#include "Bitmap.h"

void Bitmap_Create(Bitmap* bitmap, void* buffer, size_t size) {
	*bitmap = (Bitmap){
		.Buffer = buffer,
		.Size = size,
	};
}

uint8_t Bitmap_GetBit(Bitmap* bitmap, uint64_t index) {
	if (index > bitmap->Size) {
		return 0;
	}

	uint64_t byteIndex = index / 8;
	uint64_t bitIndex = index % 8;

	uint8_t byte = ((uint8_t*)bitmap->Buffer)[byteIndex];
	uint8_t bit = 0b10000000 >> bitIndex;

	return (byte & bit) != 0;
}

void Bitmap_SetBit(Bitmap* bitmap, uint64_t index, uint8_t value) {
	if (index > bitmap->Size) {
		return;
	}

	uint64_t byteIndex = index / 8;
	uint64_t bitIndex = index % 8;

	uint8_t* byte = &((uint8_t*)bitmap->Buffer)[byteIndex];
	uint8_t bit = 0b10000000 >> bitIndex;

	*byte &= ~bit;

	if (value) {
		*byte |= bit;
	}
}
