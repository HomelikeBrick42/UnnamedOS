#include "Framebuffer.h"
#include "PSF1_Font.h"
#include "TextRenderer.h"
#include "EfiMemory.h"
#include "Memory.h"
#include "Bitmap.h"
#include "PageFrameAllocator.h"

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
	TextRenderer textRenderer; //                                                       A R G B
	TextRenderer_Create(&textRenderer, bootInfo->Framebuffer, bootInfo->Font, 10, 20, 0xFF00FF00);

	if (!PageFrameAllocator_Init(bootInfo->MemoryMapDescriptors, bootInfo->MemoryMapSize, bootInfo->MemoryMapDescriptorSize)) {
		textRenderer.Color = 0xFFFF0000;
		TextRenderer_PutString(&textRenderer, "Failed to iniialize PageFrameAllocator\r\n");
		return;
	}

	textRenderer.Color = 0xFF00FF00;
	TextRenderer_PutString(&textRenderer, "Total Memory Size: ");
	textRenderer.Color = 0xFFFF00FF;
	TextRenderer_PutUInt(&textRenderer, PageFrameAllocator_GetMemorySize() / 1024);
	TextRenderer_PutString(&textRenderer, " KB");
	TextRenderer_PutString(&textRenderer, "\r\n");

	textRenderer.Color = 0xFF00FF00;
	TextRenderer_PutString(&textRenderer, "Free Memory Size: ");
	textRenderer.Color = 0xFFFF00FF;
	TextRenderer_PutUInt(&textRenderer, PageFrameAllocator_GetFreeMemory() / 1024);
	TextRenderer_PutString(&textRenderer, " KB");
	TextRenderer_PutString(&textRenderer, "\r\n");

	textRenderer.Color = 0xFF00FF00;
	TextRenderer_PutString(&textRenderer, "Used Memory Size: ");
	textRenderer.Color = 0xFFFF00FF;
	TextRenderer_PutUInt(&textRenderer, PageFrameAllocator_GetUsedMemory() / 1024);
	TextRenderer_PutString(&textRenderer, " KB");
	TextRenderer_PutString(&textRenderer, "\r\n");

	textRenderer.Color = 0xFF00FF00;
	TextRenderer_PutString(&textRenderer, "Reserved Memory Size: ");
	textRenderer.Color = 0xFFFF00FF;
	TextRenderer_PutUInt(&textRenderer, PageFrameAllocator_GetReservedMemory() / 1024);
	TextRenderer_PutString(&textRenderer, " KB");
	TextRenderer_PutString(&textRenderer, "\r\n");

	while (1);
}
