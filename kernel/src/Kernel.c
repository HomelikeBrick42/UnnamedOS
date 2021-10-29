#include "Framebuffer.h"
#include "PSF1_Font.h"
#include "TextRenderer.h"

#include <stdint.h>
#include <stddef.h>

void _start(Framebuffer* framebuffer, PSF1_Font* font) {
	TextRenderer textRenderer; //                                   A R G B
	TextRenderer_Create(&textRenderer, framebuffer, font, 10, 20, 0xFF00FF00);

	TextRenderer_PutString(&textRenderer, "This is a string\r\n");

	textRenderer.Color = 0xFF00FFFF;

	TextRenderer_PutString(&textRenderer, "This is a number: ");
	TextRenderer_PutUInt(&textRenderer, 123);
	TextRenderer_PutString(&textRenderer, "\r\n");

	textRenderer.Color = 0xFF0000FF;

	TextRenderer_PutString(&textRenderer, "This is a negative number: ");
	TextRenderer_PutInt(&textRenderer, -123987);
	TextRenderer_PutString(&textRenderer, "\r\n");

	textRenderer.Color = 0xFFFF00FF;

	TextRenderer_PutString(&textRenderer, "This is a pointer: ");
	TextRenderer_PutPtr(&textRenderer, framebuffer->BaseAddress);
	TextRenderer_PutString(&textRenderer, "\r\n");

	while (1);
}
