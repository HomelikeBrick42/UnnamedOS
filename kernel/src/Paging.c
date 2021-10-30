#include "Paging.h"
#include "PageFrameAllocator.h"
#include "Memory.h"

PageTableIndex GetPageTableIndex(uint64_t virtualAddress) {
	PageTableIndex index = (PageTableIndex){};
	virtualAddress >>= 12;
	index.P = virtualAddress & 0x1FF;
	virtualAddress >>= 9;
	index.PT = virtualAddress & 0x1FF;
	virtualAddress >>= 9;
	index.PD = virtualAddress & 0x1FF;
	virtualAddress >>= 9;
	index.PDP = virtualAddress & 0x1FF;
	return index;
}

void SetPageTable(PageTable* PML4) {
	asm ("mov %0, %%cr3" : : "r" (PML4));
}

void MapMemory(PageTable* PML4, void* virtualAddress, void* physicalAddress) {
	PageTableIndex index = GetPageTableIndex((uint64_t)virtualAddress);
	PageDirectoryEntry pde;

	pde = PML4->Entries[index.PDP];
	PageTable* pdp;
	if (!pde.Present) {
		pdp = (PageTable*)PageFrameAllocator_RequestPage();
		SetMemory(pdp, 0, 4096);
		pde.Address = (uint64_t)pdp >> 12;
		pde.Present = 1;
		pde.ReadWrite = 1;
		PML4->Entries[index.PDP] = pde;
	} else {
		pdp = (PageTable*)((uint64_t)pde.Address << 12);
	}

	pde = pdp->Entries[index.PD];
	PageTable* pd;
	if (!pde.Present) {
		pd = (PageTable*)PageFrameAllocator_RequestPage();
		SetMemory(pd, 0, 4096);
		pde.Address = (uint64_t)pd >> 12;
		pde.Present = 1;
		pde.ReadWrite = 1;
		pdp->Entries[index.PD] = pde;
	} else {
		pd = (PageTable*)((uint64_t)pde.Address << 12);
	}

	pde = pd->Entries[index.PT];
	PageTable* pt;
	if (!pde.Present) {
		pt = (PageTable*)PageFrameAllocator_RequestPage();
		SetMemory(pt, 0, 4096);
		pde.Address = (uint64_t)pt >> 12;
		pde.Present = 1;
		pde.ReadWrite = 1;
		pd->Entries[index.PT] = pde;
	} else {
		pt = (PageTable*)((uint64_t)pde.Address << 12);
	}

	pde = pt->Entries[index.P];
	pde.Address = (uint64_t)physicalAddress >> 12;
	pde.Present = 1;
	pde.ReadWrite = 1;
	pt->Entries[index.P] = pde;
}
