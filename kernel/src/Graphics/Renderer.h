#pragma once

#include "Common.h"

#include "Graphics/Framebuffer.h"
#include "Graphics/PSF1_Font.h"

void Renderer_Init(Framebuffer* framebuffer, PSF1_Font* font);
uint32_t Renderer_GetWidth(void);
uint32_t Renderer_GetHeight(void);
uint32_t Renderer_GetCharacterWidth(void);
uint32_t Renderer_GetCharacterHeight(void);
void Renderer_SwapBuffers(void);
void Renderer_Clear(uint32_t color);
void Renderer_DrawPixel(uint32_t x, uint32_t y, uint32_t color);
void Renderer_DrawCharacter(char character, uint32_t baseX, uint32_t* x, uint32_t* y, uint32_t color);
void Renderer_DrawString(const char* string, uint32_t baseX, uint32_t* x, uint32_t* y, uint32_t color);
void Renderer_DrawUInt(uint64_t value, uint32_t* x, uint32_t* y, uint32_t color);
void Renderer_DrawInt(int64_t value, uint32_t* x, uint32_t* y, uint32_t color);
void Renderer_DrawQuad(uint32_t x, uint32_t y, uint32_t width, uint32_t height, uint32_t color);

extern uint32_t GlobalBaseCursorX;
extern uint32_t GlobalCursorX;
extern uint32_t GlobalCursorY;
