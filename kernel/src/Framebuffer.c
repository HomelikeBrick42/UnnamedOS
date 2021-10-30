#include "Framebuffer.h"

Framebuffer* GlobalFramebuffer = NULL;

void PutPixel(Framebuffer* framebuffer, uint32_t x, uint32_t y, uint32_t color) {
	if (x > framebuffer->Width || y > framebuffer->Height) {
		return;
	}

	const uint32_t BBP = 4;
	*(uint32_t*)(x * BBP + (y * framebuffer->PixelsPerScanLine * BBP) + framebuffer->BaseAddress) = color;
}

void ClearScreen(Framebuffer* framebuffer, uint32_t color) {
	uint32_t* line = framebuffer->BaseAddress;
	for (uint32_t y = 0; y < framebuffer->Height; y++) {
		for (uint32_t x = 0; x < framebuffer->Width; x++) {
			line[x] = color;
		}
		for (uint32_t x = 0; x < framebuffer->PixelsPerScanLine - framebuffer->Width; x++) {
			line[x] = 0x00000000;
		}
		line += framebuffer->PixelsPerScanLine;
	}
}
