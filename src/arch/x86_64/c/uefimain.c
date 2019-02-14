#include <efi.h>
#include <efilib.h>

//#include "log.h"
EFI_SYSTEM_TABLE *ST;

EFI_STATUS efi_main (EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE *systemTable) {
	//logLn ("We're booted into UEFI!");
	EFI_STATUS Status;
    EFI_INPUT_KEY Key;

    /* Store the system table for future use in other functions */
    ST = systemTable;

    /* Say hi */
    Status = ST->ConOut->OutputString(ST->ConOut, (CHAR16 *)(L"Hello World\n"));
    if (EFI_ERROR(Status))
        return Status;

    /* Now wait for a keystroke before continuing, otherwise your
       message will flash off the screen before you see it.

       First, we need to empty the console input buffer to flush
       out any keystrokes entered before this point */
    Status = ST->ConIn->Reset(ST->ConIn, FALSE);
    if (EFI_ERROR(Status))
        return Status;

    /* Now wait until a key becomes available.  This is a simple
       polling implementation.  You could try and use the WaitForKey
       event instead if you like */
    while ((Status = ST->ConIn->ReadKeyStroke(ST->ConIn, &Key)) == EFI_NOT_READY) ;

    return Status;
}
