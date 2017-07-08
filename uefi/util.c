#include <efi.h>
#include <efilib.h>

#include "util.h"
#include "guid.h"

VOID print_efi_status(EFI_STATUS status)
{
    switch(status) {
        case EFI_SUCCESS:
            Print(L"EFI_SUCCESS\n");
            break;
        case EFI_LOAD_ERROR:
            Print(L"EFI_LOAD_ERROR\n");
            break;
        case EFI_INVALID_PARAMETER:
            Print(L"EFI_INVALID_PARAMETER\n");
            break;
        case EFI_UNSUPPORTED:
            Print(L"EFI_UNSUPPORTED\n");
            break;
        case EFI_BAD_BUFFER_SIZE:
            Print(L"EFI_BAD_BUFFER_SIZE\n");
            break;
        case EFI_BUFFER_TOO_SMALL:
            Print(L"EFI_BUFFER_TOO_SMALL\n");
            break;
        case EFI_NOT_READY:
            Print(L"EFI_NOT_READY\n");
            break;
        case EFI_DEVICE_ERROR:
            Print(L"EFI_DEVICE_ERROR\n");
            break;
        case EFI_WRITE_PROTECTED:
            Print(L"EFI_WRITE_PROTECTED\n");
            break;
        case EFI_OUT_OF_RESOURCES:
            Print(L"EFI_OUT_OF_RESOURCES\n");
            break;
        case EFI_VOLUME_CORRUPTED:
            Print(L"EFI_VOLUME_CORRUPTED\n");
            break;
        case EFI_VOLUME_FULL:
            Print(L"EFI_VOLUME_FULL\n");
            break;
        case EFI_NO_MEDIA:
            Print(L"EFI_NO_MEDIA\n");
            break;
        case EFI_MEDIA_CHANGED:
            Print(L"EFI_MEDIA_CHANGED\n");
            break;
        case EFI_NOT_FOUND:
            Print(L"EFI_NOT_FOUND\n");
            break;
        case EFI_ACCESS_DENIED:
            Print(L"EFI_ACCESS_DENIED\n");
            break;
        case EFI_NO_RESPONSE:
            Print(L"EFI_NO_RESPONSE\n");
            break;
        case EFI_NO_MAPPING:
            Print(L"EFI_NO_MAPPING\n");
            break;
        case EFI_TIMEOUT:
            Print(L"EFI_TIMEOUT\n");
            break;
        case EFI_NOT_STARTED:
            Print(L"EFI_NOT_STARTED\n");
            break;
        case EFI_ALREADY_STARTED:
            Print(L"EFI_ALREADY_STARTED\n");
            break;
        case EFI_ABORTED:
            Print(L"EFI_ABORTED\n");
            break;
        case EFI_ICMP_ERROR:
            Print(L"EFI_ICMP_ERROR\n");
            break;
        case EFI_TFTP_ERROR:
            Print(L"EFI_TFTP_ERROR\n");
            break;
        case EFI_PROTOCOL_ERROR:
            Print(L"EFI_PROTOCOL_ERROR\n");
            break;
        case EFI_INCOMPATIBLE_VERSION:
            Print(L"EFI_INCOMPATIBLE_VERSION\n");
            break;
        case EFI_SECURITY_VIOLATION:
            Print(L"EFI_SECURITY_VIOLATION\n");
            break;
        case EFI_CRC_ERROR:
            Print(L"EFI_CRC_ERROR\n");
            break;
        case EFI_END_OF_MEDIA:
            Print(L"EFI_END_OF_MEDIA\n");
            break;
        case EFI_END_OF_FILE:
            Print(L"EFI_END_OF_FILE\n");
            break;
        case EFI_WARN_DELETE_FAILURE:
            Print(L"EFI_WARN_DELETE_FAILURE\n");
            break;
        case EFI_WARN_WRITE_FAILURE:
            Print(L"EFI_WARN_WRITE_FAILURE\n");
            break;
        default:
            break;
    }

    return;
}

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
// *data must be freed by FreePool, responsibility of caller
EFI_STATUS efivar_get(CHAR16 *name, OUT UINTN *size, OUT VOID **data)
{
    EFI_STATUS status;
    UINT8 dump;

    *size = 0;

    // We don't know how big the resulting efivar will be, will this work?
    // try a failed read to get correct size, will it fail with 0?
    status = uefi_call_wrapper(RT->GetVariable, 5, name, &efi_guid, NULL, size, &dump); 
    print_efi_status(status);
    if (status == EFI_BUFFER_TOO_SMALL) {
        *data = AllocatePool(*size);
        if (!*data) {
            return EFI_OUT_OF_RESOURCES;
        }

        status = uefi_call_wrapper(RT->GetVariable, 5, name, &efi_guid, NULL, size, *data);

        if (status != EFI_SUCCESS) {
            FreePool(*data);
            *data = NULL;
        }
    }

    return status;
}
