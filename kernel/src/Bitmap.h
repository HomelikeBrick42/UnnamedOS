#pragma once

#include <stdint.h>
#include <stddef.h>

typedef struct Bitmap {
	void* Buffer;
	size_t Size; // NOTE: This size is in bits
} Bitmap;

void Bitmap_Create(Bitmap* bitmap, void* buffer, size_t size);
uint8_t Bitmap_GetBit(Bitmap* bitmap, uint64_t index);
void Bitmap_SetBit(Bitmap* bitmap, uint64_t index, uint8_t value);
