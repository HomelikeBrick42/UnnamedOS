#pragma once

#include "Framebuffer.h"
#include "PSF1_Font.h"

#include <stdint.h>
#include <stddef.h>

typedef struct TextRenderer {
	Framebuffer* Framebuffer;
	PSF1_Font* Font;
	uint32_t BaseCursorX;
	uint32_t CursorX;
	uint32_t CursorY;
	uint32_t Color;
} TextRenderer;

void TextRenderer_Create(TextRenderer* renderer, Framebuffer* framebuffer, PSF1_Font* font, uint32_t cursorX, uint32_t cursorY, uint32_t color);
void TextRenderer_PutChar(TextRenderer* renderer, char character);
void TextRenderer_PutString(TextRenderer* renderer, const char* string);
void TextRenderer_PutUInt(TextRenderer* renderer, uint64_t value);
void TextRenderer_PutInt(TextRenderer* renderer, int64_t value);
