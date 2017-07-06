// Uefi boot stub using gnu-efi

#include <efi.h>
#include <efilib.h>
#include <efiprot.h>

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable) 
{
    EFI_STATUS status;
    EFI_INPUT_KEY key;

    // init efi lib for use
    InitializeLib(ImageHandle, SystemTable);

    ST = SystemTable;

    Print(L"test string\n");

    status = ST->ConIn->Reset(ST->ConIn, FALSE);
    if (EFI_ERROR(status)) {
        return status;   
    }
    
    while ((status = ST->ConIn->ReadKeyStroke(ST->ConIn, &key)) == EFI_NOT_READY);

    return status;
}
