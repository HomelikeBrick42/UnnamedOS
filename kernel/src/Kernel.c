#include "Common.h"
#include "Graphics/Framebuffer.h"
#include "Graphics/PSF1_Font.h"
#include "Graphics/Renderer.h"
#include "Memory/EfiMemory.h"
#include "Memory/Memory.h"
#include "Memory/Bitmap.h"
#include "Memory/Paging/PageFrameAllocator.h"
#include "Memory/Paging/Paging.h"
#include "GDT/GDT.h"
#include "Interrupts/IDT.h"
#include "Interrupts/Interrupts.h"
#include "IO/IO.h"
#include "IO/Mouse.h"

typedef struct BootInfo {
	Framebuffer* Framebuffer;
	PSF1_Font* Font;
	EfiMemoryDescriptor* MemoryMapDescriptors;
	uint64_t MemoryMapDescriptorSize;
	uint64_t MemoryMapSize;
} BootInfo;

extern uint64_t _KernelStart;
extern uint64_t _KernelEnd;

static bool SetupPagesAndMemoryMapping(BootInfo* bootInfo) {
	GDTDescriptor gdtDescriptor = {};
	gdtDescriptor.Size = sizeof(GDT) - 1;
	gdtDescriptor.Offset = (uint64_t)&DefaultGDT;
	LoadGDT(&gdtDescriptor);

	if (!PageFrameAllocator_Init(bootInfo->MemoryMapDescriptors, bootInfo->MemoryMapSize, bootInfo->MemoryMapDescriptorSize)) {
		Renderer_DrawString("Failed to iniialize PageFrameAllocator\r\n", GlobalBaseCursorX, &GlobalCursorX, &GlobalCursorY, 0xFFFF0000);
		return false;
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

	return true;
}

static IDTR GlobalIDTR;

static void SetIDTGate(void* handler, uint8_t entryOffset, uint8_t type_attributes, uint8_t selector) {
	IDTDescriptorEntry* interupt = (IDTDescriptorEntry*)(GlobalIDTR.Offset + entryOffset * sizeof(IDTDescriptorEntry));
	IDTDescriptorEntry_SetOffset(interupt, (uint64_t)handler);
	interupt->Type_Attributes = type_attributes;
	interupt->Selector = selector;
}

static void SetupInterupts(void) {
	GlobalIDTR.Limit = 0x0FFF;
	GlobalIDTR.Offset = (uint64_t)PageFrameAllocator_RequestPage();

	SetIDTGate(PageFault_Handler,              0x0E, IDT_TA_InteruptGate, 0x08);
	SetIDTGate(DoubleFault_Handler,            0x08, IDT_TA_InteruptGate, 0x08);
	SetIDTGate(GeneralProtectionFault_Handler, 0x0D, IDT_TA_InteruptGate, 0x08);
	SetIDTGate(KeyboardInt_Handler,            0x21, IDT_TA_InteruptGate, 0x08);
	SetIDTGate(MouseInt_Handler,               0x2C, IDT_TA_InteruptGate, 0x08);

	asm volatile ("lidt %0" : : "m" (GlobalIDTR));

	PIC_Remap();
}

void _start(BootInfo* bootInfo) {
	Renderer_Init(bootInfo->Framebuffer, bootInfo->Font);

	if (!SetupPagesAndMemoryMapping(bootInfo)) {
		return;
	}

	Renderer_Clear(0xFF224488);

	SetupInterupts();

	PS2Mouse_Init();

	outb(PIC1_DATA, 0b11111001);
	outb(PIC2_DATA, 0b11101111);

	asm volatile ("sti");

	while (true) {
		PS2Mouse_ProcessPacket();
		// asm volatile ("hlt");
	}
}
