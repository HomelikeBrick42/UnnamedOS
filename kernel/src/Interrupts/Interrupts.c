#include "Interrupts.h"

#include "Graphics/TextRenderer.h"
#include "IO/Keyboard.h"
#include "IO/IO.h"

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
	uint8_t scancode = inb(0x60);

	static uint8_t CapslockOn = 0;
	static uint8_t ShiftPressed = 0;
	static uint8_t CtrlPressed = 0;

	static uint8_t PrintCharacters = 0;

	static uint8_t LastScancode = 0;

	if (scancode == 1) {
		PrintCharacters = !PrintCharacters;
	}

	if (PrintCharacters) {
		uint32_t oldColor = GlobalTextRenderer->Color;
		GlobalTextRenderer->Color = scancode <= 127 ? 0xFFFF0000 : 0xFF550000;
		TextRenderer_PutUInt(GlobalTextRenderer, scancode);
		TextRenderer_PutString(GlobalTextRenderer, "\r\n");
		GlobalTextRenderer->Color = oldColor;
	} else {
		switch (scancode) {
			case 14: {
				// TODO: Find a better way to do this
				GlobalTextRenderer->CursorX -= 8;
				TextRenderer_PutChar(GlobalTextRenderer, ' ');
				GlobalTextRenderer->CursorX -= 8;
			} break;

			case 28: {
				TextRenderer_PutString(GlobalTextRenderer, "\r\n");
			} break;

			case 29: {
				CtrlPressed = 1;
			} break;

			case 42: {
				ShiftPressed = 1;
			} break;

			case 58: {
				CapslockOn = !CapslockOn;
			} break;

			case 72: {
				if (LastScancode == 224) {
					GlobalTextRenderer->CursorY -= GlobalTextRenderer->Font->Header->CharSize;
				}
			} break;

			case 80: {
				if (LastScancode == 224) {
					GlobalTextRenderer->CursorY += GlobalTextRenderer->Font->Header->CharSize;
				}
			} break;

			case 75: {
				if (LastScancode == 224) {
					GlobalTextRenderer->CursorX -= 8;
				}
			} break;

			case 77: {
				if (LastScancode == 224) {
					GlobalTextRenderer->CursorX += 8;
				}
			} break;

			case 157: {
				CtrlPressed = 0;
			} break;

			case 170: {
				ShiftPressed = 0;
			} break;

			case 224: {
			} break;

			default: {
				const char* table =
					ShiftPressed ^ CapslockOn
					? US_QWERTY_ASCII_Uppercase_Table
					: US_QWERTY_ASCII_Lowercase_Table;

				char character = table[scancode];
				switch (character) {
					case 0: {
					} break;

					case 'c': {
						if (CtrlPressed) {
							ClearScreen(GlobalFramebuffer, 0xFF224488);
							GlobalTextRenderer->CursorX = GlobalTextRenderer->BaseCursorX;
							GlobalTextRenderer->CursorY = GlobalTextRenderer->BaseCursorY;
						} else {
							goto Default;
						}
					} break;

					default: Default: {
						TextRenderer_PutChar(GlobalTextRenderer, character);
					} break;
				}
			} break;
		}
	}

	LastScancode = scancode;

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
