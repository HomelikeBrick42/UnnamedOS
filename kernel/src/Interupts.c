#include "Interupts.h"
#include "TextRenderer.h"
#include "IO.h"

__attribute__((interrupt)) void PageFault_Handler(InteruptFrame* interuptFrame) {
	GlobalTextRenderer->Color = 0xFFFF0000;
	TextRenderer_PutString(GlobalTextRenderer, "Page fault detected\r\n");
	while (1);
}

__attribute__((interrupt)) void DoubleFault_Handler(InteruptFrame* interuptFrame) {
	GlobalTextRenderer->Color = 0xFFFF0000;
	TextRenderer_PutString(GlobalTextRenderer, "Double fault detected\r\n");
	while (1);
}

__attribute__((interrupt)) void GeneralProtectionFault_Handler(InteruptFrame* interuptFrame) {
	GlobalTextRenderer->Color = 0xFFFF0000;
	TextRenderer_PutString(GlobalTextRenderer, "General Protection fault detected\r\n");
	while (1);
}

__attribute__((interrupt)) void KeyboardInt_Handler(InteruptFrame* interuptFrame) {
	GlobalTextRenderer->Color = 0xFF00FF00;
	TextRenderer_PutString(GlobalTextRenderer, "Key pressed: ");
	uint8_t scancode = inb(0x60);
	TextRenderer_PutUInt(GlobalTextRenderer, scancode);
	TextRenderer_PutString(GlobalTextRenderer, "                             \r");
	PIC_EndMaster();
}

void PIC_Remap(void) {
	uint8_t a1, a2;

	a1 = inb(PIC1_DATA);
	io_wait();
	a2 = inb(PIC2_DATA);
	io_wait();

	outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();

	outb(PIC1_DATA, 0x20);
	io_wait();
	outb(PIC2_DATA, 0x28);
	io_wait();

	outb(PIC1_DATA, 4);
	io_wait();
	outb(PIC2_DATA, 2);
	io_wait();

	outb(PIC1_DATA, ICW4_8086);
	io_wait();
	outb(PIC2_DATA, ICW4_8086);
	io_wait();

	outb(PIC1_DATA, a1);
	io_wait();
	outb(PIC2_DATA, a2);
	io_wait();
}

void PIC_EndMaster(void) {
	outb(PIC1_COMMAND, PIC_EOI);
}

void PIC_EndSlave(void) {
	outb(PIC2_COMMAND, PIC_EOI);
	outb(PIC1_COMMAND, PIC_EOI);
}
