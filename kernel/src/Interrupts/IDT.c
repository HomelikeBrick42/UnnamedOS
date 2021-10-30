#include "IDT.h"

void IDTDescriptorEntry_SetOffset(IDTDescriptorEntry* idtDescriptorEntry, uint64_t offset) {
	idtDescriptorEntry->Offset0 = (uint16_t)((offset & 0x000000000000FFFF) >> 0);
	idtDescriptorEntry->Offset1 = (uint16_t)((offset & 0x00000000FFFF0000) >> 16);
	idtDescriptorEntry->Offset2 = (uint32_t)((offset & 0xFFFFFFFF00000000) >> 32);
}

uint64_t IDTDescriptorEntry_GetOffset(IDTDescriptorEntry* idtDescriptorEntry) {
	uint64_t offset = 0;
	offset |= (uint64_t)idtDescriptorEntry->Offset0 << 0;
	offset |= (uint64_t)idtDescriptorEntry->Offset1 << 16;
	offset |= (uint64_t)idtDescriptorEntry->Offset2 << 32;
	return offset;
}
