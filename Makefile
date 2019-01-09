arch ?= x86_64
kernel := build/kernel-$(arch).bin
iso := build/neutron-$(arch).iso

linker_script := src/arch/$(arch)/linker.ld
grub_cfg := src/arch/$(arch)/grub.cfg
assembly_source_files := $(wildcard src/arch/$(arch)/asm/*.asm)
assembly_object_files := $(patsubst src/arch/$(arch)/asm/%.asm, build/arch/$(arch)/%.o, $(assembly_source_files))

cpp_source_files := $(wildcard src/arch/$(arch)/cc/*.cc)
cpp_object_files := $(patsubst src/arch/$(arch)/cc/%.cc, build/arch/$(arch)/%.o, $(cpp_source_files))
date    :=          `date +'%d.%m.%y_%H-%M-%S'`.log
logfile :=          log/serial/Serial-$(date)

.PHONY: all clean run iso

all: $(kernel)

cleanserial:
	@echo "Clearing serial logs"
	@rm log/serial/*

clean:
	@echo "Cleaning"
	@rm -r build

run: $(iso)
	@echo "Starting"
	@touch $(logfile)
	@qemu-system-x86_64 -cdrom $(iso) -serial file:$(logfile) -no-reboot -m 8G

debug: $(iso)
	@echo "Starting debug"
	@echo "GDB needs 'target remote localhost:1234'"
	@touch $(logfile)
	@qemu-system-x86_64 -cdrom $(iso) -serial file:$(logfile) -no-reboot -m 8G -s -S

iso: $(iso)

$(iso): $(kernel) $(grub_cfg)
	@echo "Generating iso file"
	@mkdir -p build/isofiles/boot/grub
	@cp $(kernel) build/isofiles/boot/kernel.bin
	@cp $(grub_cfg) build/isofiles/boot/grub
	@grub-mkrescue -o $(iso) build/isofiles 2> /dev/null
	@rm -r build/isofiles

$(kernel): $(cpp_object_files) $(assembly_object_files) $(linker_script)
	@echo "Linking all"
	@~/opt/cross/bin/x86_64-elf-ld -n -o $(kernel) -T $(linker_script) $(cpp_object_files) $(assembly_object_files)

# compile assembly files
build/arch/$(arch)/%.o: src/arch/$(arch)/asm/%.asm
	@echo "Compiling assembly source file " $<
	@mkdir -p $(shell dirname $@)
	@nasm -f elf64 $< -o $@

# compile c++ files
build/arch/$(arch)/%.o: src/arch/$(arch)/cc/%.cc
	@echo "Compiling C++ source file " $<
	@mkdir -p $(shell dirname $@)
	@~/opt/cross/bin/x86_64-elf-g++ -c $< -o $@ -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -w
