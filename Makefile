arch ?= x86_64
kernel := build/BOOTX64.EFI
iso := build/neutron-$(arch).iso
usb := build/neutron-$(arch).img

linker_script := src/arch/$(arch)/linker.ld
grub_cfg := src/arch/$(arch)/grub.cfg
ovmf_dir := /usr/share/ovmf-x64
ovmf := $(ovmf_dir)/OVMF-pure-efi.fd
gnu_efi := /usr/include/gnu-efi

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
	@sudo qemu-system-x86_64 -L $(ovmf_dir)/ -bios $(ovmf) -usb -drive file=$(usb) -net none

iso: $(iso)

usb: $(usb)

$(usb): $(kernel)
	@dd if=/dev/zero of=$(usb) bs=1k count=1440
	@mformat -i $(usb) -f 1440 ::
	@mmd -i $(usb) ::/EFI
	@mmd -i $(usb) ::/EFI/BOOT
	@mcopy -i $(usb) $(kernel) ::/EFI/BOOT
#
# $(iso): $(kernel) $(grub_cfg)
# 	@echo "Generating iso file"
# 	@mkdir -p build/isofiles/boot/grub
# 	@cp $(kernel) build/isofiles/boot/kernel.bin
# 	@cp $(grub_cfg) build/isofiles/boot/grub
# 	@grub-mkrescue -o $(iso) build/isofiles 2> /dev/null
# 	@rm -r build/isofiles

$(kernel): $(c_object_files)
	@echo "Linking all"
	@mkdir -p build
	@x86_64-w64-mingw32-gcc -nostdlib -Wl,-dll -shared -Wl,--subsystem,10 -e efi_main -o $(kernel) $(c_object_files) -lgcc

# compile c files
build/arch/$(arch)/%.o: src/arch/$(arch)/c/%.c
	@echo "Compiling C source file" $<
	@mkdir -p $(shell dirname $@)
	@x86_64-w64-mingw32-gcc -ffreestanding -I$(gnu_efi)/inc -I$(gnu_efi)/x86_64 -I$(gnu_efi)/inc/protocol -c -o $@ $<
