arch ?= x86_64
kernel := build/BOOTX64.EFI
iso := build/neutron-$(arch).iso
usb := build/neutron-$(arch).img

linker_script := src/arch/$(arch)/linker.ld
grub_cfg := src/arch/$(arch)/grub.cfg
assembly_source_files := $(wildcard src/arch/$(arch)/asm/*.asm)
assembly_object_files := $(patsubst src/arch/$(arch)/asm/%.asm, build/arch/$(arch)/%.o, $(assembly_source_files))

c_source_files := $(wildcard src/arch/$(arch)/c/*.c src/arch/$(arch)/c/*/*.c)
c_object_files := $(patsubst src/arch/$(arch)/c/%.c, build/arch/$(arch)/%.o, $(cpp_source_files))
date    :=          `date +'%d.%m.%y_%H-%M-%S'`.log
logfile :=          log/serial/Serial-$(date)

.PHONY: all clean run usb

all: $(kernel)

cleanserial:
	@echo "Clearing serial logs"
	@rm log/serial/*

clean:
	@echo "Cleaning"
	@rm -r build

run: $(usb)
	@echo "Starting"
	@touch $(logfile)
	@qemu-system-x86_64 -L ovmf-x64/ -bios ovmf-x64/OVMF-pure-efi.fd -net none -no-reboot -usb -usbdevice disk::$(usb) -serial file:$(logfile) -no-reboot
	@#@qemu-system-x86_64 -cdrom $(iso) -serial stdio -no-reboot -m 8G

debug: $(usb)
	@echo "Starting debug"
	@echo "GDB needs 'target remote localhost:1234'"
	@touch $(logfile)
	@qemu-system-x86_64 -L ovmf-x64/ -bios ovmf-x64/OVMF-pure-efi.fd -net none -no-reboot -usb -usbdevice disk::$(usb) -serial file:$(logfile) -no-reboot -s -S -d int

iso: $(iso)

usb: $(usb)

$(usb): $(kernel)
	@dd if=/dev/zero of=$(usb) bs=1k count=1440
	@mformat -i $(usb) -f 1440 ::
	@mmd -i $(usb) ::/EFI
	@mmd -i $(usb) ::/EFI/BOOT
	@mcopy -i $(usb) $(kernel) ::/EFI/BOOT

$(iso): $(kernel) $(grub_cfg)
	@echo "Generating iso file"
	@mkdir -p build/isofiles/boot/grub
	@cp $(kernel) build/isofiles/boot/kernel.bin
	@cp $(grub_cfg) build/isofiles/boot/grub
	@grub-mkrescue -o $(iso) build/isofiles 2> /dev/null
	@rm -r build/isofiles

$(kernel): $(c_object_files) #$(linker_script) #$(assembly_object_files)
	@echo "Linking all"
	@mkdir -p build
	@x86_64-w64-mingw32.shared-gcc -nostdlib -Wl,-dll -shared -Wl,--subsystem,10 -e efi_main -o build/BOOTX64.EFI $(c_object_files) -lgcc -shared -Bsymbolic -T src/arch/x86_64/linker.ld

# compile assembly files
build/arch/$(arch)/%.o: src/arch/$(arch)/asm/%.asm
	@echo "Compiling assembly source file" $<
	@mkdir -p $(shell dirname $@)
	@nasm -f elf64 $< -o $@

# compile c files
build/arch/$(arch)/%.o: src/arch/$(arch)/c/%.c
	@echo "Compiling C source file" $<
	@mkdir -p $(shell dirname $@)
	@x86_64-w64-mingw32.shared-gcc -c $< -o $@ -ffreestanding -Ignu-efi/inc -Ignu-efi/inc/x86_64 -Ignu-efi/inc/protocol -O2 -Wall -Wextra -fno-exceptions -fshort-wchar -fno-mangle -fno-pic -fno-stack-protector -mno-red-zone -DEFI_FUNCTION_WRAPPER
