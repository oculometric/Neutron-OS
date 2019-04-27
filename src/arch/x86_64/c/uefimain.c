#include <efi.h>
#include <efilib.h>
#include <efiprot.h>

EFI_STATUS
efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
    InitializeLib(ImageHandle, SystemTable);
    Print(L"Hello, world from x64!");
    for(;;) __asm__("hlt");
}
