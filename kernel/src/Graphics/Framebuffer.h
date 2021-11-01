#pragma once

#include "Common.h"

// TODO: Include the pixel format in this struct
// or set the pixel format in the bootloader
typedef struct Framebuffer {
	void* BaseAddress;
	size_t BufferSize;
	uint32_t Width;
	uint32_t Height;
	uint32_t PixelsPerScanLine;
} Framebuffer;
