#include "Interrupts.h"

#include "Graphics/Renderer.h"
#include "Memory/Memory.h"
#include "IO/PS2Scancodes.h"
#include "IO/IO.h"
#include "IO/Mouse.h"

__attribute__((interrupt)) void PageFault_Handler(InteruptFrame* interuptFrame) {
	Renderer_DrawString("Page fault detected\r\n", GlobalBaseCursorX, &GlobalCursorX, &GlobalCursorY, 0xFFFF0000);
	while (true);
}

__attribute__((interrupt)) void DoubleFault_Handler(InteruptFrame* interuptFrame) {
	Renderer_DrawString("Double fault detected\r\n", GlobalBaseCursorX, &GlobalCursorX, &GlobalCursorY, 0xFFFF0000);
	while (true);
}

__attribute__((interrupt)) void GeneralProtectionFault_Handler(InteruptFrame* interuptFrame) {
	Renderer_DrawString("General Protection fault detected\r\n", GlobalBaseCursorX, &GlobalCursorX, &GlobalCursorY, 0xFFFF0000);
	while (true);
}

__attribute__((interrupt)) void KeyboardInt_Handler(InteruptFrame* interuptFrame) {
	uint8_t scancode = inb(0x60);

	static bool capslockOn = false;
	static bool shiftPressed = false;
	static uint8_t previousScancodes[5] = {};

	switch (scancode) {
		case PS2Scancode_Pressed_Backspace: {
			// TODO: Find a way to do this
		} break;

		case PS2Scancode_Pressed_Enter: {
			Renderer_DrawString("\r\n", GlobalBaseCursorX, &GlobalCursorX, &GlobalCursorY, 0xFF00FF00);
		} break;

		case PS2Scancode_Pressed_LeftShift: {
			shiftPressed = true;
		} break;

		case PS2Scancode_Released_LeftShift: {
			shiftPressed = false;
		} break;

		case PS2Scancode_Pressed_CapsLock: {
			capslockOn = !capslockOn;
			// TODO: Turn the CapsLock LED on
		} break;

		case 0x48: {
			if (previousScancodes[0] == 0xE0) {
				GlobalCursorY -= Renderer_GetCharacterHeight();
			} else {
				goto Default;
			}
		} break;

		case 0x50: {
			if (previousScancodes[0] == 0xE0) {
				GlobalCursorY += Renderer_GetCharacterHeight();
			} else {
				goto Default;
			}
		} break;

		case 0x4B: {
			if (previousScancodes[0] == 0xE0) {
				GlobalCursorX -= Renderer_GetCharacterWidth();
			} else {
				goto Default;
			}
		} break;

		case 0x4D: {
			if (previousScancodes[0] == 0xE0) {
				GlobalCursorX += Renderer_GetCharacterWidth();
			} else {
				goto Default;
			}
		} break;

		default: Default: {
			char character = (shiftPressed ^ capslockOn
				? PS2Scancode_UppercaseASCII
				: PS2Scancode_LowercaseASCII
				)[scancode];
			if (character) {
				Renderer_DrawCharacter(character, GlobalBaseCursorX, &GlobalCursorX, &GlobalCursorY, 0xFF00FF00);
			}
		} break;
	}

	CopyMemory(previousScancodes + 1, previousScancodes, 4);
	previousScancodes[0] = scancode;

	PIC_EndMaster();
}

__attribute__((interrupt)) void MouseInt_Handler(InteruptFrame* interuptFrame) {
	uint8_t mouseData = inb(0x60);
	PS2Mouse_Handle(mouseData);
	PIC_EndSlave();
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
