#pragma once

#include <stdint.h>
#include <stddef.h>

typedef struct PageDirectoryEntry {
	uint8_t Present : 1;
	uint8_t ReadWrite : 1;
	uint8_t UserSuper : 1;
	uint8_t WriteThrough : 1;
	uint8_t CacheDisabled : 1;
	uint8_t Accessed : 1;
	uint8_t Ignore0 : 1;
	uint8_t LargerPages : 1;
	uint8_t Ignore1 : 1;
	uint8_t Available : 3;
	uint64_t Address : 52;
} PageDirectoryEntry;
_Static_assert(sizeof(PageDirectoryEntry) == 8, "The size must be 8");

typedef struct PageTable {
	PageDirectoryEntry Entries[512];
} __attribute__((aligned(0x1000))) PageTable;

typedef struct PageTableIndex {
	uint64_t PDP;
	uint64_t PD;
	uint64_t PT;
	uint64_t P;
} PageTableIndex;

PageTableIndex GetPageTableIndex(uint64_t virtualAddress);

void SetPageTable(PageTable* PML4);
void MapMemory(PageTable* PML4, void* virtualAddress, void* physicalAddress);
