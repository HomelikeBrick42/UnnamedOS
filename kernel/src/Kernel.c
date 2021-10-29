#include "Framebuffer.h"
#include "PSF1_Font.h"
#include "TextRenderer.h"
#include "EfiMemory.h"
#include "Memory.h"
#include "Bitmap.h"

#include <stdint.h>
#include <stddef.h>

typedef struct BootInfo {
	Framebuffer* Framebuffer;
	PSF1_Font* Font;
	EfiMemoryDescriptor* MemoryMapDescriptors;
	uint64_t MemoryMapDescriptorSize;
	uint64_t MemoryMapSize;
} BootInfo;

void _start(BootInfo* bootInfo) {
	TextRenderer textRenderer; //                                   A R G B
	TextRenderer_Create(&textRenderer, bootInfo->Framebuffer, bootInfo->Font, 10, 20, 0xFF00FF00);

	uint64_t memoryMapEntries = bootInfo->MemoryMapSize / bootInfo->MemoryMapDescriptorSize;
	uint64_t memorySize = GetMemorySize(bootInfo->MemoryMapDescriptors, memoryMapEntries, bootInfo->MemoryMapDescriptorSize);

	textRenderer.Color = 0xFF00FF00;
	TextRenderer_PutString(&textRenderer, "Total Memory Size: ");
	textRenderer.Color = 0xFFFF00FF;
	TextRenderer_PutUInt(&textRenderer, memorySize);
	TextRenderer_PutString(&textRenderer, "\r\n");

	while (1);
}
