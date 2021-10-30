#include "Framebuffer.h"
#include "PSF1_Font.h"
#include "TextRenderer.h"
#include "EfiMemory.h"
#include "Memory.h"
#include "Bitmap.h"
#include "PageFrameAllocator.h"
#include "Paging.h"

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

static uint8_t SetupPagesAndMemoryMapping(BootInfo* bootInfo, TextRenderer* textRenderer) {
	if (!PageFrameAllocator_Init(bootInfo->MemoryMapDescriptors, bootInfo->MemoryMapSize, bootInfo->MemoryMapDescriptorSize)) {
		textRenderer->Color = 0xFFFF0000;
		TextRenderer_PutString(textRenderer, "Failed to iniialize PageFrameAllocator\r\n");
		return 0;
	}

	uint64_t kernelSize = (uint64_t)&_KernelEnd - (uint64_t)&_KernelStart;
	uint64_t kernelPages = kernelSize / 4096 + 1;
	PageFrameAllocator_LockPages(&_KernelStart, kernelPages);

	uint64_t framebufferBase = (uint64_t)bootInfo->Framebuffer->BaseAddress;
	uint64_t framebufferSize = (uint64_t)bootInfo->Framebuffer->BufferSize + 4096;

	PageFrameAllocator_LockPages((void*)framebufferBase, framebufferSize / 4096 + 1);

	PageTable* PML4 = (PageTable*)PageFrameAllocator_RequestPage();
	SetMemory(PML4, 0, 4096);

	for (uint64_t i = 0; i < PageFrameAllocator_GetMemorySize(); i += 4096) {
		MapMemory(PML4, (void*)i, (void*)i);
	}

	for (uint64_t i = framebufferBase; i < framebufferBase + framebufferSize; i += 4096) {
		MapMemory(PML4, (void*)i, (void*)i);
	}

	asm ("mov %0, %%cr3" : : "r" (PML4));

	SetMemory(bootInfo->Framebuffer->BaseAddress, 0, bootInfo->Framebuffer->BufferSize);

	return 1;
}

void _start(BootInfo* bootInfo) {
	TextRenderer textRenderer; //                                                       A R G B
	TextRenderer_Create(&textRenderer, bootInfo->Framebuffer, bootInfo->Font, 10, 20, 0xFF00FF00);

	if (!SetupPagesAndMemoryMapping(bootInfo, &textRenderer)) {
		return;
	}

	PrintMemoryUsage(&textRenderer);

	while (1);
}
