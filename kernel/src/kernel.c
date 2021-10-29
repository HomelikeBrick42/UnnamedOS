#include <stdint.h>
#include <stddef.h>

typedef struct Framebuffer {
	void* BaseAddress;
	size_t BufferSize;
	uint32_t Width;
	uint32_t Height;
	uint32_t PixelsPerScanLine;
} Framebuffer;

#define PSF1_MAGIC0 0x36
#define PSF1_MAGIC1 0x04

typedef struct PSF1_Header {
	uint8_t Magic[2];
	uint8_t Mode;
	uint8_t CharSize;
} PSF1_Header;

typedef struct PSF1_Font {
	PSF1_Header* Header;
	void* GlyphBuffer;
} PSF1_Font;

void PutPixel(Framebuffer* framebuffer, uint32_t x, uint32_t y, uint32_t color) {
	if (x > framebuffer.Width || y > framebuffer.Height) {
		return;
	}

	const uint32_t BBP = 4;
	*(uint32_t*)(x * BBP + (y * framebuffer->PixelsPerScanLine * BBP) + framebuffer->BaseAddress) = color;
}

void PutChar(Framebuffer* framebuffer, PSF1_Font* font, uint32_t x, uint32_t y, uint32_t color, char chr) {
	char* fontPtr = (char*)font->GlyphBuffer + ((uint64_t)chr * (uint64_t)font->Header->CharSize);
	for (uint32_t yOff = 0; yOff < font->Header->CharSize; yOff++) {
		for (uint32_t xOff = 0; xOff < 8; xOff++) {
			if ((*fontPtr) & (0b10000000 >> xOff)) {
				PutPixel(framebuffer, x + xOff, y + yOff, color);
			}
		}
		fontPtr++;
	}
}

void PutString(Framebuffer* framebuffer, PSF1_Font* font, uint32_t x, uint32_t y, uint32_t color, const char* string) {
	while (*string) {
		PutChar(framebuffer, font, x, y, color, *string);
		x += 8;
		string++;
	}
}

void _start(Framebuffer* framebuffer, PSF1_Font* font) {
	PutString(framebuffer, font, 10, 10, 0xFFFFFFFF, "Hello, This is some text");
	while (1);
}
