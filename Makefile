ARCH		= x86_64

HEADERS		=
OBJS		= uefi/boot.o

EFIINC			= /usr/local/include/efi
EFIINCS			= -I$(EFIINC) -I$(EFIINC)/$(ARCH) -Ii$(EFIINC)/protocol
EFILIB			= /usr/local/lib
EFI_CRT_OBJS	= $(EFILIB)/crt0-efi-$(ARCH).o
EFI_LDS			= $(EFILIB)/elf_$(ARCH)_efi.lds

CFLAGS			= $(EFIINCS) -xc -std=c11 -fno-stack-protector -fpic -fshort-wchar -mno-red-zone \
				  -Wall -Wno-incompatible-library-redeclaration -O0

ifeq ($(ARCH),x86_64)
	CFLAGS += -DHAVE_USE_MS_ABI
endif

LDFLAGS			= -nostdlib -znocombreloc -T $(EFI_LDS) -shared -Bsymbolic -L $(EFILIB) $(EFI_CRT_OBJS)

all: image.iso

run: image.iso
	qemu-system-x86_64 -L $(OVMF_DIR) -bios $(OVMF_DIR)/OVMF-pure-efi.fd -cdrom $<

clean:
	rm -rf iso
	rm -f *.iso
	rm -f *.img
	rm -f *.efi
	rm -f uefi/*.o
	rm -f *.so

image.iso: image.img
	mkdir iso
	cp $< iso
	xorriso -as mkisofs -R -f -e $< -no-emul-boot -o $@ iso

image.img: boot.efi
	dd if=/dev/zero of=$@ bs=1M count=33
	mkfs.vfat -F 32 $@
	mmd -i $@ ::/EFI
	mmd -i $@ ::/EFI/BOOT
	mcopy -i $@ boot.efi ::/EFI/BOOT/BOOTX64.EFI

boot.so: $(OBJS)
	ld.gold $(LDFLAGS) $(OBJS) -o $@ -lefi -lgnuefi

%.efi: %.so
	objcopy -j .text -j .sdata -j .data -j .dynamic -j .dynsym -j .rel -j .rela -j .reloc --target=efi-app-$(ARCH) $^ $@

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c -o $@ $<
