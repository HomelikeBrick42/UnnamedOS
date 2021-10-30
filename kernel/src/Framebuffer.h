#pragma once

#include <stdint.h>
#include <stddef.h>

// TODO: Include the pixel format in this struct
typedef struct Framebuffer {
	void* BaseAddress;
	size_t BufferSize;
	uint32_t Width;
	uint32_t Height;
	uint32_t PixelsPerScanLine;
} Framebuffer;

void PutPixel(Framebuffer* framebuffer, uint32_t x, uint32_t y, uint32_t color);
void ClearScreen(Framebuffer* framebuffer, uint32_t color);

extern Framebuffer* GlobalFramebuffer;
