CC = clang
CFLAGS = -target x86_64-unknown-windows \
		-ffreestanding \
		-fshort-wchar \
		-mno-red-zone \
		-Wall \
		-Werror
LDFLAGS = -target x86_64-unknown-windows \
		-nostdlib \
		-Wl,-entry:EfiMain \
		-Wl,-subsystem:efi_application \
		-fuse-ld=lld

rwildcard = $(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRCDIR := src
OBJDIR := obj
BUILDDIR = bin

SRC = $(call rwildcard,$(SRCDIR),*.c)
OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))

all: setup $(OBJS) link

setup:
	@mkdir -p $(OBJDIR)
	@mkdir -p $(BUILDDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $^ -o $@

link:
	$(CC) $(LDFLAGS) -o $(BUILDDIR)/BOOTX64.EFI $(OBJS)

buildimg:
	dd if=/dev/zero of=$(BUILDDIR)/fat.img bs=1k count=1440
	mformat -i $(BUILDDIR)/fat.img -f 1440 ::
	mmd -i $(BUILDDIR)/fat.img ::/EFI
	mmd -i $(BUILDDIR)/fat.img ::/EFI/BOOT
	mcopy -i $(BUILDDIR)/fat.img $(BUILDDIR)/BOOTX64.EFI ::/EFI/BOOT

qemu: all $(BUILDDIR)/OVMF.fd buildimg
	qemu-system-x86_64 -L OVMF_dir/ -pflash $(BUILDDIR)/OVMF.fd -usb -drive file=$(BUILDDIR)/fat.img

$(BUILDDIR)/OVMF.fd:
	wget -q -P $(BUILDDIR) https://downloads.sourceforge.net/project/edk2/OVMF/OVMF-X64-r15214.zip
	unzip $(BUILDDIR)/OVMF-X64-r15214.zip OVMF.fd -d bin
	rm -f $(BUILDDIR)/OVMF-X64-r15214.zip

clean:
	@rm -rf $(OBJDIR)
	@rm -rf $(BUILDDIR)
