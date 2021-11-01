#include "PageFrameAllocator.h"

#include "Memory/Memory.h"
#include "Memory/Bitmap.h"

static Bitmap PageBitmap;
static uint64_t MemorySize;
static uint64_t FreeMemory;
static uint64_t ReservedMemory;
static uint64_t UsedMemory;

static uint64_t GetMemorySize(EfiMemoryDescriptor* memoryMapDescriptors, uint64_t memoryMapEntries, uint64_t memoryMapDescriptorSize);
void PageFrameAllocator_UnreservePage(void* address);
void PageFrameAllocator_UnreservePages(void* address, uint64_t count);
void PageFrameAllocator_ReservePage(void* address);
void PageFrameAllocator_ReservePages(void* address, uint64_t count);

uint8_t PageFrameAllocator_Init(EfiMemoryDescriptor* memoryMapDescriptors, uint64_t memoryMapSize, uint64_t memoryMapDescriptorSize) {
	uint64_t memoryMapEntries = memoryMapSize / memoryMapDescriptorSize;

	void* largestFreeMemorySegment = NULL;
	uint64_t largestFreeMemorySegmentSize = 0;

	for (uint64_t i = 0; i < memoryMapEntries; i++) {
		EfiMemoryDescriptor* descriptor = (EfiMemoryDescriptor*)((uint64_t)memoryMapDescriptors + i * memoryMapDescriptorSize);
		if (descriptor->Type == EfiMemoryType_ConventionalMemory) {
			if (descriptor->NumPages * 4096 > largestFreeMemorySegmentSize) {
				largestFreeMemorySegment = descriptor->PhysicalAddress;
				largestFreeMemorySegmentSize = descriptor->NumPages * 4096;
			}
		}
	}

	MemorySize = GetMemorySize(memoryMapDescriptors, memoryMapEntries, memoryMapDescriptorSize);
	FreeMemory = MemorySize;

	uint64_t bitMapSize = MemorySize / 4096 / 8 + 1;

	if (bitMapSize >= largestFreeMemorySegmentSize) {
		return 0;
	}

	PageBitmap.Size = bitMapSize * 8;
	PageBitmap.Buffer = largestFreeMemorySegment;
	SetMemory(PageBitmap.Buffer, 0, bitMapSize);

	PageFrameAllocator_LockPages(PageBitmap.Buffer, bitMapSize);

	for (uint64_t i = 0; i < memoryMapEntries; i++) {
		EfiMemoryDescriptor* descriptor = (EfiMemoryDescriptor*)((uint64_t)memoryMapDescriptors + i * memoryMapDescriptorSize);
		if (descriptor->Type != EfiMemoryType_ConventionalMemory) {
			PageFrameAllocator_ReservePages(descriptor->PhysicalAddress, descriptor->NumPages);
		}
	}

	return 1;
}

uint64_t PageFrameAllocator_GetMemorySize(void) {
	return MemorySize;
}

uint64_t PageFrameAllocator_GetFreeMemory(void) {
	return FreeMemory;
}

uint64_t PageFrameAllocator_GetUsedMemory(void) {
	return UsedMemory;
}

uint64_t PageFrameAllocator_GetReservedMemory(void) {
	return ReservedMemory;
}

static uint64_t PageBitmapIndex = 0;
void* PageFrameAllocator_RequestPage(void) {
	for (; PageBitmapIndex < PageBitmap.Size; PageBitmapIndex++) {
		if (!Bitmap_GetBit(&PageBitmap, PageBitmapIndex)) {
			void* address = (void*)(PageBitmapIndex * 4096);
			PageFrameAllocator_LockPage(address);
			return address;
		}
	}
	return NULL;
}

static uint64_t GetMemorySize(EfiMemoryDescriptor* memoryMapDescriptors, uint64_t memoryMapEntries, uint64_t memoryMapDescriptorSize) {
	uint64_t MemorySizeBytes = 0;

	for (uint64_t i = 0; i < memoryMapEntries; i++) {
		EfiMemoryDescriptor* descriptor = (EfiMemoryDescriptor*)((uint64_t)memoryMapDescriptors + i * memoryMapDescriptorSize);
		MemorySizeBytes += descriptor->NumPages * 4096;
	}

	return MemorySizeBytes;
}

void PageFrameAllocator_FreePage(void* address) {
	uint64_t index = (uint64_t)address / 4096;
	if (!Bitmap_GetBit(&PageBitmap, index)) {
		return;
	}
	if (Bitmap_SetBit(&PageBitmap, index, 0)) {
		if (index < PageBitmapIndex) {
			PageBitmapIndex = index;
		}
		FreeMemory += 4096;
		UsedMemory -= 4096;
	}
}

void PageFrameAllocator_FreePages(void* address, uint64_t count) {
	for (uint64_t i = 0; i < count; i++) {
		PageFrameAllocator_FreePage((void*)((uint64_t)address + i * 4096));
	}
}

void PageFrameAllocator_LockPage(void* address) {
	uint64_t index = (uint64_t)address / 4096;
	if (Bitmap_GetBit(&PageBitmap, index)) {
		return;
	}
	if (Bitmap_SetBit(&PageBitmap, index, 1)) {
		FreeMemory -= 4096;
		UsedMemory += 4096;
	}
}

void PageFrameAllocator_LockPages(void* address, uint64_t count) {
	for (uint64_t i = 0; i < count; i++) {
		PageFrameAllocator_LockPage((void*)((uint64_t)address + i * 4096));
	}
}

void PageFrameAllocator_UnreservePage(void* address) {
	uint64_t index = (uint64_t)address / 4096;
	if (!Bitmap_GetBit(&PageBitmap, index)) {
		return;
	}
	if (Bitmap_SetBit(&PageBitmap, index, 0)) {
		if (index < PageBitmapIndex) {
			PageBitmapIndex = index;
		}
		FreeMemory += 4096;
		ReservedMemory -= 4096;
	}
}

void PageFrameAllocator_UnreservePages(void* address, uint64_t count) {
	for (uint64_t i = 0; i < count; i++) {
		PageFrameAllocator_UnreservePage((void*)((uint64_t)address + i * 4096));
	}
}

void PageFrameAllocator_ReservePage(void* address) {
	uint64_t index = (uint64_t)address / 4096;
	if (Bitmap_GetBit(&PageBitmap, index)) {
		return;
	}
	if (Bitmap_SetBit(&PageBitmap, index, 1)) {
		FreeMemory -= 4096;
		ReservedMemory += 4096;
	}
}

void PageFrameAllocator_ReservePages(void* address, uint64_t count) {
	for (uint64_t i = 0; i < count; i++) {
		PageFrameAllocator_ReservePage((void*)((uint64_t)address + i * 4096));
	}
}
