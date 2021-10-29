#include <efi.h>
#include <efilib.h>
#include <elf.h>

#include <stdint.h>
#include <stddef.h>

typedef struct Framebuffer {
	void* BaseAddress;
	size_t BufferSize;
	uint32_t Width;
	uint32_t Height;
	uint32_t PixelsPerScanLine;
} Framebuffer;

#define PSF1_MAGIC0 0x36
#define PSF1_MAGIC1 0x04

typedef struct PSF1_Header {
	uint8_t Magic[2];
	uint8_t Mode;
	uint8_t CharSize;
} PSF1_Header;

typedef struct PSF1_Font {
	PSF1_Header* Header;
	void* GlyphBuffer;
} PSF1_Font;

EFI_FILE* LoadFile(EFI_FILE* Directory, CHAR16* Path, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
	EFI_FILE* LoadedFile;

	EFI_LOADED_IMAGE_PROTOCOL* LoadedImage;
	SystemTable->BootServices->HandleProtocol(ImageHandle, &gEfiLoadedImageProtocolGuid, (void**)&LoadedImage);

	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* FileSystem;
	SystemTable->BootServices->HandleProtocol(LoadedImage->DeviceHandle, &gEfiSimpleFileSystemProtocolGuid, (void**)&FileSystem);

	if (!Directory) {
		FileSystem->OpenVolume(FileSystem, &Directory);
	}

	EFI_STATUS s = Directory->Open(Directory, &LoadedFile, Path, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
	if (s != EFI_SUCCESS) {
		return NULL;
	}

	return LoadedFile;
}

// TODO: Move somewhere else
int memcmp(const void* a, const void* b, size_t size) {
	for (size_t i = 0; i < size; i++) {
		if (((uint8_t*)a)[i] < ((uint8_t*)b)[i]) {
			return -1;
		} else if (((uint8_t*)a)[i] > ((uint8_t*)b)[i]) {
			return 1;
		}
	}
	return 0;
}

EFI_STATUS InitialiseGOP(Framebuffer* framebuffer) {
	*framebuffer = (Framebuffer){};

	EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
	EFI_GRAPHICS_OUTPUT_PROTOCOL* gop;

	EFI_STATUS status = uefi_call_wrapper(BS->LocateProtocol, 3, &gopGuid, NULL, (void**)&gop);
	if (EFI_ERROR(status)) {
		return status;
	}

	framebuffer->BaseAddress = (void*)gop->Mode->FrameBufferBase;
	framebuffer->BufferSize = gop->Mode->FrameBufferSize;
	framebuffer->Width = gop->Mode->Info->HorizontalResolution;
	framebuffer->Height = gop->Mode->Info->VerticalResolution;
	framebuffer->PixelsPerScanLine = gop->Mode->Info->PixelsPerScanLine;

	return EFI_SUCCESS;
}

EFI_STATUS LoadPSF1Font(PSF1_Font* font, EFI_FILE* Directory, CHAR16* Path, EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable) {
	*font = (PSF1_Font){};

	EFI_FILE* fontFile = LoadFile(Directory, Path, ImageHandle, SystemTable);
	if (!fontFile) {
		return EFI_LOAD_ERROR;
	}

	SystemTable->BootServices->AllocatePool(EfiLoaderData, sizeof(PSF1_Header), (void**)&font->Header);
	UINTN size = sizeof(PSF1_Header);
	fontFile->Read(fontFile, &size, font->Header);

	if (font->Header->Magic[0] != PSF1_MAGIC0 ||
		font->Header->Magic[1] != PSF1_MAGIC1) {
		return EFI_LOAD_ERROR;
	}

	UINTN glyphBufferSize = font->Header->CharSize * ((font->Header->Mode == 1) ? 512 : 256);

	{
		fontFile->SetPosition(fontFile, sizeof(PSF1_Header));
		SystemTable->BootServices->AllocatePool(EfiLoaderData, glyphBufferSize, (void**)&font->GlyphBuffer);
		fontFile->Read(fontFile, &glyphBufferSize, font->GlyphBuffer);
	}

	return EFI_SUCCESS;
}

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) {
	InitializeLib(ImageHandle, SystemTable);

	EFI_FILE* KernelFile = LoadFile(NULL, L"kernel.elf", ImageHandle, SystemTable);
	if (!KernelFile) {
		Print(L"Failed not load kernel\n\r");
		return EFI_LOAD_ERROR;
	}

	Print(L"Kernel found\n\r");

	Elf64_Ehdr header;
	{
		UINTN FileInfoSize;
		EFI_FILE_INFO* FileInfo;
		KernelFile->GetInfo(KernelFile, &gEfiFileInfoGuid, &FileInfoSize, NULL);
		SystemTable->BootServices->AllocatePool(EfiLoaderData, FileInfoSize, (void**)&FileInfo);
		KernelFile->GetInfo(KernelFile, &gEfiFileInfoGuid, &FileInfoSize, (void**)&FileInfo);

		UINTN size = sizeof(header);
		KernelFile->Read(KernelFile, &size, &header);
	}

	if (memcmp(&header.e_ident[EI_MAG0], ELFMAG, SELFMAG) != 0 ||
		header.e_ident[EI_CLASS] != ELFCLASS64 ||
		header.e_ident[EI_DATA] != ELFDATA2LSB ||
		header.e_type != ET_EXEC ||
		header.e_machine != EM_X86_64 ||
		header.e_version != EV_CURRENT) {
		Print(L"Kernel is corrupted\n\r");
		return EFI_LOAD_ERROR;
	}

	Print(L"Kernel header verified\n\r");

	Elf64_Phdr* phdrs;
	{
		KernelFile->SetPosition(KernelFile, header.e_phoff);
		UINTN size = header.e_phnum * header.e_phentsize;
		SystemTable->BootServices->AllocatePool(EfiLoaderData, size, (void**)&phdrs);
		KernelFile->Read(KernelFile, &size, phdrs);
	}

	for (
		Elf64_Phdr* phdr = phdrs;
		(char*)phdr < (char*)phdrs + header.e_phnum * header.e_phentsize;
		phdr = (Elf64_Phdr*)((char*)phdr + header.e_phentsize)) {
		switch (phdr->p_type) {
			case PT_LOAD: {
				int pages = (phdr->p_memsz + 0x1000 - 1) / 0x1000;
				Elf64_Addr segment = phdr->p_paddr;
				SystemTable->BootServices->AllocatePages(AllocateAddress, EfiLoaderData, pages, &segment);

				KernelFile->SetPosition(KernelFile, phdr->p_offset);
				UINTN size = phdr->p_filesz;
				KernelFile->Read(KernelFile, &size, (void*)segment);
			} break;
		}
	}

	Print(L"Kernel loaded\n\r");

	Framebuffer framebuffer;
	EFI_STATUS gopStatus = InitialiseGOP(&framebuffer);
	if (EFI_ERROR(gopStatus)) {
		Print(L"Failed to initialise GOP\n\r");
		return gopStatus;
	}

	Print(L"Initialised GOP\n\r");
	Print(L"    BaseAddress = 0x%llx\n\r", framebuffer.BaseAddress);
	Print(L"    BufferSize = 0x%llx\n\r", framebuffer.BufferSize);
	Print(L"    Width = %u\n\r", framebuffer.Width);
	Print(L"    Height = %u\n\r", framebuffer.Height);
	Print(L"    PixelsPerScanLine = %u\n\r", framebuffer.PixelsPerScanLine);

	PSF1_Font font;
	EFI_STATUS fontStatus = LoadPSF1Font(&font, NULL, L"zap-light16.psf", ImageHandle, SystemTable);
	if (EFI_ERROR(fontStatus)) {
		Print(L"Failed to load font\n\r");
		return fontStatus;
	}

	Print(L"Font loaded\n\r");

	typedef __attribute__((sysv_abi)) void KernelStart(Framebuffer*, PSF1_Font*);

	KernelStart* kernelStart = (KernelStart*)header.e_entry;

	kernelStart(&framebuffer, &font);

	return EFI_SUCCESS;
}
