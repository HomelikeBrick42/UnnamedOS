#include "Framebuffer.h"

Framebuffer* GlobalFramebuffer = NULL;

void Framebuffer_PutPixel(Framebuffer* framebuffer, uint32_t x, uint32_t y, uint32_t color) {
	if (x > framebuffer->Width || y > framebuffer->Height) {
		return;
	}

	const uint32_t BBP = 4;
	*(uint32_t*)(x * BBP + (y * framebuffer->PixelsPerScanLine * BBP) + framebuffer->BaseAddress) = color;
}

void Framebuffer_DrawQuad(Framebuffer* framebuffer, uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color) {
	uint32_t* line = (uint32_t*)framebuffer->BaseAddress + x + y * framebuffer->PixelsPerScanLine;

	for (uint32_t yOff = 0; yOff < height; yOff++) {
		if (y + yOff > framebuffer->Height) {
			break;
		}

		for (uint32_t xOff = 0; xOff < width; xOff++) {
			if (x + xOff > framebuffer->Width) {
				break;
			}

			line[xOff] = color;
		}

		line += framebuffer->PixelsPerScanLine;
	}
}

void Framebuffer_ClearScreen(Framebuffer* framebuffer, uint32_t color) {
	uint32_t* line = framebuffer->BaseAddress;
	for (uint32_t y = 0; y < framebuffer->Height; y++) {
		for (uint32_t x = 0; x < framebuffer->Width; x++) {
			line[x] = color;
		}
		line += framebuffer->PixelsPerScanLine;
	}
}
