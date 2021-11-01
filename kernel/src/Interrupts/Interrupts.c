#include "Interrupts.h"

#include "Graphics/TextRenderer.h"
#include "Memory/Memory.h"
#include "IO/PS2Scancodes.h"
#include "IO/IO.h"

__attribute__((interrupt)) void PageFault_Handler(InteruptFrame* interuptFrame) {
	GlobalTextRenderer->Color = 0xFFFF0000;
	TextRenderer_PutString(GlobalTextRenderer, "Page fault detected\r\n");
	while (true);
}

__attribute__((interrupt)) void DoubleFault_Handler(InteruptFrame* interuptFrame) {
	GlobalTextRenderer->Color = 0xFFFF0000;
	TextRenderer_PutString(GlobalTextRenderer, "Double fault detected\r\n");
	while (true);
}

__attribute__((interrupt)) void GeneralProtectionFault_Handler(InteruptFrame* interuptFrame) {
	GlobalTextRenderer->Color = 0xFFFF0000;
	TextRenderer_PutString(GlobalTextRenderer, "General Protection fault detected\r\n");
	while (true);
}

__attribute__((interrupt)) void KeyboardInt_Handler(InteruptFrame* interuptFrame) {
	uint8_t scancode = inb(0x60);

	static bool CapslockOn = false;
	static bool ShiftPressed = false;
	static uint8_t PreviousScancodes[5] = {};

	switch (scancode) {
		case PS2Scancode_Pressed_Backspace: {
			// TODO: Find a better way to do this
			GlobalTextRenderer->CursorX -= 8;
			TextRenderer_PutChar(GlobalTextRenderer, ' ');
			GlobalTextRenderer->CursorX -= 8;
		} break;

		case PS2Scancode_Pressed_Enter: {
			TextRenderer_PutString(GlobalTextRenderer, "\r\n");
		} break;

		case PS2Scancode_Pressed_LeftShift: {
			ShiftPressed = true;
		} break;

		case PS2Scancode_Released_LeftShift: {
			ShiftPressed = false;
		} break;

		case PS2Scancode_Pressed_CapsLock: {
			CapslockOn = !CapslockOn;
			// TODO: Turn the CapsLock LED on
		} break;

		case 0x48: {
			if (PreviousScancodes[0] == 0xE0) {
				GlobalTextRenderer->CursorY -= GlobalTextRenderer->Font->Header->CharSize;
			} else {
				goto Default;
			}
		} break;

		case 0x50: {
			if (PreviousScancodes[0] == 0xE0) {
				GlobalTextRenderer->CursorY += GlobalTextRenderer->Font->Header->CharSize;
			} else {
				goto Default;
			}
		} break;

		case 0x4B: {
			if (PreviousScancodes[0] == 0xE0) {
				GlobalTextRenderer->CursorX -= 8;
			} else {
				goto Default;
			}
		} break;

		case 0x4D: {
			if (PreviousScancodes[0] == 0xE0) {
				GlobalTextRenderer->CursorX += 8;
			} else {
				goto Default;
			}
		} break;

		default: Default: {
			char character = (ShiftPressed ^ CapslockOn
				? PS2Scancode_UppercaseASCII
				: PS2Scancode_LowercaseASCII
				)[scancode];
			if (character) {
				TextRenderer_PutChar(GlobalTextRenderer, character);
			}
		} break;
	}

	CopyMemory(PreviousScancodes + 1, PreviousScancodes, 4);
	PreviousScancodes[0] = scancode;

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
