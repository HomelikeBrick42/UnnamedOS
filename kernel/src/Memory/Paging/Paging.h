#pragma once

#include "Common.h"

typedef struct PageDirectoryEntry {
	bool Present : 1;
	bool ReadWrite : 1;
	bool UserSuper : 1;
	bool WriteThrough : 1;
	bool CacheDisabled : 1;
	bool Accessed : 1;
	bool Ignore0 : 1;
	bool LargerPages : 1;
	bool Ignore1 : 1;
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
