#include "Framebuffer.h"

void PutPixel(Framebuffer* framebuffer, uint32_t x, uint32_t y, uint32_t color) {
	if (x > framebuffer->Width || y > framebuffer->Height) {
		return;
	}

	const uint32_t BBP = 4;
	*(uint32_t*)(x * BBP + (y * framebuffer->PixelsPerScanLine * BBP) + framebuffer->BaseAddress) = color;
}
