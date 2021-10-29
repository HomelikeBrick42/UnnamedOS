#include "TextRenderer.h"

void TextRenderer_Create(TextRenderer* renderer, Framebuffer* framebuffer, PSF1_Font* font, uint32_t cursorX, uint32_t cursorY, uint32_t color) {
	*renderer = (TextRenderer){
		.Framebuffer = framebuffer,
		.Font = font,
		.BaseCursorX = cursorX,
		.CursorX = cursorX,
		.CursorY = cursorY,
		.Color = color,
	};
}

void TextRenderer_PutChar(TextRenderer* renderer, char character) {
	char* fontPtr = (char*)renderer->Font->GlyphBuffer + ((uint64_t)character * (uint64_t)renderer->Font->Header->CharSize);
	for (uint32_t yOff = 0; yOff < renderer->Font->Header->CharSize; yOff++) {
		for (uint32_t xOff = 0; xOff < 8; xOff++) {
			if ((*fontPtr) & (0b10000000 >> xOff)) {
				PutPixel(renderer->Framebuffer, renderer->CursorX + xOff, renderer->CursorY + yOff, renderer->Color);
			}
		}
		fontPtr++;
	}
	renderer->CursorX += 8;
}

void TextRenderer_PutString(TextRenderer* renderer, const char* string) {
	while (*string) {
		switch (*string) {
			case '\n': {
				renderer->CursorY += renderer->Font->Header->CharSize;
			} break;

			case '\r': {
				renderer->CursorX = renderer->BaseCursorX;
			} break;

			default: {
				TextRenderer_PutChar(renderer, *string);
			} break;
		}
		string++;
	}
}

void TextRenderer_PutUInt(TextRenderer* renderer, uint64_t value) {
	uint64_t temp = value;

	uint8_t stringSize;
	while (temp / 10) {
		temp /= 10;
		stringSize++;
	}

	char buffer[stringSize + 1];
	buffer[stringSize] = '\0';

	uint8_t index = 0;
	while (value / 10) {
		uint8_t remainder = value % 10;
		value /= 10;

		buffer[stringSize - index] = remainder + '0';
		index++;
	}

	uint8_t remainder = value % 10;
	buffer[stringSize - index] = remainder + '0';

	TextRenderer_PutString(renderer, buffer);
}

void TextRenderer_PutInt(TextRenderer* renderer, int64_t value) {
	if (value < 0) {
		TextRenderer_PutChar(renderer, '-');
		value *= -1;
	}
	TextRenderer_PutUInt(renderer, (uint64_t)value);
}

void TextRenderer_PutPtr(TextRenderer* renderer, void* ptr) {
	char buffer[17];
	buffer[16] = '\0';

	uint8_t size = 8 * 2 - 1;
	for (uint8_t i = 0; i < 8; i++) {
		uint8_t* valPtr = (uint8_t*)&ptr + i;
		uint8_t temp = (*valPtr & 0xF0) >> 4;
		buffer[size - (i * 2 + 1)] = temp + (temp > 9 ? 'A' - 10 : '0');
		temp = (*valPtr & 0x0F);
		buffer[size - (i * 2 + 0)] = temp + (temp > 9 ? 'A' - 10 : '0');
	}

	TextRenderer_PutString(renderer, "0x");
	TextRenderer_PutString(renderer, buffer);
}
