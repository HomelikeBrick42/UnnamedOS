typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long long u64;

typedef signed char      s8;
typedef signed short     s16;
typedef signed int       s32;
typedef signed long long s64;

_Static_assert(sizeof(u8) == 1, "Expected sizeof u8 to be 1 byte.");
_Static_assert(sizeof(u16) == 2, "Expected sizeof u16 to be 2 bytes.");
_Static_assert(sizeof(u32) == 4, "Expected sizeof u32 to be 4 bytes.");
_Static_assert(sizeof(u64) == 8, "Expected sizeof u64 to be 8 bytes.");

_Static_assert(sizeof(s8) == 1, "Expected sizeof s8 to be 1 byte.");
_Static_assert(sizeof(s16) == 2, "Expected sizeof s16 to be 2 bytes.");
_Static_assert(sizeof(s32) == 4, "Expected sizeof s32 to be 4 bytes.");
_Static_assert(sizeof(s64) == 8, "Expected sizeof s64 to be 8 bytes.");

typedef u64 EFI_Status;
enum {
	EFI_Success = 0,
};

typedef u16 Char16;
typedef void* EFI_Handle;

typedef struct EFI_SimpleTextOutputProtocol EFI_SimpleTextOutputProtocol;
typedef EFI_Status (*EFI_TextString)(EFI_SimpleTextOutputProtocol* this, Char16* string);

struct EFI_SimpleTextOutputProtocol {
	void*          Reset;
	EFI_TextString OutputString;
	void*          TestString;
	void*          QueryMode;
	void*          SetMode;
	void*          SetAttribute;
	void*          ClearScreen;
	void*          SetCursorPosition;
	void*          EnableCursor;
	void*          Mode;
};

typedef struct EFI_TableHeader {
	u64 Signature;
	u32 Revision;
	u32 HeaderSize;
	u32 CRC32;
	u32 Reserved;
} EFI_TableHeader;

typedef struct EFI_SystemTable {
	EFI_TableHeader               Header;
	Char16*                       FirmwareVendor;
	u32                           FirmwareVersion;
	EFI_Handle                    ConsoleInHandle;
	void*                         onsoleIn;
	EFI_Handle                    ConsoleOutHandle;
	EFI_SimpleTextOutputProtocol* ConsoleOut;
	EFI_Handle                    StandardErrorHandle;
	EFI_SimpleTextOutputProtocol* StandardError;
	void*                         RuntimeServices;
	void*                         BootServices;
	u64                           NumberOfTableEntries;
	void*                         ConfigurationTable;
} EFI_SystemTable;

EFI_Status EfiMain(EFI_Handle imageHandle, EFI_SystemTable* systemTable) {
	systemTable->ConsoleOut->OutputString(systemTable->ConsoleOut, L"Hello, World!");

	while (1);
	return EFI_Success;
}
