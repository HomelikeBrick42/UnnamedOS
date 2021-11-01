#include "Renderer.h"

uint32_t GlobalBaseCursorX = 0;
uint32_t GlobalCursorX = 0;
uint32_t GlobalCursorY = 0;

static Framebuffer* TargetFramebuffer = NULL;
static PSF1_Font* Font = NULL;

void Renderer_Init(Framebuffer* framebuffer, PSF1_Font* font) {
	TargetFramebuffer = framebuffer;
	Font = font;
}

uint32_t Renderer_GetWidth(void) {
	return TargetFramebuffer->Width;
}

uint32_t Renderer_GetHeight(void) {
	return TargetFramebuffer->Height;
}

uint32_t Renderer_GetCharacterWidth(void) {
	return 8;
}

uint32_t Renderer_GetCharacterHeight(void) {
	return Font->Header->CharSize;
}

void Renderer_SwapBuffers(void) {
	// TODO: Make backbuffer
}

void Renderer_Clear(uint32_t color) {
	uint32_t* line = TargetFramebuffer->BaseAddress;
	for (uint32_t y = 0; y < TargetFramebuffer->Height; y++) {
		for (uint32_t x = 0; x < TargetFramebuffer->Width; x++) {
			line[x] = color;
		}
		line += TargetFramebuffer->PixelsPerScanLine;
	}
}

void Renderer_DrawPixel(uint32_t x, uint32_t y, uint32_t color) {
	if (x > TargetFramebuffer->Width || y > TargetFramebuffer->Height) {
		return;
	}
	*((uint32_t*)TargetFramebuffer->BaseAddress + x + y * TargetFramebuffer->PixelsPerScanLine) = color;
}

void Renderer_DrawCharacter(char character, uint32_t baseX, uint32_t* x, uint32_t* y, uint32_t color) {
	switch (character) {
		case '\r': {
			*x = baseX;
		} break;

		case '\n': {
			*y += Font->Header->CharSize;
		} break;

		default: {
			char* fontPtr = (char*)Font->GlyphBuffer + ((uint64_t)character * (uint64_t)Font->Header->CharSize);
			for (uint32_t yOff = 0; yOff < Font->Header->CharSize; yOff++) {
				for (uint32_t xOff = 0; xOff < 8; xOff++) {
					if ((*fontPtr) & (0b10000000 >> xOff)) {
						Renderer_DrawPixel(*x + xOff, *y + yOff, color);
					}
				}
				fontPtr++;
			}
			*x += 8;
		} break;
	}
}

void Renderer_DrawString(const char* string, uint32_t baseX, uint32_t* x, uint32_t* y, uint32_t color) {
	if (!string) {
		return;
	}

	while (*string) {
		Renderer_DrawCharacter(*string, baseX, x, y, color);
		string++;
	}
}

void Renderer_DrawUInt(uint64_t value, uint32_t* x, uint32_t* y, uint32_t color) {
	uint64_t temp = value;

	uint8_t stringSize = 0;
	while (temp / 10) {
		temp /= 10;
		stringSize++;
	}

	char buffer[stringSize + 2];

	uint8_t index = 0;
	while (value / 10) {
		uint8_t remainder = value % 10;
		value /= 10;

		buffer[stringSize - index] = remainder + '0';
		index++;
	}

	uint8_t remainder = value % 10;
	buffer[stringSize - index] = remainder + '0';

	buffer[stringSize + 1] = '\0';

	Renderer_DrawString(buffer, *x, x, y, color);
}

void Renderer_DrawInt(int64_t value, uint32_t* x, uint32_t* y, uint32_t color) {
	if (value < 0) {
		Renderer_DrawCharacter('-', *x, x, y, color);
		value *= -1;
	}
	Renderer_DrawUInt((uint64_t)value, x, y, color);
}

void Renderer_DrawQuad(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color) {
	uint32_t* line = (uint32_t*)TargetFramebuffer->BaseAddress + x + y * TargetFramebuffer->PixelsPerScanLine;

	for (uint32_t yOff = 0; yOff < height; yOff++) {
		if (y + yOff > TargetFramebuffer->Height) {
			break;
		}

		for (uint32_t xOff = 0; xOff < width; xOff++) {
			if (x + xOff > TargetFramebuffer->Width) {
				break;
			}

			line[xOff] = color;
		}

		line += TargetFramebuffer->PixelsPerScanLine;
	}
}
