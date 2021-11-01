#include "Mouse.h"

#include "Graphics/Framebuffer.h"
#include "IO/IO.h"

void PS2Mouse_Init(void) {
	outb(0x64, 0xA8);
	PS2Mouse_Wait();

	outb(0x64, 0x20);
	PS2Mouse_WaitInput();

	uint8_t status = inb(0x60);
	status |= 0b10;
	PS2Mouse_Wait();
	outb(0x64, 0x60);
	PS2Mouse_Wait();
	outb(0x60, status);

	PS2Mouse_Write(0xF6);
	PS2Mouse_Read();

	PS2Mouse_Write(0xF4);
	PS2Mouse_Read();
}

static bool mousePacketReady = false;
static uint8_t mousePacket[4] = {};
int32_t MouseX = 0;
int32_t MouseY = 0;
void PS2Mouse_Handle(uint8_t data) {
	static uint8_t mouseCycle = 0;

	switch (mouseCycle) {
		case 0: {
			if (mousePacketReady) {
				break;
			}
			if ((data & 0b00001000) == 0) {
				break;
			}
			mousePacket[0] = data;
			mouseCycle++;
		} break;

		case 1: {
			if (mousePacketReady) {
				break;
			}
			mousePacket[1] = data;
			mouseCycle++;
		} break;

		case 2: {
			if (mousePacketReady) {
				break;
			}
			mousePacket[2] = data;
			mouseCycle = 0;
			mousePacketReady = true;
		} break;
	}
}

void PS2Mouse_ProcessPacket(void) {
	if (!mousePacketReady) {
		return;
	}
	mousePacketReady = false;

	bool xNegative = (mousePacket[0] & PS2_X_Sign)     != 0;
	bool yNegative = (mousePacket[0] & PS2_Y_Sign)     != 0;
	bool xOverflow = (mousePacket[0] & PS2_X_Overflow) != 0;
	bool yOverflow = (mousePacket[0] & PS2_Y_Overflow) != 0;

	if (!xNegative) {
		MouseX += mousePacket[1];
		if (xOverflow) {
			MouseX += 255;
		}
	} else {
		mousePacket[1] = 256 - mousePacket[1];
		MouseX -= mousePacket[1];
		if (xOverflow) {
			MouseX -= 255;
		}
	}

	if (!yNegative) {
		MouseY -= mousePacket[2];
		if (yOverflow) {
			MouseY -= 255;
		}
	} else {
		mousePacket[2] = 256 - mousePacket[2];
		MouseY += mousePacket[2];
		if (yOverflow) {
			MouseY += 255;
		}
	}

	// Do we put this here?
	if (MouseX < 0) MouseX = 0;
	if (MouseY < 0) MouseY = 0;

	if (MouseX >= GlobalFramebuffer->Width) MouseX = GlobalFramebuffer->Width;
	if (MouseY >= GlobalFramebuffer->Height) MouseY = GlobalFramebuffer->Height;

	Framebuffer_DrawQuad(GlobalFramebuffer, (uint32_t)MouseX, (uint32_t)MouseY, 10, 10, 0xFFFF0000);
}

void PS2Mouse_Write(uint8_t value) {
	PS2Mouse_Wait();
	outb(0x64, 0xD4);
	PS2Mouse_Wait();
	outb(0x60, value);
}

uint8_t PS2Mouse_Read(void) {
	PS2Mouse_WaitInput();
	return inb(0x60);
}

void PS2Mouse_Wait(void) {
	uint64_t timeout = 100000;
	while (timeout--) {
		if ((inb(0x64) & 0b10) == 0) {
			return;
		}
	}
}

void PS2Mouse_WaitInput(void) {
	uint64_t timeout = 100000;
	while (timeout--) {
		if (inb(0x64) & 0b1) {
			return;
		}
	}
}
