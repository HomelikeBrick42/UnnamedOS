#include "GDT.h"

__attribute__((aligned(0x1000))) GDT DefaultGDT = {
	.Null       = { 0, 0, 0, 0x00, 0x00, 0 },
	.KernelCode = { 0, 0, 0, 0x9A, 0xA0, 0 },
	.KernelData = { 0, 0, 0, 0x92, 0xA0, 0 },
	.UserNull   = { 0, 0, 0, 0x00, 0x00, 0 },
	.UserCode   = { 0, 0, 0, 0x9A, 0xA0, 0 },
	.UserData   = { 0, 0, 0, 0x92, 0xA0, 0 },
};
