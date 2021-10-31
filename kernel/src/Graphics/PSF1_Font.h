#pragma once

#include "Common.h"

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
