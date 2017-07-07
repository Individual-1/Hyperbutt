#include <efi.h>
#include <efilib.h>

#include "util.h"
#include "guid.h"

EFI_STATUS efivar_set(CHAR16 *name, UINTN *size, VOID *data, BOOLEAN persist)
{
    UINT32 attr = EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS;

    if (persist) {
        attr |= EFI_VARIABLE_NON_VOLATILE;
    }

    if (*size > EFI_MAXIMUM_VARIABLE_SIZE) {
        // input is too large for storage
        return EFI_INVALID_PARAMETER;
    }

    return uefi_call_wrapper(RT->SetVariable, 5, name, &efi_guid, attr, *size, data);
}

// Pass in an empty ptr for size and data, they will be populated by the function
EFI_STATUS efivar_get(CHAR16 *name, UINTN *size, VOID *data)
{
    EFI_STATUS status;
    UINT8 dump;

    *size = 0;

    // We don't know how big the resulting efivar will be, will this work?
    // try a failed read to get correct size, will it fail with 0?
    status = uefi_call_wrapper(RT->GetVariable, 5, name, &efi_guid, NULL, size, &dump); 
    if (status == EFI_BUFFER_TOO_SMALL) {
        data = AllocatePool(*size);
        if (!data) {
            return EFI_OUT_OF_RESOURCES;
        }

        status = uefi_call_wrapper(RT->GetVariable, 5, name, &efi_guid, NULL, size, data);

        if (status != EFI_SUCCESS) {
            FreePool(data);
            data = NULL;
        }
    }

    return status;
}
