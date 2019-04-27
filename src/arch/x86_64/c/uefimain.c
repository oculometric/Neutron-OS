#include <efi.h>
#include <efilib.h>

EFI_SYSTEM_TABLE *ST;

EFI_STATUS efi_main (EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE *systemTable) {
	EFI_STATUS status;
    InitializeLib(ImageHandle, SystemTable);
		__asm__("hlt");
    // Initialize graphics
    EFI_GRAPHICS_OUTPUT_PROTOCOL *graphics;
    EFI_GUID graphics_proto = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    status = SystemTable->BootServices->LocateProtocol(&graphics_proto, NULL, (void **)&graphics);
    if(status != EFI_SUCCESS) return status;
    status = init_graphics(graphics);
    if(status != EFI_SUCCESS) return status;

    // Figure out the memory map (should be identity mapping)
    boot_state.memory_map = LibMemoryMap(&boot_state.memory_map_size,
                                         &boot_state.map_key,
                                         &boot_state.descriptor_size,
                                         &boot_state.descriptor_version);
    // Exit the boot services...
    SystemTable->BootServices->ExitBootServices(ImageHandle, boot_state.map_key);
    // and set up the memory map we just found.
    SystemTable->RuntimeServices->SetVirtualAddressMap(boot_state.memory_map_size,
                                                       boot_state.descriptor_size,
                                                       boot_state.descriptor_version,
                                                       boot_state.memory_map);
    // Once we’re done we power off the machine.
    SystemTable->RuntimeServices->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL);
    for(;;) __asm__("hlt");
}
