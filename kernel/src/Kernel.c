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

extern uint64_t _KernelStart;
extern uint64_t _KernelEnd;

static void PrintMemoryUsage(TextRenderer* textRenderer) {
	textRenderer->Color = 0xFF00FF00;
	TextRenderer_PutString(textRenderer, "Total Memory Size: ");
	textRenderer->Color = 0xFFFF00FF;
	TextRenderer_PutUInt(textRenderer, PageFrameAllocator_GetMemorySize() / 1024);
	TextRenderer_PutString(textRenderer, " KB\r\n");

	textRenderer->Color = 0xFF00FF00;
	TextRenderer_PutString(textRenderer, "Free Memory Size: ");
	textRenderer->Color = 0xFFFF00FF;
	TextRenderer_PutUInt(textRenderer, PageFrameAllocator_GetFreeMemory() / 1024);
	TextRenderer_PutString(textRenderer, " KB\r\n");

	textRenderer->Color = 0xFF00FF00;
	TextRenderer_PutString(textRenderer, "Used Memory Size: ");
	textRenderer->Color = 0xFFFF00FF;
	TextRenderer_PutUInt(textRenderer, PageFrameAllocator_GetUsedMemory() / 1024);
	TextRenderer_PutString(textRenderer, " KB\r\n");

	textRenderer->Color = 0xFF00FF00;
	TextRenderer_PutString(textRenderer, "Reserved Memory Size: ");
	textRenderer->Color = 0xFFFF00FF;
	TextRenderer_PutUInt(textRenderer, PageFrameAllocator_GetReservedMemory() / 1024);
	TextRenderer_PutString(textRenderer, " KB\r\n");
}

void _start(BootInfo* bootInfo) {
	TextRenderer textRenderer; //                                                       A R G B
	TextRenderer_Create(&textRenderer, bootInfo->Framebuffer, bootInfo->Font, 10, 20, 0xFF00FF00);

	if (!PageFrameAllocator_Init(bootInfo->MemoryMapDescriptors, bootInfo->MemoryMapSize, bootInfo->MemoryMapDescriptorSize)) {
		textRenderer.Color = 0xFFFF0000;
		TextRenderer_PutString(&textRenderer, "Failed to iniialize PageFrameAllocator\r\n");
		return;
	}

	uint64_t kernelSize = (uint64_t)&_KernelEnd - (uint64_t)&_KernelStart;
	uint64_t kernelPages = kernelSize / 4096 + 1;
	PageFrameAllocator_LockPages(&_KernelStart, kernelPages);

	PrintMemoryUsage(&textRenderer);

	while (1);
}
