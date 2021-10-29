#include "Framebuffer.h"
#include "PSF1_Font.h"
#include "TextRenderer.h"
#include "EfiMemory.h"

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

	/*
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
	TextRenderer_PutPtr(&textRenderer, bootInfo->Framebuffer->BaseAddress);
	TextRenderer_PutString(&textRenderer, "\r\n");
	*/

	uint64_t memoryMapEntries = bootInfo->MemoryMapSize / bootInfo->MemoryMapDescriptorSize;

	for (uint64_t i = 0; i < memoryMapEntries; i++) {
		EfiMemoryDescriptor* descriptor = (EfiMemoryDescriptor*)((uint64_t)bootInfo->MemoryMapDescriptors + i * bootInfo->MemoryMapDescriptorSize);
		textRenderer.Color = 0xFF00FF00;
		TextRenderer_PutString(&textRenderer, EfiMemoryTypeNames[descriptor->Type]);
		TextRenderer_PutChar(&textRenderer, ' ');
		textRenderer.Color = 0xFFFF00FF;
		TextRenderer_PutUInt(&textRenderer, descriptor->NumPages * 4096 / 1024);
		TextRenderer_PutString(&textRenderer, " KB");
		TextRenderer_PutString(&textRenderer, "\r\n");
	}

	while (1);
}
