#include "EfiMemory.h"

const char* EfiMemoryTypeNames[EfiMemoryType_Count] = {
	[EfiMemoryType_ReservedMemory]          = "EfiReservedMemory",
	[EfiMemoryType_LoaderCode]              = "EfiLoaderCode",
	[EfiMemoryType_LoaderData]              = "EfiLoaderData",
	[EfiMemoryType_BootServicesCode]        = "EfiBootServicesCode",
	[EfiMemoryType_BootServicesData]        = "EfiBootServicesData",
	[EfiMemoryType_RuntimeServicesCode]     = "EfiRuntimeServicesCode",
	[EfiMemoryType_RuntimeServicesData]     = "EfiRuntimeServicesData",
	[EfiMemoryType_ConventionalMemory]      = "EfiConventionalMemory",
	[EfiMemoryType_UnusableMemory]          = "EfiUnusableMemory",
	[EfiMemoryType_ACPIReclaimMemory]       = "EfiACPIReclaimMemory",
	[EfiMemoryType_ACPIMemoryNVS]           = "EfiACPIMemoryNVS",
	[EfiMemoryType_MemoryMappedIO]          = "EfiMemoryMappedIO",
	[EfiMemoryType_MemoryMappedIOPortSpace] = "EfiMemoryMappedIOPortSpace",
	[EfiMemoryType_PalCode]                 = "EfiPalCode",
};
