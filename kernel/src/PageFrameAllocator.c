#include "PageFrameAllocator.h"

#include "Memory.h"
#include "Bitmap.h"

static Bitmap PageBitmap;
static uint64_t MemorySize;
static uint64_t FreeMemory;
static uint64_t ReservedMemory;
static uint64_t UsedMemory;

static uint64_t GetMemorySize(EfiMemoryDescriptor* memoryMapDescriptors, uint64_t memoryMapEntries, uint64_t memoryMapDescriptorSize);
static void FreePage(void* address);
static void FreePages(void* address, uint64_t count);
static void LockPage(void* address);
static void LockPages(void* address, uint64_t count);
static void UnreservePage(void* address);
static void UnreservePages(void* address, uint64_t count);
static void ReservePage(void* address);
static void ReservePages(void* address, uint64_t count);

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

	LockPages(PageBitmap.Buffer, bitMapSize);

	for (uint64_t i = 0; i < memoryMapEntries; i++) {
		EfiMemoryDescriptor* descriptor = (EfiMemoryDescriptor*)((uint64_t)memoryMapDescriptors + i * memoryMapDescriptorSize);
		if (descriptor->Type != EfiMemoryType_ConventionalMemory) {
			ReservePages(descriptor->PhysicalAddress, descriptor->NumPages);
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

static uint64_t GetMemorySize(EfiMemoryDescriptor* memoryMapDescriptors, uint64_t memoryMapEntries, uint64_t memoryMapDescriptorSize) {
	uint64_t MemorySizeBytes = 0;

	for (uint64_t i = 0; i < memoryMapEntries; i++) {
		EfiMemoryDescriptor* descriptor = (EfiMemoryDescriptor*)((uint64_t)memoryMapDescriptors + i * memoryMapDescriptorSize);
		MemorySizeBytes += descriptor->NumPages * 4096;
	}

	return MemorySizeBytes;
}

static void FreePage(void* address) {
	uint64_t index = (uint64_t)address / 4096;
	if (!Bitmap_GetBit(&PageBitmap, index)) {
		return;
	}
	Bitmap_SetBit(&PageBitmap, index, 0);
	FreeMemory += 4096;
	UsedMemory -= 4096;
}

static void FreePages(void* address, uint64_t count) {
	for (uint64_t i = 0; i < count; i++) {
		FreePage((void*)((uint64_t)address + i * 4096));
	}
}

static void LockPage(void* address) {
	uint64_t index = (uint64_t)address / 4096;
	if (Bitmap_GetBit(&PageBitmap, index)) {
		return;
	}
	Bitmap_SetBit(&PageBitmap, index, 1);
	FreeMemory -= 4096;
	UsedMemory += 4096;
}

static void LockPages(void* address, uint64_t count) {
	for (uint64_t i = 0; i < count; i++) {
		LockPage((void*)((uint64_t)address + i * 4096));
	}
}

static void UnreservePage(void* address) {
	uint64_t index = (uint64_t)address / 4096;
	if (!Bitmap_GetBit(&PageBitmap, index)) {
		return;
	}
	Bitmap_SetBit(&PageBitmap, index, 0);
	FreeMemory += 4096;
	ReservedMemory -= 4096;
}

static void UnreservePages(void* address, uint64_t count) {
	for (uint64_t i = 0; i < count; i++) {
		UnreservePage((void*)((uint64_t)address + i * 4096));
	}
}

static void ReservePage(void* address) {
	uint64_t index = (uint64_t)address / 4096;
	if (Bitmap_GetBit(&PageBitmap, index)) {
		return;
	}
	Bitmap_SetBit(&PageBitmap, index, 1);
	FreeMemory -= 4096;
	ReservedMemory += 4096;
}

static void ReservePages(void* address, uint64_t count) {
	for (uint64_t i = 0; i < count; i++) {
		ReservePage((void*)((uint64_t)address + i * 4096));
	}
}
