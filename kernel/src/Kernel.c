#include "Graphics/Framebuffer.h"
#include "Graphics/PSF1_Font.h"
#include "Graphics/TextRenderer.h"
#include "Memory/EfiMemory.h"
#include "Memory/Memory.h"
#include "Memory/Bitmap.h"
#include "Memory/Paging/PageFrameAllocator.h"
#include "Memory/Paging/Paging.h"
#include "GDT/GDT.h"
#include "Interrupts/IDT.h"
#include "Interrupts/Interrupts.h"
#include "IO/IO.h"

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

static void PrintMemoryUsage(void) {
	GlobalTextRenderer->Color = 0xFF00FF00;
	TextRenderer_PutString(GlobalTextRenderer, "Total Memory Size: ");
	GlobalTextRenderer->Color = 0xFFFF00FF;
	TextRenderer_PutUInt(GlobalTextRenderer, PageFrameAllocator_GetMemorySize() / 1024);
	TextRenderer_PutString(GlobalTextRenderer, " KB\r\n");

	GlobalTextRenderer->Color = 0xFF00FF00;
	TextRenderer_PutString(GlobalTextRenderer, "Free Memory Size: ");
	GlobalTextRenderer->Color = 0xFFFF00FF;
	TextRenderer_PutUInt(GlobalTextRenderer, PageFrameAllocator_GetFreeMemory() / 1024);
	TextRenderer_PutString(GlobalTextRenderer, " KB\r\n");

	GlobalTextRenderer->Color = 0xFF00FF00;
	TextRenderer_PutString(GlobalTextRenderer, "Used Memory Size: ");
	GlobalTextRenderer->Color = 0xFFFF00FF;
	TextRenderer_PutUInt(GlobalTextRenderer, PageFrameAllocator_GetUsedMemory() / 1024);
	TextRenderer_PutString(GlobalTextRenderer, " KB\r\n");

	GlobalTextRenderer->Color = 0xFF00FF00;
	TextRenderer_PutString(GlobalTextRenderer, "Reserved Memory Size: ");
	GlobalTextRenderer->Color = 0xFFFF00FF;
	TextRenderer_PutUInt(GlobalTextRenderer, PageFrameAllocator_GetReservedMemory() / 1024);
	TextRenderer_PutString(GlobalTextRenderer, " KB\r\n");
}

static uint8_t SetupPagesAndMemoryMapping(BootInfo* bootInfo) {
	GDTDescriptor gdtDescriptor = {};
	gdtDescriptor.Size = sizeof(GDT) - 1;
	gdtDescriptor.Offset = (uint64_t)&DefaultGDT;
	LoadGDT(&gdtDescriptor);

	if (!PageFrameAllocator_Init(bootInfo->MemoryMapDescriptors, bootInfo->MemoryMapSize, bootInfo->MemoryMapDescriptorSize)) {
		GlobalTextRenderer->Color = 0xFFFF0000;
		TextRenderer_PutString(GlobalTextRenderer, "Failed to iniialize PageFrameAllocator\r\n");
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

	SetPageTable(PML4);

	return 1;
}

static IDTR GlobalIDTR;
static void SetupInterupts(void) {
	GlobalIDTR.Limit = 0x0FFF;
	GlobalIDTR.Offset = (uint64_t)PageFrameAllocator_RequestPage();

	IDTDescriptorEntry* interuptPageFault = (IDTDescriptorEntry*)(GlobalIDTR.Offset + 0x0E * sizeof(IDTDescriptorEntry));
	IDTDescriptorEntry_SetOffset(interuptPageFault, (uint64_t)PageFault_Handler);
	interuptPageFault->Type_Attributes = IDT_TA_InteruptGate;
	interuptPageFault->Selector = 0x08;

	IDTDescriptorEntry* interuptDoubleFault = (IDTDescriptorEntry*)(GlobalIDTR.Offset + 0x08 * sizeof(IDTDescriptorEntry));
	IDTDescriptorEntry_SetOffset(interuptDoubleFault, (uint64_t)DoubleFault_Handler);
	interuptDoubleFault->Type_Attributes = IDT_TA_InteruptGate;
	interuptDoubleFault->Selector = 0x08;

	IDTDescriptorEntry* interuptGeneralProtectionFault = (IDTDescriptorEntry*)(GlobalIDTR.Offset + 0x0D * sizeof(IDTDescriptorEntry));
	IDTDescriptorEntry_SetOffset(interuptGeneralProtectionFault, (uint64_t)GeneralProtectionFault_Handler);
	interuptGeneralProtectionFault->Type_Attributes = IDT_TA_InteruptGate;
	interuptGeneralProtectionFault->Selector = 0x08;

	IDTDescriptorEntry* interuptKeyboard = (IDTDescriptorEntry*)(GlobalIDTR.Offset + 0x21 * sizeof(IDTDescriptorEntry));
	IDTDescriptorEntry_SetOffset(interuptKeyboard, (uint64_t)KeyboardInt_Handler);
	interuptKeyboard->Type_Attributes = IDT_TA_InteruptGate;
	interuptKeyboard->Selector = 0x08;

	asm volatile ("lidt %0" : : "m" (GlobalIDTR));

	PIC_Remap();

	outb(PIC1_DATA, 0b11111101);
	outb(PIC2_DATA, 0b11111111);

	asm volatile ("sti");
}

void _start(BootInfo* bootInfo) {
	GlobalFramebuffer = bootInfo->Framebuffer;

	//                           A R G B
	uint32_t backgroundColor = 0xFF224488;

	TextRenderer textRenderer;
	TextRenderer_Create(&textRenderer, GlobalFramebuffer, bootInfo->Font, 10, 20, backgroundColor, 0xFF00FF00);

	GlobalTextRenderer = &textRenderer;

	if (!SetupPagesAndMemoryMapping(bootInfo)) {
		return;
	}

	ClearScreen(GlobalFramebuffer, backgroundColor);

	SetupInterupts();

	PrintMemoryUsage();
	
	GlobalTextRenderer->Color = 0xFF00FF00;

	while (1) {
		asm volatile ("hlt");
	}
}
