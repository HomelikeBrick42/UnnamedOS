#pragma once

#include "Common.h"

typedef struct Bitmap {
	void* Buffer;
	size_t Size; // NOTE: This size is in bits
} Bitmap;

void Bitmap_Create(Bitmap* bitmap, void* buffer, size_t size);
bool Bitmap_GetBit(Bitmap* bitmap, uint64_t index);
bool Bitmap_SetBit(Bitmap* bitmap, uint64_t index, bool value);
